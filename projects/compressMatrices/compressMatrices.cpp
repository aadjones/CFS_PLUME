/*
 This file is part of SSFR (Zephyr).
 
 Zephyr is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Zephyr is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Zephyr.  If not, see <http://www.gnu.org/licenses/>.
 
 Copyright 2013 Theodore Kim
 */

/*
 * Multi-dimensional DCT testing
 * Aaron Demby Jones
 * Fall 2014
 */

#include <iostream>
#include <fftw3.h>
#include "EIGEN.h"
#include "SUBSPACE_FLUID_3D_EIGEN.h"
#include "FLUID_3D_MIC.h"
#include "CUBATURE_GENERATOR_EIGEN.h"
#include "MATRIX.h"
#include "BIG_MATRIX.h"
#include "SIMPLE_PARSER.h"
#include "COMPRESSION.h"
#include <string>
#include <cmath>
#include <cfenv>
#include <climits>
#include "VECTOR3_FIELD_3D.h"
#include <sys/stat.h>
#include <errno.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

using std::vector;
using std::string;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::stringstream;
using std::put_time;
using std::localtime;
using std::setfill;
using std::setw;

////////////////////////////////////////////////////////
// Function Declarations
////////////////////////////////////////////////////////

// Helper function to get current time as string
string getCurrentTime();

// set the damping matrix and compute the number of blocks
void PreprocessEncoder(COMPRESSION_DATA* data0, COMPRESSION_DATA* data1, COMPRESSION_DATA* data2, int maxIterations, const char* filename);

// check if a file exists
bool fileExists(const string& filename);

// get the length of a file in bytes
unsigned long long FileSize(const string& filename);

// compute and print the compression ratios
double GetCompressionRatios(const string& preadvectFilename, const string& finalFilename);

// update the cfg file to point to the correct compression path
void UpdateCfgFile(int roundedOverallCompression);

// Helper function to create directory if it doesn't exist
bool createDirectoryIfNotExists(const string& path);

string preadvectPath;
string finalPath;
string cfgFilename;

