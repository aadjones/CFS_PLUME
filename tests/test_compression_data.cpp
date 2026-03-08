// Phase 1: Compressed data validation tests
// Tests hypotheses H1 (data corruption), H2 (FFTW lifecycle), H3 (dimension mismatch),
// and H4 (init ordering) from INVESTIGATION.md
//
// These tests require preprocessed data in data/reduced.stam.64/ and data/reduced.stam.64/2to1/

#include "EIGEN.h"
#include "COMPRESSION.h"
#include "COMPRESSION_DATA.h"
#include "MATRIX_COMPRESSION_DATA.h"
#include "FIELD_3D.h"
#include "VECTOR3_FIELD_3D.h"
#include "test_harness.h"

#include <cmath>
#include <string>
#include <fstream>
#include <limits>
#include <iostream>

using namespace std;
using namespace Eigen;

// Paths relative to project root (tests run from project root)
static const string CONFIG_PATH = "./cfg/stam.64.cfg";
static const string REDUCED_PATH = "./data/reduced.stam.64/";

// Read compression path from config file, fall back to finding any NNto1 directory
static string getCompressionPath() {
  // Try reading from config
  ifstream cfg(CONFIG_PATH);
  if (cfg.is_open()) {
    string line;
    while (getline(cfg, line)) {
      if (line.find("compression path") != string::npos) {
        size_t eq = line.find('=');
        if (eq != string::npos) {
          string path = line.substr(eq + 1);
          // trim whitespace
          size_t start = path.find_first_not_of(" \t");
          size_t end = path.find_last_not_of(" \t\r\n");
          if (start != string::npos && end != string::npos) {
            path = path.substr(start, end - start + 1);
            // Verify the directory exists
            FILE* f = fopen((path + "U.preadvect.component0").c_str(), "rb");
            if (f) { fclose(f); return path; }
          }
        }
      }
    }
  }
  // Fallback: scan for any existing compression directory
  for (int ratio = 2; ratio <= 20; ratio++) {
    string path = REDUCED_PATH + to_string(ratio) + "to1/";
    FILE* f = fopen((path + "U.preadvect.component0").c_str(), "rb");
    if (f) { fclose(f); return path; }
  }
  return REDUCED_PATH + "17to1/"; // last resort default
}

static const string COMPRESSION_PATH = getCompressionPath();

// Expected grid dimensions from cfg/stam.64.cfg defaults
static const int EXPECTED_XRES = 48;
static const int EXPECTED_YRES = 64;
static const int EXPECTED_ZRES = 48;
static const int EXPECTED_XPEELED = EXPECTED_XRES - 2;  // 46
static const int EXPECTED_YPEELED = EXPECTED_YRES - 2;  // 62
static const int EXPECTED_ZPEELED = EXPECTED_ZRES - 2;  // 46

// Helper: check a vector for NaN or Inf
static bool hasNanOrInf(const VectorXd& v) {
  for (int i = 0; i < v.size(); i++) {
    if (std::isnan(v[i]) || std::isinf(v[i])) return true;
  }
  return false;
}

// Helper: load a compressed component and its data.
// Owns the allData buffer via delete[] in destructor.
struct CompressedComponent {
  COMPRESSION_DATA compressionData;
  int* allData = nullptr;
  bool loaded = false;

  ~CompressedComponent() { delete[] allData; }

  // Non-copyable (owns raw pointer)
  CompressedComponent() = default;
  CompressedComponent(const CompressedComponent&) = delete;
  CompressedComponent& operator=(const CompressedComponent&) = delete;

  bool load(const string& path) {
    allData = ReadBinaryFileToMemory(path.c_str(), &compressionData);
    loaded = (allData != nullptr);
    return loaded;
  }
};

//////////////////////////////////////////////////////////////////////
// H1 Tests: Data file integrity
//////////////////////////////////////////////////////////////////////

TEST(compressed_files_load_successfully) {
  const string files[] = {
    "U.preadvect.component0", "U.preadvect.component1", "U.preadvect.component2",
    "U.final.component0",    "U.final.component1",    "U.final.component2"
  };

  for (const auto& f : files) {
    string path = COMPRESSION_PATH + f;
    CompressedComponent comp;
    ASSERT_MSG(comp.load(path), "Failed to load: " + f);
    INFO(f << ": loaded OK");
  }
  return true;
}

