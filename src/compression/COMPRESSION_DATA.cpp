#include "COMPRESSION_DATA.h"
#include <iostream>

COMPRESSION_DATA::COMPRESSION_DATA() {}

COMPRESSION_DATA::COMPRESSION_DATA(const VEC3I& dims, int numCols, int nBits, double percent) :
  _dims(dims), _numCols(numCols), _nBits(nBits), _percent(percent)
{
  // Initialize matrices - using proper Eigen initialization
  _sListMatrix = MatrixXd();
  _gammaListMatrix = MatrixXd();
  _blockLengthsMatrix = MatrixXi();
  _blockIndicesMatrix = MatrixXi();

  // Initialize FFTW plans
  _dct_in = (double*)fftw_malloc(BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE * sizeof(double));
  _dct_out = (double*)fftw_malloc(BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE * sizeof(double));
  
  if (!_dct_in || !_dct_out) {
    exit(1);
  }

  // Create FFTW plan
  _dct_plan = fftw_plan_r2r_3d(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,
                               _dct_in, _dct_out,
                               FFTW_REDFT10, FFTW_REDFT10, FFTW_REDFT10,
                               FFTW_ESTIMATE);
  
  if (!_dct_plan) {
    exit(1);
  }
}

COMPRESSION_DATA::~COMPRESSION_DATA()
{
  cout << "Starting COMPRESSION_DATA cleanup" << endl;
  
  if (_dct_plan) {
    fftw_destroy_plan(_dct_plan);
  }
  
  if (_dct_in) {
    fftw_free(_dct_in);
    _dct_in = NULL;
  }
  
  if (_dct_out) {
    fftw_free(_dct_out);
    _dct_out = NULL;
  }
}

/*
COMPRESSION_DATA::COMPRESSION_DATA(COMPRESSION_DATA& data) :
  _dims(data.get_dims()), _paddedDims(data.get_paddedDims()), _numCols(data.get_numCols()), _nBits(data.get_nBits()), _percent(data.get_percent()),
  _maxIterations(data.get_maxIterations()), _numBlocks(data.get_numBlocks()) 
{
   _blockLengths = data.get_blockLengths();
   _blockIndices = data.get_blockIndices();
   _blockLengthsMatrix = *(data.get_blockLengthsMatrix());
   _blockIndicesMatrix = *(data.get_blockIndicesMatrix());
   _sList = *(data.get_sList());
   _gammaList = *(data.get_gammaList());
   _singularList = *(data.get_singularList());
   _vList = *(data.get_vList());
   _dampingArray = data.get_dampingArray();
   _zigzagArray = data.get_zigzagArray();
   _reverseZigzag = data.get_reverseZigzag();
   _dct_plan = data.get_dct_plan();

   // memory allocations
   if (data.get_dct_in()) {
     _dct_in = (double*)fftw_malloc(BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE * sizeof(double));
     }
   else { 
     _dct_in = NULL;
     }
   if (data.get_dct_out()) {
     _dct_out = (double*)fftw_malloc(BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE * sizeof(double));
   }
   else {
     _dct_out = NULL;
   }
}
*/

/*  
COMPRESSION_DATA& COMPRESSION_DATA::operator=(COMPRESSION_DATA& data)
{
   _dims = data.get_dims();
   _paddedDims = data.get_paddedDims();
   _numCols = data.get_numCols();
   _nBits = data.get_nBits();
   _percent = data.get_percent();
   _maxIterations = data.get_maxIterations();
   _numBlocks = data.get_numBlocks();
   _blockLengths = data.get_blockLengths();
   _blockIndices = data.get_blockIndices();
   _blockLengthsMatrix = *(data.get_blockLengthsMatrix());
   _blockIndicesMatrix = *(data.get_blockIndicesMatrix());
   _sList = *(data.get_sList());
   _gammaList = *(data.get_gammaList());
   _singularList = *(data.get_singularList());
   _vList = *(data.get_vList());
   _dampingArray = data.get_dampingArray();
   _zigzagArray = data.get_zigzagArray();
   _reverseZigzag = data.get_reverseZigzag();
   _dct_plan = data.get_dct_plan();

   // memory allocations
   if (data.get_dct_in()) {
     _dct_in = (double*)fftw_malloc(BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE * sizeof(double));
     }
   else { 
     _dct_in = NULL;
     }
   if (data.get_dct_out()) {
     _dct_out = (double*)fftw_malloc(BLOCK_SIZE * BLOCK_SIZE * BLOCK_SIZE * sizeof(double));
   }
   else {
     _dct_out = NULL;
   }

   return *this;
}
*/