////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
  TIMER functionTimer(__FUNCTION__);
  
  cout << "\n=== Starting compression process at " << getCurrentTime() << " ===" << endl;
  
  // read in the cfg file
  if (argc != 2) {
    cout << "Error: Usage: " << argv[0] << " *.cfg" << endl;
    return 1;
  }
  
  cout << "Reading configuration from: " << argv[1] << endl;
  SIMPLE_PARSER parser(argv[1]);
  cfgFilename = argv[1];
  string reducedPath = parser.getString("reduced path", "./data/reduced.dummy/"); 
  
  cout << "Creating reduced path directory: " << reducedPath << endl;
  // Create reduced path directory if it doesn't exist
  if (!createDirectoryIfNotExists(reducedPath)) {
    cout << "Error: Failed to create reduced path directory: " << reducedPath << endl;
    return 1;
  }

  cout << "\n=== Reading simulation parameters ===" << endl;
  int xRes = parser.getInt("xRes", 48);
  int yRes = parser.getInt("yRes", 64);
  int zRes = parser.getInt("zRes", 48);
  bool usingIOP = parser.getBool("iop", 0);
  cout << "Simulation dimensions: " << xRes << "x" << yRes << "x" << zRes << endl;
  cout << "Using IOP: " << (usingIOP ? "Yes" : "No") << endl;
  cout << "Block size: " << BLOCK_SIZE << endl;

  // we want the peeled resolutions for the matrices
  xRes -= 2;
  yRes -= 2;
  zRes -= 2;
  cout << "Peeled dimensions: " << xRes << "x" << yRes << "x" << zRes << endl;

  VEC3I dims(xRes, yRes, zRes);

  cout << "\n=== Reading compression parameters ===" << endl;
  int nBits = parser.getInt("nBits", 24);
  double percent = parser.getFloat("percent", 0.99);
  int maxIterations = parser.getInt("maxIterations", 32);

  cout << "Compression settings:" << endl;
  cout << "  - Bits per value: " << nBits << endl;
  cout << "  - Compression percentage: " << percent << endl;
  cout << "  - Max iterations: " << maxIterations << endl;

  cout << "\n=== Loading input matrices ===" << endl;
  preadvectPath = reducedPath + string("U.preadvect.matrix");
  finalPath = reducedPath + string("U.final.matrix");

  // Load matrices first so we know the actual column counts
  MatrixXd U_preadvect;
  MatrixXd U_final;

  cout << "Reading preadvect matrix from: " << preadvectPath << endl;
  if (!fileExists(preadvectPath)) {
    cout << "Error: Preadvect matrix file not found: " << preadvectPath << endl;
    return 1;
  }
  EIGEN::read(preadvectPath, U_preadvect);
  cout << "Successfully read preadvect matrix: " << U_preadvect.rows() << " x " << U_preadvect.cols() << endl;

  cout << "Reading final matrix from: " << finalPath << endl;
  if (!fileExists(finalPath)) {
    cout << "Error: Final matrix file not found: " << finalPath << endl;
    return 1;
  }
  EIGEN::read(finalPath, U_final);
  cout << "Successfully read final matrix: " << U_final.rows() << " x " << U_final.cols() << endl;

  // Use actual column counts from the loaded matrices
  int preadvectCols = U_preadvect.cols();
  int finalCols = U_final.cols();
  cout << "Preadvect basis columns: " << preadvectCols << endl;
  cout << "Final basis columns: " << finalCols << endl;

  cout << "\n=== Initializing compression data ===" << endl;
  COMPRESSION_DATA preadvect_compression_data0(dims, preadvectCols, nBits, percent);
  COMPRESSION_DATA preadvect_compression_data1(dims, preadvectCols, nBits, percent);
  COMPRESSION_DATA preadvect_compression_data2(dims, preadvectCols, nBits, percent);
  COMPRESSION_DATA final_compression_data0(dims, finalCols, nBits, percent);
  COMPRESSION_DATA final_compression_data1(dims, finalCols, nBits, percent);
  COMPRESSION_DATA final_compression_data2(dims, finalCols, nBits, percent);

  cout << "Setting up scratch directory..." << endl;
  string scratchPath = "./scratch/";
  if (!createDirectoryIfNotExists(scratchPath)) {
    cout << "Error: Failed to create scratch directory: " << scratchPath << endl;
    return 1;
  }

  string preadvectSingularFilename = scratchPath + string("velocity.preadvect.matrix.singularValues.vector");
  string finalSingularFilename = scratchPath + string("velocity.final.matrix.singularValues.vector");

  cout << "\n=== Preprocessing encoder data ===" << endl;
  cout << "Processing preadvect data..." << endl;
  PreprocessEncoder(&preadvect_compression_data0, &preadvect_compression_data1, &preadvect_compression_data2, 
      maxIterations, preadvectSingularFilename.c_str());
  cout << "Processing final data..." << endl;
  PreprocessEncoder(&final_compression_data0, &final_compression_data1, &final_compression_data2,
      maxIterations, finalSingularFilename.c_str());

  cout << "\n=== Setting up output directories ===" << endl;
  string tmpDir = reducedPath + string("tmp");
  if (!createDirectoryIfNotExists(tmpDir)) {
    cout << "Error: Failed to create tmp directory: " << tmpDir << endl;
    return 1;
  }

  string preadvectFilename = reducedPath + string("tmp/U.preadvect.component");
  string finalFilename = reducedPath + string("tmp/U.final.component");

  cout << "\n=== Starting compression ===" << endl;
  cout << "Compressing preadvect matrix..." << endl;
  CompressAndWriteMatrixComponents(preadvectFilename.c_str(), U_preadvect, &preadvect_compression_data0,
    &preadvect_compression_data1, &preadvect_compression_data2);

  cout << "Compressing final matrix..." << endl;
  CompressAndWriteMatrixComponents(finalFilename.c_str(), U_final, &final_compression_data0,
    &final_compression_data1, &final_compression_data2);

  cout << "\n=== Computing compression ratios ===" << endl;
  double ratio = GetCompressionRatios(preadvectFilename, finalFilename);
  int roundedRatio = rint(ratio);
  cout << "Rounded compression ratio: " << roundedRatio << ":1" << endl;

  cout << "\n=== Finalizing output ===" << endl;
  string newName = reducedPath + to_string(roundedRatio) + string("to1");
  string rename = string("mv ") + reducedPath + string("tmp ") + newName;
  cout << "Moving compressed data to: " << newName << endl;
  system(rename.c_str());
  
  string pbrtDir = newName + string("/pbrt");
  if (!createDirectoryIfNotExists(pbrtDir)) {
    cout << "Error: Failed to create pbrt directory: " << pbrtDir << endl;
    return 1;
  }

  cout << "Updating configuration file..." << endl;
  UpdateCfgFile(roundedRatio);

  cout << "\n=== Compression process completed at " << getCurrentTime() << " ===" << endl;
  cout << "Timing information:" << endl;
  TIMER::printTimings();
  
  return 0;
}