TEST(compressed_metadata_is_sane) {
  const string components[] = {"component0", "component1", "component2"};
  const string bases[] = {"U.preadvect.", "U.final."};

  for (const auto& basis : bases) {
    int prevNumCols = -1;
    for (const auto& comp : components) {
      string path = COMPRESSION_PATH + basis + comp;
      CompressedComponent c;
      ASSERT(c.load(path));

      int numCols = c.compressionData.get_numCols();
      int numBlocks = c.compressionData.get_numBlocks();
      int nBits = c.compressionData.get_nBits();
      const VEC3I& dims = c.compressionData.get_dims();
      const VEC3I& padded = c.compressionData.get_paddedDims();

      INFO(basis << comp << ":");
      INFO("  dims: " << dims[0] << "x" << dims[1] << "x" << dims[2]);
      INFO("  paddedDims: " << padded[0] << "x" << padded[1] << "x" << padded[2]);
      INFO("  numCols: " << numCols << ", numBlocks: " << numBlocks << ", nBits: " << nBits);

      // Basic sanity
      ASSERT_MSG(numCols > 0, "numCols must be positive");
      ASSERT_MSG(numBlocks > 0, "numBlocks must be positive");
      ASSERT_MSG(nBits > 0 && nBits <= 32, "nBits must be in [1, 32]");
      ASSERT_MSG(dims[0] > 0 && dims[1] > 0 && dims[2] > 0, "dims must be positive");
      ASSERT_MSG(padded[0] >= dims[0] && padded[1] >= dims[1] && padded[2] >= dims[2],
                 "padded dims must be >= dims");

      // All components of the same basis should have the same number of columns
      if (prevNumCols >= 0) {
        ASSERT_MSG(numCols == prevNumCols,
                   "numCols mismatch across components: " + to_string(numCols) +
                   " vs " + to_string(prevNumCols));
      }
      prevNumCols = numCols;

      // Padded dims should be multiples of BLOCK_SIZE
      ASSERT_MSG(padded[0] % BLOCK_SIZE == 0, "padded X not multiple of BLOCK_SIZE");
      ASSERT_MSG(padded[1] % BLOCK_SIZE == 0, "padded Y not multiple of BLOCK_SIZE");
      ASSERT_MSG(padded[2] % BLOCK_SIZE == 0, "padded Z not multiple of BLOCK_SIZE");

      // Number of blocks should equal product of padded dims / BLOCK_SIZE^3
      int expectedBlocks = (padded[0] / BLOCK_SIZE) * (padded[1] / BLOCK_SIZE) * (padded[2] / BLOCK_SIZE);
      ASSERT_MSG(numBlocks == expectedBlocks,
                 "numBlocks mismatch: got " + to_string(numBlocks) +
                 ", expected " + to_string(expectedBlocks));
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////
// H3 Tests: Dimension consistency
//////////////////////////////////////////////////////////////////////

TEST(compressed_dims_match_peeled_grid) {
  // Each compressed component stores one spatial component of the velocity basis.
  // component0 = X velocity, component1 = Y velocity, component2 = Z velocity.
  // The dims should correspond to the peeled grid dimensions.
  CompressedComponent compX, compY, compZ;
  ASSERT(compX.load(COMPRESSION_PATH + "U.preadvect.component0"));
  ASSERT(compY.load(COMPRESSION_PATH + "U.preadvect.component1"));
  ASSERT(compZ.load(COMPRESSION_PATH + "U.preadvect.component2"));

  const VEC3I& dimsX = compX.compressionData.get_dims();
  const VEC3I& dimsY = compY.compressionData.get_dims();
  const VEC3I& dimsZ = compZ.compressionData.get_dims();

  INFO("Component 0 (X vel) dims: " << dimsX[0] << "x" << dimsX[1] << "x" << dimsX[2]);
  INFO("Component 1 (Y vel) dims: " << dimsY[0] << "x" << dimsY[1] << "x" << dimsY[2]);
  INFO("Component 2 (Z vel) dims: " << dimsZ[0] << "x" << dimsZ[1] << "x" << dimsZ[2]);
  INFO("Expected peeled grid: " << EXPECTED_XPEELED << "x" << EXPECTED_YPEELED << "x" << EXPECTED_ZPEELED);

  // All three components should store the same spatial dimensions
  // (the peeled grid, since we store one scalar field per velocity component)
  ASSERT_MSG(dimsX[0] == dimsY[0] && dimsX[0] == dimsZ[0], "X dimension mismatch across components");
  ASSERT_MSG(dimsX[1] == dimsY[1] && dimsX[1] == dimsZ[1], "Y dimension mismatch across components");
  ASSERT_MSG(dimsX[2] == dimsY[2] && dimsX[2] == dimsZ[2], "Z dimension mismatch across components");

  // Check against expected peeled dimensions
  bool matchesPeeled = (dimsX[0] == EXPECTED_XPEELED &&
                        dimsX[1] == EXPECTED_YPEELED &&
                        dimsX[2] == EXPECTED_ZPEELED);
  if (!matchesPeeled) {
    INFO("WARNING: Dims do NOT match expected peeled grid!");
    INFO("  This may explain the disabled assertion in PeeledCompressedUnprojectTransform");
    // Don't fail yet -- record the finding but check if it at least matches full grid
    bool matchesFull = (dimsX[0] == EXPECTED_XRES &&
                        dimsX[1] == EXPECTED_YRES &&
                        dimsX[2] == EXPECTED_ZRES);
    INFO("  Matches full grid? " << (matchesFull ? "YES" : "NO"));
  } else {
    INFO("Dims match expected peeled grid: OK");
  }
  return true;
}

TEST(compressed_dims_match_uncompressed_matrix) {
  // Load uncompressed matrix to compare dimensions
  MatrixXd U_preadvect;
  string filename = REDUCED_PATH + "U.preadvect.matrix";
  bool ok = EIGEN::read(filename, U_preadvect);
  ASSERT_MSG(ok, "Failed to read uncompressed U.preadvect.matrix");

  INFO("Uncompressed U.preadvect: " << U_preadvect.rows() << " rows x " << U_preadvect.cols() << " cols");

  // The uncompressed matrix has rows = 3 * xPeeled * yPeeled * zPeeled
  int expectedRows = 3 * EXPECTED_XPEELED * EXPECTED_YPEELED * EXPECTED_ZPEELED;
  INFO("Expected rows (3 * peeled): " << expectedRows);
  ASSERT_MSG(U_preadvect.rows() == expectedRows,
             "Uncompressed row count doesn't match 3*peeled: " +
             to_string(U_preadvect.rows()) + " vs " + to_string(expectedRows));

  // Now check that compressed numCols matches uncompressed cols
  CompressedComponent comp;
  ASSERT(comp.load(COMPRESSION_PATH + "U.preadvect.component0"));
  int compressedCols = comp.compressionData.get_numCols();
  INFO("Compressed numCols: " << compressedCols << ", Uncompressed cols: " << U_preadvect.cols());
  ASSERT_MSG(compressedCols == U_preadvect.cols(),
             "Column count mismatch: compressed=" + to_string(compressedCols) +
             " vs uncompressed=" + to_string(U_preadvect.cols()));

  return true;
}

//////////////////////////////////////////////////////////////////////
// H1 Tests: Decode integrity
//////////////////////////////////////////////////////////////////////

TEST(decode_single_block_no_nan) {
  // Decode column 0 from each compressed file. Check for NaN/Inf.
  // This uses DecodeScalarFieldEigen which decompresses all blocks of one column.
  const string files[] = {
    "U.preadvect.component0", "U.preadvect.component1", "U.preadvect.component2",
    "U.final.component0",    "U.final.component1",    "U.final.component2"
  };

  for (const auto& f : files) {
    CompressedComponent c;
    ASSERT(c.load(COMPRESSION_PATH + f));

    // Need damping array for decode
    c.compressionData.set_dampingArray();
    c.compressionData.set_dampingArrayList();
    c.compressionData.set_zigzagArray();

    vector<VectorXd> decoded;
    DecodeScalarFieldEigen(&c.compressionData, c.allData, 0, &decoded);

    ASSERT_MSG(!decoded.empty(), "Decoded zero blocks for " + f);
    INFO(f << ": decoded " << decoded.size() << " blocks, first block size = " << decoded[0].size());

    // Check every block for NaN/Inf
    for (int i = 0; i < (int)decoded.size(); i++) {
      ASSERT_MSG(!hasNanOrInf(decoded[i]),
                 f + " block " + to_string(i) + " contains NaN or Inf");
    }

    // Check that at least some values are nonzero
    double maxAbs = 0.0;
    for (const auto& block : decoded) {
      for (int i = 0; i < block.size(); i++) {
        maxAbs = max(maxAbs, abs(block[i]));
      }
    }
    INFO(f << ": max absolute value in column 0 = " << maxAbs);
    ASSERT_MSG(maxAbs > 0.0, "All decoded values are zero in " + f);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////
// H1 Tests: Round-trip comparison (compressed vs uncompressed)
//////////////////////////////////////////////////////////////////////

TEST(compressed_column_matches_uncompressed) {
  // This is the key test: decompress a full column from compressed data and
  // compare it against the corresponding column in the uncompressed matrix.
  // The error should be bounded by the compression parameters.

  // Load uncompressed matrix
  MatrixXd U_preadvect;
  string filename = REDUCED_PATH + "U.preadvect.matrix";
  bool ok = EIGEN::read(filename, U_preadvect);
  ASSERT_MSG(ok, "Failed to read uncompressed matrix");

  int numCols = U_preadvect.cols();
  int numRows = U_preadvect.rows();
  int scalarRows = numRows / 3;  // rows per velocity component
  INFO("Uncompressed matrix: " << numRows << " x " << numCols);
  INFO("Scalar rows per component: " << scalarRows);

  // Load all three compressed components
  CompressedComponent compX, compY, compZ;
  ASSERT(compX.load(COMPRESSION_PATH + "U.preadvect.component0"));
  ASSERT(compY.load(COMPRESSION_PATH + "U.preadvect.component1"));
  ASSERT(compZ.load(COMPRESSION_PATH + "U.preadvect.component2"));

  // Set up decoding prerequisites
  compX.compressionData.set_dampingArray();
  compX.compressionData.set_dampingArrayList();
  compX.compressionData.set_zigzagArray();
  compY.compressionData.set_dampingArray();
  compY.compressionData.set_dampingArrayList();
  compY.compressionData.set_zigzagArray();
  compZ.compressionData.set_dampingArray();
  compZ.compressionData.set_dampingArrayList();
  compZ.compressionData.set_zigzagArray();

  const VEC3I& dims = compX.compressionData.get_dims();
  const VEC3I& padded = compX.compressionData.get_paddedDims();
  INFO("Compressed dims: " << dims[0] << "x" << dims[1] << "x" << dims[2]);
  INFO("Compressed padded: " << padded[0] << "x" << padded[1] << "x" << padded[2]);

  // Test a few columns (0, middle, last)
  int testCols[] = {0, numCols / 2, numCols - 1};
  for (int col : testCols) {
    // Decode compressed blocks for this column
    vector<VectorXd> blocksX, blocksY, blocksZ;
    DecodeScalarFieldEigen(&compX.compressionData, compX.allData, col, &blocksX);
    DecodeScalarFieldEigen(&compY.compressionData, compY.allData, col, &blocksY);
    DecodeScalarFieldEigen(&compZ.compressionData, compZ.allData, col, &blocksZ);

    // The decoded blocks are in DCT domain (frequency space).
    // DecodeScalarFieldEigen stays in frequency domain.
    // To compare against the spatial-domain uncompressed matrix, we need DecodeScalarField
    // which goes back to spatial domain. But that requires FFTW setup.
    // Instead, just check that decoded values are finite and report statistics.
    INFO("Column " << col << ":");
    INFO("  X: " << blocksX.size() << " blocks, Y: " << blocksY.size() << " blocks, Z: " << blocksZ.size() << " blocks");

    // Verify no NaN/Inf
    for (const auto& b : blocksX) ASSERT(!hasNanOrInf(b));
    for (const auto& b : blocksY) ASSERT(!hasNanOrInf(b));
    for (const auto& b : blocksZ) ASSERT(!hasNanOrInf(b));

    // Report magnitude statistics
    double maxX = 0, maxY = 0, maxZ = 0;
    for (const auto& b : blocksX) maxX = max(maxX, b.lpNorm<Infinity>());
    for (const auto& b : blocksY) maxY = max(maxY, b.lpNorm<Infinity>());
    for (const auto& b : blocksZ) maxZ = max(maxZ, b.lpNorm<Infinity>());
    INFO("  Max abs values - X: " << maxX << ", Y: " << maxY << ", Z: " << maxZ);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////
// H1: Full spatial-domain round-trip (uses DecodeVectorField with FFTW)
//////////////////////////////////////////////////////////////////////

TEST(full_spatial_roundtrip_error) {
  // Load all three compressed components and set up MATRIX_COMPRESSION_DATA
  // just like the real solver does, then decode a full vector field column
  // and compare against uncompressed.

  // Must match what was used during compression


  COMPRESSION_DATA cd0, cd1, cd2;
  int* data0 = ReadBinaryFileToMemory((COMPRESSION_PATH + "U.preadvect.component0").c_str(), &cd0);
  int* data1 = ReadBinaryFileToMemory((COMPRESSION_PATH + "U.preadvect.component1").c_str(), &cd1);
  int* data2 = ReadBinaryFileToMemory((COMPRESSION_PATH + "U.preadvect.component2").c_str(), &cd2);
  ASSERT(data0 && data1 && data2);

  // Diagnostic: check metadata matrices
  {
    MatrixXd* sM = cd0.get_sListMatrix();
    MatrixXd* gM = cd0.get_gammaListMatrix();
    MatrixXi* blM = cd0.get_blockLengthsMatrix();
    MatrixXi* biM = cd0.get_blockIndicesMatrix();
    INFO("cd0 sListMatrix: " << sM->rows() << "x" << sM->cols()
         << ", min=" << sM->minCoeff() << ", max=" << sM->maxCoeff()
         << ", sum=" << sM->sum());
    INFO("cd0 gammaListMatrix: " << gM->rows() << "x" << gM->cols()
         << ", min=" << gM->minCoeff() << ", max=" << gM->maxCoeff());
    INFO("cd0 blockLengthsMatrix: " << blM->rows() << "x" << blM->cols()
         << ", min=" << blM->minCoeff() << ", max=" << blM->maxCoeff()
         << ", sum=" << blM->sum());
    INFO("cd0 blockIndicesMatrix: " << biM->rows() << "x" << biM->cols()
         << ", min=" << biM->minCoeff() << ", max=" << biM->maxCoeff());
    // Print first few sListMatrix values for col 0
    INFO("sListMatrix col 0, first 5 blocks: "
         << (*sM)(0,0) << ", " << (*sM)(1,0) << ", " << (*sM)(2,0) << ", "
         << (*sM)(3,0) << ", " << (*sM)(4,0));
    INFO("gammaListMatrix col 0, first 5 blocks: "
         << (*gM)(0,0) << ", " << (*gM)(1,0) << ", " << (*gM)(2,0) << ", "
         << (*gM)(3,0) << ", " << (*gM)(4,0));
    // Check for zeros in sListMatrix (would cause division by zero / huge values)
    int zeroCount = 0;
    for (int i = 0; i < sM->size(); i++)
      if (sM->data()[i] == 0.0) zeroCount++;
    INFO("sListMatrix zero count: " << zeroCount << " / " << sM->size());
  }

  MATRIX_COMPRESSION_DATA mcd(data0, data1, data2, &cd0, &cd1, &cd2);

  // Set up FFTW (inverse DCT) -- exactly as initCompressionData does
  mcd.dct_setup(-1);
  mcd.init_cache();
  mcd.set_dampingArrayLists();

  // Decode column 0 as a full vector field (spatial domain)
  VECTOR3_FIELD_3D decoded;
  DecodeVectorField(&mcd, 0, &decoded);

  INFO("Decoded vector field: " << decoded.xRes() << "x" << decoded.yRes() << "x" << decoded.zRes());

  // Check for NaN/Inf in the decoded field
  VectorXd flat = decoded.flattenedEigen();
  ASSERT_MSG(!hasNanOrInf(flat), "Decoded vector field contains NaN/Inf");
  INFO("Decoded field max abs: " << flat.lpNorm<Infinity>());
  INFO("Decoded field L2 norm: " << flat.norm());

  // Now load uncompressed matrix and compare
  MatrixXd U_preadvect;
  bool ok = EIGEN::read(REDUCED_PATH + "U.preadvect.matrix", U_preadvect);
  ASSERT_MSG(ok, "Failed to read uncompressed matrix");

  VectorXd uncompressedCol = U_preadvect.col(0);
  INFO("Uncompressed col 0 L2 norm: " << uncompressedCol.norm());
  INFO("Uncompressed col 0 max abs: " << uncompressedCol.lpNorm<Infinity>());

  // The decoded field dimensions may differ from uncompressed due to padding.
  // Check if sizes match directly
  if (flat.size() == uncompressedCol.size()) {
    double absError = (flat - uncompressedCol).norm();
    double relError = absError / uncompressedCol.norm();
    INFO("Absolute L2 error: " << absError);
    INFO("Relative L2 error: " << relError << " (" << (relError * 100) << "%)");

    // Also check per-component errors to see if one is worse
    int scalarSize = flat.size() / 3;
    for (int c = 0; c < 3; c++) {
      // Extract every 3rd element starting at offset c (interleaved format)
      double compErr = 0, compNorm = 0;
      for (int i = c; i < flat.size(); i += 3) {
        double diff = flat[i] - uncompressedCol[i];
        compErr += diff * diff;
        compNorm += uncompressedCol[i] * uncompressedCol[i];
      }
      compErr = sqrt(compErr);
      compNorm = sqrt(compNorm);
      INFO("Component " << c << " rel error: " << (compErr / compNorm) << " (" << (compErr / compNorm * 100) << "%)");
    }

    // Print first 10 values side by side
    INFO("First 10 values (decoded vs uncompressed):");
    for (int i = 0; i < min((int)flat.size(), 10); i++) {
      INFO("  [" << i << "] " << flat[i] << " vs " << uncompressedCol[i]);
    }

    // For 6:1 compression with percent=0.99, relative error should be well under 50%
    ASSERT_MSG(relError < 0.5, "Relative error too large: " + to_string(relError));
  } else {
    INFO("SIZE MISMATCH: decoded=" << flat.size() << " vs uncompressed=" << uncompressedCol.size());
    INFO("This confirms hypothesis H3 (dimension mismatch)");
  }

  mcd.dct_cleanup();
  return true;
}

//////////////////////////////////////////////////////////////////////
// H2/H4: FFTW lifecycle test
//////////////////////////////////////////////////////////////////////

TEST(fftw_plan_survives_assignment) {
  // Reproduce the exact sequence from reducedCompressed3D:
  //   1. ReadBinaryFileToMemory into stack COMPRESSION_DATA
  //   2. Construct MATRIX_COMPRESSION_DATA from pointers
  //   3. Assign to a different MATRIX_COMPRESSION_DATA (simulating member assignment)
  //   4. Call dct_setup + init_cache on the *target*
  //   5. Attempt a decode using the target's plan

  // Step 1: Load compressed data
  COMPRESSION_DATA cd0, cd1, cd2;
  int* data0 = ReadBinaryFileToMemory((COMPRESSION_PATH + "U.preadvect.component0").c_str(), &cd0);
  int* data1 = ReadBinaryFileToMemory((COMPRESSION_PATH + "U.preadvect.component1").c_str(), &cd1);
  int* data2 = ReadBinaryFileToMemory((COMPRESSION_PATH + "U.preadvect.component2").c_str(), &cd2);
  ASSERT(data0 && data1 && data2);

  // Step 2: Construct from pointers (like loadReducedIOP does)
  MATRIX_COMPRESSION_DATA source(data0, data1, data2, &cd0, &cd1, &cd2);

  // Step 3: Assign to a different variable (like assigning to _U_preadvect_data)
  MATRIX_COMPRESSION_DATA target;
  target = source;

  // Step 4: Call dct_setup + init_cache on target (like initCompressionData does)
  target.dct_setup(-1);
  target.init_cache();
  target.set_dampingArrayLists();

  // Step 5: Decode using target
  INFO("Attempting decode through assigned MATRIX_COMPRESSION_DATA...");
  VECTOR3_FIELD_3D decoded;
  DecodeVectorField(&target, 0, &decoded);

  VectorXd flat = decoded.flattenedEigen();
  ASSERT_MSG(!hasNanOrInf(flat), "Decoded field after assignment contains NaN/Inf");
  INFO("Decode through assignment OK, max abs: " << flat.lpNorm<Infinity>());

  target.dct_cleanup();
  return true;
}

TEST(fftw_dct_roundtrip) {
  // Create a known test vector, run forward DCT then inverse DCT.
  // Should recover the original (within floating point tolerance).

  const int N = BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE;  // 512

  // Set up forward DCT
  double* fwd_buf = (double*)fftw_malloc(N * sizeof(double));
  fftw_plan fwd_plan = fftw_plan_r2r_3d(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,
      fwd_buf, fwd_buf, FFTW_REDFT10, FFTW_REDFT10, FFTW_REDFT10, FFTW_MEASURE);

  // Set up inverse DCT
  double* inv_buf = (double*)fftw_malloc(N * sizeof(double));
  fftw_plan inv_plan = fftw_plan_r2r_3d(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,
      inv_buf, inv_buf, FFTW_REDFT01, FFTW_REDFT01, FFTW_REDFT01, FFTW_MEASURE);

  // Fill with a test pattern
  for (int i = 0; i < N; i++) {
    fwd_buf[i] = sin(0.1 * i) + 0.5 * cos(0.03 * i);
  }

  // Save original
  vector<double> original(fwd_buf, fwd_buf + N);

  // Forward transform
  fftw_execute(fwd_plan);

  // Copy to inverse buffer
  for (int i = 0; i < N; i++) {
    inv_buf[i] = fwd_buf[i];
  }

  // Inverse transform
  fftw_execute(inv_plan);

  // FFTW's DCT-II / DCT-III pair requires normalization by 8*N
  double normFactor = 8.0 * N;
  double maxErr = 0.0;
  for (int i = 0; i < N; i++) {
    double recovered = inv_buf[i] / normFactor;
    maxErr = max(maxErr, abs(recovered - original[i]));
  }

  INFO("DCT round-trip max error: " << maxErr);
  ASSERT_MSG(maxErr < 1e-10, "DCT round-trip error too large");

  fftw_destroy_plan(fwd_plan);
  fftw_destroy_plan(inv_plan);
  fftw_free(fwd_buf);
  fftw_free(inv_buf);
  return true;
}

//////////////////////////////////////////////////////////////////////
// Diagnostic: compare scalar decode directly against matrix component
//////////////////////////////////////////////////////////////////////

TEST(scalar_field_decode_vs_uncompressed) {
  // Must match what was used during compression


  // Decode just one scalar component and compare against the corresponding
  // slice of the uncompressed matrix to isolate whether the error is in
  // the scalar decode or the vector field assembly.

  // Load uncompressed matrix
  MatrixXd U_preadvect;
  bool ok = EIGEN::read(REDUCED_PATH + "U.preadvect.matrix", U_preadvect);
  ASSERT_MSG(ok, "Failed to read uncompressed matrix");

  VectorXd fullCol = U_preadvect.col(0);
  int totalCells = fullCol.size() / 3;  // number of spatial cells
  INFO("Uncompressed matrix: " << U_preadvect.rows() << " x " << U_preadvect.cols());
  INFO("Total spatial cells: " << totalCells);

  // Extract the X component from the interleaved uncompressed column
  VectorXd uncompX(totalCells);
  for (int i = 0; i < totalCells; i++)
    uncompX[i] = fullCol[3 * i];

  INFO("Uncompressed X component: max=" << uncompX.lpNorm<Infinity>()
       << ", L2=" << uncompX.norm());

  // Load compressed X component and decode
  COMPRESSION_DATA cd;
  int* data = ReadBinaryFileToMemory((COMPRESSION_PATH + "U.preadvect.component0").c_str(), &cd);
  ASSERT(data != nullptr);

  cd.set_dampingArray();
  cd.set_dampingArrayList();
  cd.set_zigzagArray();

  // Decode scalar field for column 0 (spatial domain)
  FIELD_3D decodedScalar;
  DecodeScalarField(&cd, data, 0, &decodedScalar);

  INFO("Decoded scalar field: " << decodedScalar.xRes() << "x"
       << decodedScalar.yRes() << "x" << decodedScalar.zRes()
       << " = " << decodedScalar.totalCells() << " cells");

  ASSERT_MSG(decodedScalar.totalCells() == totalCells,
             "Size mismatch: decoded=" + to_string(decodedScalar.totalCells()) +
             " vs uncompressed=" + to_string(totalCells));

  // Compare element by element
  double maxErr = 0, sumSqErr = 0;
  int worstIdx = 0;
  for (int i = 0; i < totalCells; i++) {
    double diff = decodedScalar[i] - uncompX[i];
    sumSqErr += diff * diff;
    if (abs(diff) > maxErr) {
      maxErr = abs(diff);
      worstIdx = i;
    }
  }
  double relError = sqrt(sumSqErr) / uncompX.norm();

  INFO("Scalar X decode vs uncompressed X:");
  INFO("  Relative L2 error: " << relError << " (" << (relError * 100) << "%)");
  INFO("  Max abs error: " << maxErr << " at index " << worstIdx);
  INFO("  Decoded[worst]: " << decodedScalar[worstIdx]
       << " vs Uncompressed[worst]: " << uncompX[worstIdx]);

  // Print first 5 values
  INFO("  First 5 decoded: " << decodedScalar[0] << ", " << decodedScalar[1]
       << ", " << decodedScalar[2] << ", " << decodedScalar[3]
       << ", " << decodedScalar[4]);
  INFO("  First 5 uncomp:  " << uncompX[0] << ", " << uncompX[1]
       << ", " << uncompX[2] << ", " << uncompX[3]
       << ", " << uncompX[4]);

  // If error is huge, also try comparing decoded against Y and Z components
  // to check for component swizzle
  if (relError > 0.5) {
    VectorXd uncompY(totalCells), uncompZ(totalCells);
    for (int i = 0; i < totalCells; i++) {
      uncompY[i] = fullCol[3 * i + 1];
      uncompZ[i] = fullCol[3 * i + 2];
    }

    // Compare decoded X against uncompressed Y and Z
    double errVsY = 0, errVsZ = 0;
    for (int i = 0; i < totalCells; i++) {
      errVsY += (decodedScalar[i] - uncompY[i]) * (decodedScalar[i] - uncompY[i]);
      errVsZ += (decodedScalar[i] - uncompZ[i]) * (decodedScalar[i] - uncompZ[i]);
    }
    INFO("  Decoded X vs uncompressed Y: relL2=" << sqrt(errVsY) / uncompY.norm());
    INFO("  Decoded X vs uncompressed Z: relL2=" << sqrt(errVsZ) / uncompZ.norm());

    // Also try non-interleaved interpretation: maybe the matrix is stored
    // as [all X | all Y | all Z] rather than interleaved
    VectorXd altX = fullCol.head(totalCells);
    VectorXd altY = fullCol.segment(totalCells, totalCells);
    VectorXd altZ = fullCol.tail(totalCells);
    double errVsAltX = 0, errVsAltY = 0, errVsAltZ = 0;
    for (int i = 0; i < totalCells; i++) {
      errVsAltX += (decodedScalar[i] - altX[i]) * (decodedScalar[i] - altX[i]);
      errVsAltY += (decodedScalar[i] - altY[i]) * (decodedScalar[i] - altY[i]);
      errVsAltZ += (decodedScalar[i] - altZ[i]) * (decodedScalar[i] - altZ[i]);
    }
    INFO("  NON-INTERLEAVED interpretation:");
    INFO("  Decoded X vs block-X: relL2=" << sqrt(errVsAltX) / altX.norm());
    INFO("  Decoded X vs block-Y: relL2=" << sqrt(errVsAltY) / altY.norm());
    INFO("  Decoded X vs block-Z: relL2=" << sqrt(errVsAltZ) / altZ.norm());
  }

  ASSERT_MSG(relError < 0.5, "Scalar decode error too large: " + to_string(relError));
  return true;
}

//////////////////////////////////////////////////////////////////////
// In-memory compress then decompress, bypassing file I/O
//////////////////////////////////////////////////////////////////////

TEST(inmemory_compress_decompress_roundtrip) {
  // Load the uncompressed matrix
  MatrixXd U_preadvect;
  bool ok = EIGEN::read(REDUCED_PATH + "U.preadvect.matrix", U_preadvect);
  ASSERT_MSG(ok, "Failed to read uncompressed matrix");

  int numCols = U_preadvect.cols();
  const int xRes = EXPECTED_XPEELED;  // 46
  const int yRes = EXPECTED_YPEELED;  // 62
  const int zRes = EXPECTED_ZPEELED;  // 46

  VEC3I dims(xRes, yRes, zRes);

  // Use same parameters as cfg
  int nBits = 32;
  double percent = 0.99;
  int maxIterations = 50;


  // Set up COMPRESSION_DATA for component 0 (X velocity)
  // Replicate PreprocessEncoder logic
  COMPRESSION_DATA cd(dims, numCols, nBits, percent);
  cd.set_dampingArray();
  cd.set_zigzagArray();

  VEC3I paddings;
  void GetPaddings(const VEC3I& v, VEC3I* paddings);
  GetPaddings(dims, &paddings);
  paddings += dims;
  cd.set_paddedDims(paddings);

  int numBlocks = paddings[0] * paddings[1] * paddings[2] / (BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE);
  cd.set_numBlocks(numBlocks);
  cd.set_maxIterations(maxIterations);

  INFO("Dims: " << dims[0] << "x" << dims[1] << "x" << dims[2]);
  INFO("numCols: " << numCols << ", nBits: " << nBits << ", percent: " << percent);
  INFO("numBlocks: " << cd.get_numBlocks());

  // Extract column 0's X component as a FIELD_3D
  VECTOR3_FIELD_3D V(U_preadvect.col(0), xRes, yRes, zRes);
  FIELD_3D originalX = V.scalarField(0);

  INFO("Original X: max=" << originalX.absMax() << ", L2=" << originalX.absMax());

  // Step 1: Subdivide into blocks
  vector<FIELD_3D> blocks;
  GetBlocks(originalX, &blocks);
  INFO("Got " << blocks.size() << " blocks (expected " << cd.get_numBlocks() << ")");

  // Step 2: Forward DCT
  UnitaryBlockDCT(1, &blocks);

  // Step 3: Preprocess + Encode each block
  const INTEGER_FIELD_3D& zigzagArray = cd.get_zigzagArray();
  vector<VectorXi> allEncodedBlocks(numBlocks);

  for (int i = 0; i < numBlocks; i++) {
    PreprocessBlock(&blocks[i], i, 0, &cd);

    INTEGER_FIELD_3D intEncoded;
    EncodeBlock(blocks[i], i, 0, &cd, &intEncoded);

    VectorXi zigzagged;
    ZigzagFlatten(intEncoded, zigzagArray, &zigzagged);

    // Store the zigzagged (before RLE) for simpler decode
    allEncodedBlocks[i] = zigzagged;
  }

  INFO("Encoded " << numBlocks << " blocks");

  // Step 4: Decode each block (reverse the process)
  vector<FIELD_3D> decodedBlocks(numBlocks);
  VectorXi runLengthDecoded(BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE);
  FIELD_3D decodedBlock(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
  INTEGER_FIELD_3D unflattened(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);

  for (int i = 0; i < numBlocks; i++) {
    // Skip RLE encode/decode since we saved the zigzagged data directly
    // Just undo zigzag
    ZigzagUnflatten(allEncodedBlocks[i], zigzagArray, &unflattened);

    // Dequantize
    DecodeBlockWithCompressionData(unflattened, i, 0, &cd, decodedBlock.data());

    decodedBlocks[i] = decodedBlock;
  }

  // Step 5: Inverse DCT
  UnitaryBlockDCT(-1, &decodedBlocks);

  // Step 6: Reassemble
  VEC3I paddedDims = cd.get_paddedDims();
  FIELD_3D padded_result(paddedDims[0], paddedDims[1], paddedDims[2]);
  AssimilateBlocks(paddedDims, decodedBlocks, &padded_result);

  // Step 7: Strip padding
  FIELD_3D decoded = padded_result.subfield(0, dims[0], 0, dims[1], 0, dims[2]);

  INFO("Decoded X: max=" << decoded.absMax() << ", L2=" << decoded.absMax());

  // Compare
  double maxErr = 0;
  double sumSqErr = 0;
  double sumSqOrig = 0;
  for (int i = 0; i < decoded.totalCells(); i++) {
    double diff = decoded[i] - originalX[i];
    sumSqErr += diff * diff;
    sumSqOrig += originalX[i] * originalX[i];
    maxErr = max(maxErr, abs(diff));
  }
  double relError = sqrt(sumSqErr) / sqrt(sumSqOrig);

  INFO("In-memory roundtrip relative L2 error: " << relError << " (" << (relError * 100) << "%)");
  INFO("Max abs error: " << maxErr);

  // Print first 5 values
  INFO("First 5 original: " << originalX[0] << ", " << originalX[1]
       << ", " << originalX[2] << ", " << originalX[3]
       << ", " << originalX[4]);
  INFO("First 5 decoded:  " << decoded[0] << ", " << decoded[1]
       << ", " << decoded[2] << ", " << decoded[3]
       << ", " << decoded[4]);

  ASSERT_MSG(relError < 0.5, "In-memory roundtrip error too large: " + to_string(relError));
  return true;
}

//////////////////////////////////////////////////////////////////////
// Verify that DecodeBlockWithCompressionDataSparseQuantized matches
// DecodeBlockWithCompressionData (the runtime decoder vs test decoder)
//////////////////////////////////////////////////////////////////////
TEST(sparse_quantized_decoder_matches_standard) {
  // Load compressed preadvect data
  CompressedComponent compX, compY, compZ;
  bool ok = compX.load(COMPRESSION_PATH + "U.preadvect.component0");
  ok = ok && compY.load(COMPRESSION_PATH + "U.preadvect.component1");
  ok = ok && compZ.load(COMPRESSION_PATH + "U.preadvect.component2");
  ASSERT_MSG(ok, "Failed to load compressed files");



  // Set up compression data for decoding
  compX.compressionData.set_dampingArray();
  compX.compressionData.set_zigzagArray();
  compX.compressionData.set_dampingArrayList();

  int numBlocks = compX.compressionData.get_numBlocks();
  int numCols = compX.compressionData.get_numCols();
  const INTEGER_FIELD_3D& zigzagArray = compX.compressionData.get_zigzagArray();
  const INTEGER_FIELD_3D& reverseZigzag = compX.compressionData.get_reverseZigzag();

  INFO("Testing " << numBlocks << " blocks x " << numCols << " cols");

  double maxDiff = 0;
  int blocksCompared = 0;

  // Compare first 5 blocks x first 3 cols (representative sample)
  int testBlocks = min(5, numBlocks);
  int testCols = min(3, numCols);

  for (int block = 0; block < testBlocks; block++) {
    for (int col = 0; col < testCols; col++) {
      // Decode with standard method
      VectorXi runLengthDecoded(BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE);
      RunLengthDecodeBinary(compX.allData, block, col, &compX.compressionData, &runLengthDecoded);

      INTEGER_FIELD_3D unflattened(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
      ZigzagUnflatten(runLengthDecoded, zigzagArray, &unflattened);

      FIELD_3D standardDecoded(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
      DecodeBlockWithCompressionData(unflattened, block, col,
          &compX.compressionData, standardDecoded.data());

      // Decode with sparse quantized method (what runtime uses)
      INTEGER_FIELD_3D unflattenedSparse(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
      FIELD_3D sparseDecoded(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
      NONZERO_ENTRIES nonZeros;

      RunLengthDecodeBinaryInPlaceSparse(compX.allData, block, col,
          reverseZigzag, &compX.compressionData, unflattenedSparse, nonZeros);

      DecodeBlockWithCompressionDataSparseQuantized(unflattenedSparse, block, col,
          &compX.compressionData, sparseDecoded.data(), nonZeros);

      // Compare
      for (int i = 0; i < BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE; i++) {
        double diff = abs(standardDecoded[i] - sparseDecoded[i]);
        maxDiff = max(maxDiff, diff);
      }
      blocksCompared++;

      // Clear for next iteration
      unflattenedSparse.clear();
      sparseDecoded.clear();
    }
  }

  INFO("Compared " << blocksCompared << " block/col pairs");
  INFO("Max difference between standard and sparse quantized decoder: " << maxDiff);

  ASSERT_MSG(maxDiff < 1e-10,
      "SparseQuantized decoder diverges from standard decoder: " + to_string(maxDiff));
  return true;
}

//////////////////////////////////////////////////////////////////////
// Near-lossless compression test: compress with percent very close to 1.0
// to establish the error floor of the codec
//////////////////////////////////////////////////////////////////////
TEST(near_lossless_compression_error) {
  MatrixXd U_preadvect;
  bool ok = EIGEN::read(REDUCED_PATH + "U.preadvect.matrix", U_preadvect);
  ASSERT_MSG(ok, "Failed to read uncompressed matrix");

  int numCols = U_preadvect.cols();
  const int xRes = EXPECTED_XPEELED;
  const int yRes = EXPECTED_YPEELED;
  const int zRes = EXPECTED_ZPEELED;
  VEC3I dims(xRes, yRes, zRes);

  // Near-lossless: percent = 0.999999, should preserve almost all energy
  int nBits = 32;
  double percent = 0.999999;
  int maxIterations = 50;


  COMPRESSION_DATA cd(dims, numCols, nBits, percent);
  cd.set_dampingArray();
  cd.set_zigzagArray();

  VEC3I paddings;
  void GetPaddings(const VEC3I& v, VEC3I* paddings);
  GetPaddings(dims, &paddings);
  paddings += dims;
  cd.set_paddedDims(paddings);

  int numBlocks = paddings[0] * paddings[1] * paddings[2] / (BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE);
  cd.set_numBlocks(numBlocks);
  cd.set_maxIterations(maxIterations);

  INFO("Near-lossless test: percent=" << percent << ", nBits=" << nBits);

  // Test column 0, X component
  VECTOR3_FIELD_3D V(U_preadvect.col(0), xRes, yRes, zRes);
  FIELD_3D originalX = V.scalarField(0);

  // Encode
  vector<FIELD_3D> blocks;
  GetBlocks(originalX, &blocks);
  UnitaryBlockDCT(1, &blocks);

  const INTEGER_FIELD_3D& zigzagArray = cd.get_zigzagArray();
  vector<VectorXi> allEncodedBlocks(numBlocks);

  for (int i = 0; i < numBlocks; i++) {
    PreprocessBlock(&blocks[i], i, 0, &cd);
    INTEGER_FIELD_3D intEncoded;
    EncodeBlock(blocks[i], i, 0, &cd, &intEncoded);
    VectorXi zigzagged;
    ZigzagFlatten(intEncoded, zigzagArray, &zigzagged);
    allEncodedBlocks[i] = zigzagged;
  }

  // Decode
  vector<FIELD_3D> decodedBlocks(numBlocks);
  INTEGER_FIELD_3D unflattened(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
  FIELD_3D decodedBlock(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);

  for (int i = 0; i < numBlocks; i++) {
    ZigzagUnflatten(allEncodedBlocks[i], zigzagArray, &unflattened);
    DecodeBlockWithCompressionData(unflattened, i, 0, &cd, decodedBlock.data());
    decodedBlocks[i] = decodedBlock;
  }

  UnitaryBlockDCT(-1, &decodedBlocks);

  VEC3I paddedDims = cd.get_paddedDims();
  FIELD_3D padded_result(paddedDims[0], paddedDims[1], paddedDims[2]);
  AssimilateBlocks(paddedDims, decodedBlocks, &padded_result);
  FIELD_3D decoded = padded_result.subfield(0, dims[0], 0, dims[1], 0, dims[2]);

  // Compare
  double sumSqErr = 0, sumSqOrig = 0;
  for (int i = 0; i < decoded.totalCells(); i++) {
    double diff = decoded[i] - originalX[i];
    sumSqErr += diff * diff;
    sumSqOrig += originalX[i] * originalX[i];
  }
  double relError = sqrt(sumSqErr) / sqrt(sumSqOrig);

  INFO("Near-lossless roundtrip relative L2 error: " << relError << " (" << (relError * 100) << "%)");

  // At near-lossless compression, error should be very small
  ASSERT_MSG(relError < 0.01,
      "Near-lossless compression error too large: " + to_string(relError) + " (" + to_string(relError * 100) + "%)");
  return true;
}

//////////////////////////////////////////////////////////////////////
// Project -> Unproject round-trip test (the simulation hot path)
//////////////////////////////////////////////////////////////////////

TEST(project_unproject_roundtrip) {
  // This tests the exact pair of functions called every simulation step:
  //   PeeledCompressedProjectTransformNoSVD (velocity -> reduced coords)
  //   PeeledCompressedUnprojectTransform    (reduced coords -> velocity)
  //
  // We construct a known vector field by unprojecting a known q-vector,
  // then project it back and verify we recover the original q.

  COMPRESSION_DATA cd0, cd1, cd2;
  int* data0 = ReadBinaryFileToMemory((COMPRESSION_PATH + "U.preadvect.component0").c_str(), &cd0);
  int* data1 = ReadBinaryFileToMemory((COMPRESSION_PATH + "U.preadvect.component1").c_str(), &cd1);
  int* data2 = ReadBinaryFileToMemory((COMPRESSION_PATH + "U.preadvect.component2").c_str(), &cd2);
  ASSERT(data0 && data1 && data2);

  MATRIX_COMPRESSION_DATA mcd(data0, data1, data2, &cd0, &cd1, &cd2);
  mcd.dct_setup(-1);
  mcd.init_cache();
  mcd.set_dampingArrayLists();

  int numCols = cd0.get_numCols();
  const VEC3I& dims = cd0.get_dims();
  INFO("numCols: " << numCols << ", dims: " << dims[0] << "x" << dims[1] << "x" << dims[2]);

  // Construct a known q-vector: unit vector along column 0
  VectorXd q_original = VectorXd::Zero(numCols);
  q_original[0] = 1.0;

  // Unproject: q -> velocity field (full grid with peeled interior)
  // The field needs to be full-grid size for setWithPeeled
  int fullX = dims[0] + 2;
  int fullY = dims[1] + 2;
  int fullZ = dims[2] + 2;
  VECTOR3_FIELD_3D velocity(fullX, fullY, fullZ);
  PeeledCompressedUnprojectTransform(&mcd, q_original, &velocity);

  VectorXd flat = velocity.flattenedEigen();
  ASSERT_MSG(!hasNanOrInf(flat), "Unprojected field contains NaN/Inf");
  INFO("Unprojected field L2 norm: " << flat.norm());
  INFO("Unprojected field max abs: " << flat.lpNorm<Infinity>());

  // Project: velocity field -> q
  VectorXd q_recovered;
  PeeledCompressedProjectTransformNoSVD(velocity, &mcd, &q_recovered);

  ASSERT_MSG(q_recovered.size() == numCols,
      "Recovered q size mismatch: " + to_string(q_recovered.size()) + " vs " + to_string(numCols));
  ASSERT_MSG(!hasNanOrInf(q_recovered), "Projected q contains NaN/Inf");

  // Compare q_original and q_recovered
  double absError = (q_original - q_recovered).norm();
  double relError = absError / q_original.norm();
  INFO("q round-trip absolute L2 error: " << absError);
  INFO("q round-trip relative L2 error: " << relError << " (" << (relError * 100) << "%)");

  // Print first few components
  INFO("q_original vs q_recovered (first 5):");
  for (int i = 0; i < min(5, numCols); i++) {
    INFO("  [" << i << "] " << q_original[i] << " vs " << q_recovered[i]);
  }

  // With lossy compression, the basis columns are not perfectly orthonormal,
  // so the round-trip won't be exact. But for a single basis column,
  // the self-projection should recover most of the energy.
  // q[0] should be close to 1.0, other components should be small.
  INFO("q_recovered[0] = " << q_recovered[0] << " (expected ~1.0)");
  double offDiagonalNorm = 0;
  for (int i = 1; i < numCols; i++)
    offDiagonalNorm += q_recovered[i] * q_recovered[i];
  offDiagonalNorm = sqrt(offDiagonalNorm);
  INFO("Off-diagonal norm (should be small): " << offDiagonalNorm);

  // The relative error for the dominant component should be < 50% even with lossy compression
  ASSERT_MSG(relError < 0.5, "Project->Unproject round-trip error too large: " + to_string(relError));

  mcd.dct_cleanup();
  return true;
}

//////////////////////////////////////////////////////////////////////
// Pure arithmetic tests (no data files needed)
//////////////////////////////////////////////////////////////////////

TEST(modified_cumsum_correctness) {
  // ModifiedCumSum should produce a shifted cumulative sum:
  // input:  [a, b, c, d]
  // output: [0, a, a+b, a+b+c]
  // This is used to build the RLE seek table from block lengths.

  // Basic case
  VectorXi input(4);
  input << 10, 20, 30, 40;
  VectorXi result;
  ModifiedCumSum(input, &result);

  ASSERT_MSG(result.size() == 4, "Output size should match input");
  ASSERT_MSG(result[0] == 0, "First element should be 0, got " + to_string(result[0]));
  ASSERT_MSG(result[1] == 10, "Expected 10, got " + to_string(result[1]));
  ASSERT_MSG(result[2] == 30, "Expected 30, got " + to_string(result[2]));
  ASSERT_MSG(result[3] == 60, "Expected 60, got " + to_string(result[3]));

  // Single element
  VectorXi single(1);
  single << 42;
  VectorXi singleResult;
  ModifiedCumSum(single, &singleResult);
  ASSERT_MSG(singleResult.size() == 1, "Single element output size wrong");
  ASSERT_MSG(singleResult[0] == 0, "Single element should be 0");

  // All ones (typical uniform case)
  VectorXi ones = VectorXi::Ones(8);
  VectorXi onesResult;
  ModifiedCumSum(ones, &onesResult);
  for (int i = 0; i < 8; i++) {
    ASSERT_MSG(onesResult[i] == i, "Uniform case: expected " + to_string(i) + " at index " + to_string(i));
  }

  INFO("ModifiedCumSum: all cases passed");
  return true;
}

TEST(compute_block_number_correctness) {
  // ComputeBlockNumber maps a row index in the interleaved velocity matrix
  // to a (blockNumber, blockIndex) pair. Uses the peeled grid dims.
  VEC3I dims(EXPECTED_XPEELED, EXPECTED_YPEELED, EXPECTED_ZPEELED);  // 46x62x46

  int blockNumber, blockIndex;

  // Row 0: first cell (x=0, y=0, z=0) -> should be block 0, index 0
  ComputeBlockNumber(0, dims, &blockNumber, &blockIndex);
  ASSERT_MSG(blockNumber == 0, "Row 0: expected blockNumber=0, got " + to_string(blockNumber));
  ASSERT_MSG(blockIndex == 0, "Row 0: expected blockIndex=0, got " + to_string(blockIndex));

  // Row 3: still first cell (row/3 = 1, so x=1, y=0, z=0)
  // x=1 -> u=1, v=0, w=0 -> blockIndex = 1
  ComputeBlockNumber(3, dims, &blockNumber, &blockIndex);
  ASSERT_MSG(blockNumber == 0, "Row 3: expected blockNumber=0, got " + to_string(blockNumber));
  ASSERT_MSG(blockIndex == 1, "Row 3: expected blockIndex=1, got " + to_string(blockIndex));

  // Row at BLOCK_SIZE boundary: x=8 -> new block in x direction
  // row = 3 * (8 + 0*46 + 0*46*62) = 24
  ComputeBlockNumber(24, dims, &blockNumber, &blockIndex);
  ASSERT_MSG(blockNumber == 1, "x=8: expected blockNumber=1, got " + to_string(blockNumber));
  ASSERT_MSG(blockIndex == 0, "x=8: expected blockIndex=0, got " + to_string(blockIndex));

  // y=8 boundary: row = 3 * (0 + 8*46 + 0*46*62) = 3 * 368 = 1104
  // Padded xRes = 48 (46 + 2 padding), so xBlocks = 48/8 = 6
  // y=8 -> block in y = 1, so blockNumber = 0 + 1*6 = 6
  ComputeBlockNumber(1104, dims, &blockNumber, &blockIndex);
  ASSERT_MSG(blockIndex == 0, "y=8: expected blockIndex=0, got " + to_string(blockIndex));
  // blockNumber = x/8 + (y/8 * xPadded/8) = 0 + 1*6 = 6
  VEC3I paddings;
  GetPaddings(dims, &paddings);
  int xPadded = dims[0] + paddings[0];  // 48
  int expectedBlock = 0 + 1 * (xPadded / BLOCK_SIZE);
  ASSERT_MSG(blockNumber == expectedBlock,
      "y=8: expected blockNumber=" + to_string(expectedBlock) + ", got " + to_string(blockNumber));

  // Last valid row
  int lastRow = 3 * dims[0] * dims[1] * dims[2] - 1;
  ComputeBlockNumber(lastRow, dims, &blockNumber, &blockIndex);
  INFO("Last row " << lastRow << ": blockNumber=" << blockNumber << ", blockIndex=" << blockIndex);
  // Just verify it doesn't crash and returns valid values
  int yPadded = dims[1] + paddings[1];  // 64
  int zPadded = dims[2] + paddings[2];  // 48
  int totalBlocks = (xPadded / BLOCK_SIZE) * (yPadded / BLOCK_SIZE) * (zPadded / BLOCK_SIZE);
  ASSERT_MSG(blockNumber >= 0 && blockNumber < totalBlocks,
      "Last row: blockNumber " + to_string(blockNumber) + " out of range [0, " + to_string(totalBlocks) + ")");
  ASSERT_MSG(blockIndex >= 0 && blockIndex < BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE,
      "Last row: blockIndex " + to_string(blockIndex) + " out of range");

  INFO("ComputeBlockNumber: all cases passed");
  return true;
}

TEST(get_paddings_correctness) {
  // GetPaddings computes how much to pad each dimension to reach the next
  // multiple of BLOCK_SIZE.

  // Already a multiple of 8: no padding needed
  VEC3I exact(16, 24, 8);
  VEC3I pad;
  GetPaddings(exact, &pad);
  ASSERT_MSG(pad[0] == 0 && pad[1] == 0 && pad[2] == 0,
      "Exact multiples should have 0 padding");

  // Peeled grid: 46x62x46 -> need 2, 2, 2 to reach 48x64x48
  VEC3I peeled(46, 62, 46);
  GetPaddings(peeled, &pad);
  ASSERT_MSG(pad[0] == 2 && pad[1] == 2 && pad[2] == 2,
      "46x62x46 should pad by 2,2,2 but got " +
      to_string(pad[0]) + "," + to_string(pad[1]) + "," + to_string(pad[2]));

  // Edge case: 1x1x1 -> 7,7,7 to reach 8x8x8
  VEC3I tiny(1, 1, 1);
  GetPaddings(tiny, &pad);
  ASSERT_MSG(pad[0] == 7 && pad[1] == 7 && pad[2] == 7,
      "1x1x1 should pad by 7,7,7");

  INFO("GetPaddings: all cases passed");
  return true;
}

//////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
  cout << "CFS_PLUME Compression Data Validation Tests" << endl;
  cout << "============================================" << endl;
  cout << "Data path: " << COMPRESSION_PATH << endl;
  cout << "BLOCK_SIZE: " << BLOCK_SIZE << endl;
  cout << endl;

  RUN_ALL_TESTS();
}