void COMPRESSION_DATA::set_dampingArrayList() {
    TIMER functionTimer(__FUNCTION__);
    assert(_dampingArrayBuilt);
    if (_dampingArrayListBuilt) { return; }

    puts("Inside set_dampingArrayList!");

    FIELD_3D dampingArray(_dampingArray);
    int totalNumber = 4 * _nBits + 1;
    _dampingArrayList.resize(totalNumber);
    cout << "total number: " << totalNumber << endl;
    for (int i = 0; i < totalNumber; i++) {
      double gamma = i / 4.0;
      dampingArray.toFastPower(gamma);
      _dampingArrayList[i] = dampingArray;
      dampingArray = _dampingArray;
    }
    _dampingArrayListBuilt = true;
    cout << "array list built: " << _dampingArrayListBuilt << endl;
}

void COMPRESSION_DATA::dct_setup(int direction) {
    const int xRes = BLOCK_SIZE;
    const int yRes = BLOCK_SIZE;
    const int zRes = BLOCK_SIZE;

    _dct_in = (double*) fftw_malloc(xRes * yRes * zRes * sizeof(double));
    _dct_out = (double*) fftw_malloc(xRes * yRes * zRes * sizeof(double));

    if (direction == 1) { // forward transform
       _dct_plan = fftw_plan_r2r_3d(zRes, yRes, xRes, _dct_in, _dct_out, 
           FFTW_REDFT10, FFTW_REDFT10, FFTW_REDFT10, FFTW_MEASURE); 
    }
    else { // direction == -1; backward transform
       _dct_plan = fftw_plan_r2r_3d(zRes, yRes, xRes, _dct_in, _dct_out, 
           FFTW_REDFT01, FFTW_REDFT01, FFTW_REDFT01, FFTW_MEASURE);
    }
}

void COMPRESSION_DATA::dct_cleanup() {
   fftw_destroy_plan(_dct_plan);
   fftw_free(_dct_in);
   fftw_free(_dct_out);
   _dct_in = NULL;
   _dct_out = NULL;
   fftw_cleanup();
}

void COMPRESSION_DATA::set_dampingArray() {
    TIMER functionTimer(__FUNCTION__);
    if (_dampingArrayBuilt) { return; }

    // Create a damping array of size BLOCK_SIZE x BLOCK_SIZE x BLOCK_SIZE
    _dampingArray.resizeAndWipe(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
    
    // Fill the array based on frequency
    for (int k = 0; k < BLOCK_SIZE; k++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            for (int i = 0; i < BLOCK_SIZE; i++) {
                double freqX = (i * M_PI) / (2 * BLOCK_SIZE);
                double freqY = (j * M_PI) / (2 * BLOCK_SIZE);
                double freqZ = (k * M_PI) / (2 * BLOCK_SIZE);
                
                // Calculate damping factor based on frequency
                double damping = 1.0 / (1.0 + freqX * freqX + freqY * freqY + freqZ * freqZ);
                _dampingArray(i, j, k) = damping;
            }
        }
    }
    
    _dampingArrayBuilt = true;
}

void COMPRESSION_DATA::set_zigzagArray() {
    TIMER functionTimer(__FUNCTION__);
    if (_zigzagArrayBuilt) { return; }

    // Create zigzag and reverse zigzag arrays
    _zigzagArray.resizeAndWipe(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
    _reverseZigzag.resizeAndWipe(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
    
    // Fill arrays with indices in zigzag order
    int index = 0;
    for (int sum = 0; sum < 3 * (BLOCK_SIZE - 1); sum++) {
        for (int k = 0; k < BLOCK_SIZE; k++) {
            for (int j = 0; j < BLOCK_SIZE; j++) {
                for (int i = 0; i < BLOCK_SIZE; i++) {
                    if (i + j + k == sum) {
                        _zigzagArray(i, j, k) = index;
                        _reverseZigzag(i, j, k) = index;
                        index++;
                    }
                }
            }
        }
    }
    
    _zigzagArrayBuilt = true;
}