void PreprocessEncoder(COMPRESSION_DATA* data0, COMPRESSION_DATA* data1, COMPRESSION_DATA* data2, int maxIterations, const char* filename)
{
  // set integer rounding 'to nearest' 
  fesetround(FE_TONEAREST);
  
  // precompute and set the damping  and zigzag arrays
  data0->set_dampingArray();
  data1->set_dampingArray();
  data2->set_dampingArray();

  data0->set_zigzagArray();
  data1->set_zigzagArray();
  data2->set_zigzagArray();
   
  // fill in the appropriate paddings
  const VEC3I& dims = data0->get_dims();
  VEC3I paddings;
  GetPaddings(dims, &paddings);
  paddings += dims;

  data0->set_paddedDims(paddings);
  data1->set_paddedDims(paddings);
  data2->set_paddedDims(paddings);

  // calculates number of blocks, assuming BLOCK_SIZE 
  int numBlocks = paddings[0] * paddings[1] * paddings[2] / (BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE);

  data0->set_numBlocks(numBlocks);
  data1->set_numBlocks(numBlocks);
  data2->set_numBlocks(numBlocks);

  // set the maxIterations
  data0->set_maxIterations(maxIterations);
  data1->set_maxIterations(maxIterations);
  data2->set_maxIterations(maxIterations);
  
  // set the singular values
  // DEBUG: let's leave this out for now.
  
  /*
  data0->set_singularValues(filename);
  data1->set_singularValues(filename);
  data2->set_singularValues(filename);
  */
}

//////////////////////////////////////////////////////////////////////
// check if a file exists
//////////////////////////////////////////////////////////////////////
bool fileExists(const string& filename)
{
  FILE* file;
  file = fopen(filename.c_str(), "rb");
  
  if (file == NULL)
    return false;

  fclose(file);
  return true;
}


//////////////////////////////////////////////////////////////////////
// get the size of a file in bytes
//////////////////////////////////////////////////////////////////////
unsigned long long FileSize(const string& filename)
{
  TIMER functionTimer(__FUNCTION__);
  FILE* file;
  file = fopen(filename.c_str(), "rb");
  unsigned long long size;

  if (file == NULL) perror ("Error opening file");
  else {
  fseek(file, 0, SEEK_END);   // non-portable
  size = (unsigned long long) ftell(file);
  fclose (file);
  }
  return size;
}

//////////////////////////////////////////////////////////////////////
// compute and print the compression ratios
//////////////////////////////////////////////////////////////////////
double GetCompressionRatios(const string& preadvectFilename, const string& finalFilename)
{
  TIMER functionTimer(__FUNCTION__);
  puts("Computing compression ratios...");

  auto preadvectSize = FileSize(preadvectPath);
  auto finalSize     = FileSize(finalPath);

  string compressedPreadvect0 = preadvectFilename + '0';
  string compressedPreadvect1 = preadvectFilename + '1';
  string compressedPreadvect2 = preadvectFilename + '2';
  auto preadvectSize0 = FileSize(compressedPreadvect0);
  auto preadvectSize1 = FileSize(compressedPreadvect1);
  auto preadvectSize2 = FileSize(compressedPreadvect2);

  string compressedFinal0 = finalFilename + '0';
  string compressedFinal1 = finalFilename + '1';
  string compressedFinal2 = finalFilename + '2';
  auto finalSize0 = FileSize(compressedFinal0);
  auto finalSize1 = FileSize(compressedFinal1);
  auto finalSize2 = FileSize(compressedFinal2);

  double preadvectCompression = preadvectSize / (double) (preadvectSize0 + preadvectSize1 + preadvectSize2);
  double finalCompression = finalSize / (double) (finalSize0 + finalSize1 + finalSize2);
  double overallCompression = 0.5 * (preadvectCompression + finalCompression);

  printf("U.preadvect compression ratio is %f : 1\n", preadvectCompression);
  printf("U.final compression ratio is %f : 1\n", finalCompression);
  printf("Overall compression ratio is %f: 1\n", overallCompression);
  return overallCompression;
}

// automatically update the compression and movie paths based on compression ratio. calls a python script inside ./cfg
void UpdateCfgFile(int roundedOverallCompression)
{
  string cmd = string("python3 ./cfg/findReplace.py ") + cfgFilename + string(" ") + to_string(roundedOverallCompression);
  system(cmd.c_str());
}

// Helper function to create directory if it doesn't exist
bool createDirectoryIfNotExists(const string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        // Directory doesn't exist, try to create it
        #ifdef _WIN32
            int status = mkdir(path.c_str());
        #else
            int status = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        #endif
        if (status != 0) {
            cout << "Error creating directory " << path << ": " << strerror(errno) << endl;
            return false;
        }
        cout << "Created directory: " << path << endl;
    } else if (!(info.st_mode & S_IFDIR)) {
        cout << "Error: " << path << " exists but is not a directory" << endl;
        return false;
    }
    return true;
}

// Helper function to get current time as string
string getCurrentTime() {
    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);
    auto now_ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    stringstream ss;
    ss << put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << setfill('0') << setw(3) << now_ms.count();
    return ss.str();
}
