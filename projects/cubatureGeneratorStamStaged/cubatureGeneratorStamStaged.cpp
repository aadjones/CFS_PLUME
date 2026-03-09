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
#include <iostream>

#include "EIGEN.h"

#include "SUBSPACE_FLUID_3D_EIGEN.h"
#include "FLUID_3D_MIC.h"
#include "CUBATURE_GENERATOR_EIGEN.h"
#include "MATRIX.h"
#include "SIMPLE_PARSER.h"
#include <sys/stat.h>

// Create a directory (and parents) without shelling out
static void ensureDirectoryExists(const string& path) {
  string built;
  for (size_t i = 0; i < path.size(); i++) {
    built += path[i];
    if (path[i] == '/' || i == path.size() - 1)
      mkdir(built.c_str(), 0755);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
  TIMER functionTimer(__FUNCTION__);
  // read in the cfg file
  if (argc != 2)
  {
    cout << " Usage: " << argv[0] << " *.cfg" << endl;
    return 0;
  }
  SIMPLE_PARSER parser(argv[1]);

  cout << "================================================================" << endl;
  cout << " Loading up reduced model " << endl;
  cout << "================================================================" << endl;
  // create the subspace integrator and the cubature generator
  int xRes = parser.getInt("xRes", 48);
  int yRes = parser.getInt("yRes", 64);
  int zRes = parser.getInt("zRes", 48);
  string snapshotPath = parser.getString("snapshot path", "./data/dummy/");
  string reducedPath = parser.getString("reduced path", "./data/reduced.dummy/");
  int reducedSnapshots = parser.getInt("reduced snapshots", 20);
  int candidatesPerTry = parser.getInt("candidates per iteration", 1000);
  Real cubatureError = parser.getFloat("max cubature error", 0.01);
  bool grabAllCandidates = parser.getBool("all cells are candiates", false);
  int importanceSamples = parser.getInt("importance samples", 17000);
  cout << " Using " << importanceSamples << " importance samples " << endl;

  bool usingIOP = parser.getBool("iop", false);
  if (usingIOP) { cout << "Using IOP: true" << endl; }
  else { cout << "Using IOP: false" << endl; }

  unsigned int boundaries[6];
  boundaries[0] = parser.getInt("front", 1);
  boundaries[1] = parser.getInt("back", 1);
  boundaries[2] = parser.getInt("left", 1);
  boundaries[3] = parser.getInt("right", 1);
  boundaries[4] = parser.getInt("top", 0);

  string names[] = {"front", "back", "left", "right", "top", "bottom"};
  for (int x = 0; x < 6; x++)
  {
    cout << " Boundary on " << names[x].c_str() << "\tis set to " << flush;
    if (boundaries[x] == 0)
      cout << "Neumann " << endl;
    else
      cout << "Dirichlet " << endl;
  }
  double discardThreshold = parser.getFloat("discard threshold", 1e-20);
  cout << " Using discard threshold: " << discardThreshold << endl;

  // create the reduced order directory if necessary
  ensureDirectoryExists(reducedPath);
  cout << " dims: " << xRes << " " << yRes << " " << zRes << endl;

	SUBSPACE_FLUID_3D_EIGEN subspaceFluid(xRes, yRes, zRes, reducedPath, &boundaries[0], usingIOP, true);
  subspaceFluid.loadCubatureTrainingBases();

  CUBATURE_GENERATOR_EIGEN generator(&subspaceFluid);
  generator.errorTolerance() = cubatureError;
  generator.candidatesPerTry() = candidatesPerTry;
  generator.grabAllCandidates() = grabAllCandidates;
  generator.importanceSamples() = importanceSamples;

  // load up the training set
  FLUID_3D_MIC fluid;
  vector<VectorXd>& preadvection = generator.trainingPreadvection();
  vector<VectorXd>& postadvection = generator.trainingPostadvection();
  
  // look for training samples
  string trainingFilename = reducedPath + string("training.snapshots");
  cout << " Looking for cached training samples ... " << flush;
  FILE* file = fopen(trainingFilename.c_str(), "rb");

  const MatrixXd& preadvectU = subspaceFluid.preadvectU();
  const MatrixXd& prediffuseU = subspaceFluid.prediffuseU();

  // load up the training samples
  if (file == NULL)
  {
    cout << " None found! " << endl;

    cout << "================================================================" << endl;
    cout << " Loading up training samples " << endl;
    cout << "================================================================" << endl;

    // get the dimensions
    FILE* file;
    string dimsFilename = snapshotPath + string("velocity.preadvect.matrix.dims");
    file = fopen(dimsFilename.c_str(), "rb");
    if (file == NULL)
    {
      cout << __FILE__ << " " << __LINE__ << " : File " << dimsFilename.c_str() << " not found! " << endl;
      exit(0);
    }
    int rows, cols;
    fread((void*)&rows, sizeof(int), 1, file);
    fread((void*)&cols, sizeof(int), 1, file);
    fclose(file);

    // open the snapshots file
    string filenamePreadvect = snapshotPath + string("velocity.preadvect.matrix.transpose");
    FILE* filePreadvect = fopen(filenamePreadvect.c_str(), "rb");
    if (filePreadvect == NULL)
    {
      cout << "Couldn't open file " << filenamePreadvect.c_str() << "!!!" << endl;
      exit(0);
    }
    string filenamePrediffuse = snapshotPath + string("velocity.prediffuse.matrix.transpose");
    FILE* filePrediffuse = fopen(filenamePrediffuse.c_str(), "rb");
    if (filePrediffuse == NULL)
    {
      cout << "Couldn't open file " << filenamePrediffuse.c_str() << "!!!" << endl;
      exit(0);
    }

    for (int x = 0; x < reducedSnapshots; x++)
    {
      VectorXd preadvect, prediffuse;
      EIGEN::readRaw(filePreadvect, cols, preadvect);
      EIGEN::readRaw(filePrediffuse, cols, prediffuse);

      preadvection.push_back(preadvectU.transpose() * preadvect);
      postadvection.push_back(prediffuseU.transpose() * prediffuse);

      // do a sanity check on the projection error of preadvection
      VectorXd approx = preadvectU * (preadvectU.transpose() * preadvect);
      VectorXd diff = preadvect - approx;
      cout << " Preadvection projection error: " << diff.norm() << endl;

      approx = prediffuseU * (prediffuseU .transpose() * prediffuse);
      diff = prediffuse - approx;
      cout << " Postadvection projection error: " << diff.norm() << endl;
    }
    // cache results to a file
    file = fopen(trainingFilename.c_str(), "wb");
    int numSamples = reducedSnapshots;
    int preadvectCols = preadvectU.cols();
    int prediffuseCols = prediffuseU.cols();
    fwrite((void*)&numSamples, sizeof(int), 1, file);
    fwrite((void*)&preadvectCols, sizeof(int), 1, file);
    fwrite((void*)&prediffuseCols, sizeof(int), 1, file);
    for (int x = 0; x < reducedSnapshots; x++)
    {
      EIGEN::write(file, preadvection[x]);
      EIGEN::write(file, postadvection[x]);
    }
    fclose(file);
    fclose(filePreadvect);
    fclose(filePrediffuse);
  }
  else
  {
    // Read cached training samples, but validate dimensions match current basis
    int cachedSamples, cachedPreadvectCols, cachedPrediffuseCols;
    fread((void*)&cachedSamples, sizeof(int), 1, file);
    fread((void*)&cachedPreadvectCols, sizeof(int), 1, file);
    fread((void*)&cachedPrediffuseCols, sizeof(int), 1, file);
    fclose(file);

    bool cacheValid = (cachedPreadvectCols == preadvectU.cols() &&
                       cachedPrediffuseCols == prediffuseU.cols());

    if (cacheValid)
    {
      cout << " Found valid cache (" << cachedSamples << " samples, "
           << cachedPreadvectCols << "/" << cachedPrediffuseCols << " cols)" << endl;
      file = fopen(trainingFilename.c_str(), "rb");
      // skip the 3-int header
      fseek(file, 3 * sizeof(int), SEEK_SET);
      for (int x = 0; x < cachedSamples; x++)
      {
        VectorXd pre, post;
        int preSize, postSize;
        fread((void*)&preSize, sizeof(int), 1, file);
        pre.resize(preSize);
        for (int i = 0; i < preSize; i++) {
          double val;
          fread((void*)&val, sizeof(double), 1, file);
          pre[i] = val;
        }
        fread((void*)&postSize, sizeof(int), 1, file);
        post.resize(postSize);
        for (int i = 0; i < postSize; i++) {
          double val;
          fread((void*)&val, sizeof(double), 1, file);
          post[i] = val;
        }
        preadvection.push_back(pre);
        postadvection.push_back(post);
      }
      fclose(file);
    }
    else
    {
      cout << " Cache stale (basis changed from " << cachedPreadvectCols << "/"
           << cachedPrediffuseCols << " to " << preadvectU.cols() << "/"
           << prediffuseU.cols() << "), regenerating..." << endl;
      // Delete stale cache and regenerate from snapshots
      remove(trainingFilename.c_str());

      string dimsFilename = snapshotPath + string("velocity.preadvect.matrix.dims");
      file = fopen(dimsFilename.c_str(), "rb");
      if (file == NULL) {
        cout << __FILE__ << " " << __LINE__ << " : File " << dimsFilename.c_str() << " not found! " << endl;
        exit(0);
      }
      int rows, cols;
      fread((void*)&rows, sizeof(int), 1, file);
      fread((void*)&cols, sizeof(int), 1, file);
      fclose(file);

      FILE* filePreadvect = fopen((snapshotPath + string("velocity.preadvect.matrix.transpose")).c_str(), "rb");
      FILE* filePrediffuse = fopen((snapshotPath + string("velocity.prediffuse.matrix.transpose")).c_str(), "rb");
      if (!filePreadvect || !filePrediffuse) {
        cout << "Couldn't open snapshot files!" << endl;
        exit(0);
      }

      for (int x = 0; x < reducedSnapshots; x++)
      {
        VectorXd preadvect, prediffuse;
        EIGEN::readRaw(filePreadvect, cols, preadvect);
        EIGEN::readRaw(filePrediffuse, cols, prediffuse);
        preadvection.push_back(preadvectU.transpose() * preadvect);
        postadvection.push_back(prediffuseU.transpose() * prediffuse);
      }
      fclose(filePreadvect);
      fclose(filePrediffuse);

      // Write new cache with header
      file = fopen(trainingFilename.c_str(), "wb");
      int numSamples = reducedSnapshots;
      int preadvectCols = preadvectU.cols();
      int prediffuseCols = prediffuseU.cols();
      fwrite((void*)&numSamples, sizeof(int), 1, file);
      fwrite((void*)&preadvectCols, sizeof(int), 1, file);
      fwrite((void*)&prediffuseCols, sizeof(int), 1, file);
      for (int x = 0; x < reducedSnapshots; x++) {
        EIGEN::write(file, preadvection[x]);
        EIGEN::write(file, postadvection[x]);
      }
      fclose(file);
    }
  }

  cout << "================================================================" << endl;
  cout << " Generating greedy refined cubature for input basis rank " << preadvectU.cols() << endl;
  cout << "   and output basis rank " << prediffuseU.cols() << endl;
  cout << "================================================================" << endl;

  generator.generateImportanceSampledCubature();
  
  generator.writeCubature(reducedPath + string("cubature"));
}
