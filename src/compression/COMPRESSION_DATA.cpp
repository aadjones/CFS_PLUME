#include "COMPRESSION_DATA.h"
#include <iostream>

COMPRESSION_DATA::COMPRESSION_DATA() {}

COMPRESSION_DATA::COMPRESSION_DATA(const VEC3I& dims, int numCols, int nBits, double percent) :
  _dims(dims), _numCols(numCols), _nBits(nBits), _percent(percent) {}

COMPRESSION_DATA::~COMPRESSION_DATA()
{
  if (_dct_plan) {
    fftw_destroy_plan(_dct_plan);
    _dct_plan = nullptr;
  }
  if (_dct_in) {
    fftw_free(_dct_in);
    _dct_in = nullptr;
  }
  if (_dct_out) {
    fftw_free(_dct_out);
    _dct_out = nullptr;
  }
}

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
      dampingArray.toPower(gamma);
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
    if (_dampingArrayBuilt) { return; }

    int uRes = BLOCK_SIZE;
    int vRes = BLOCK_SIZE;
    int wRes = BLOCK_SIZE;
    FIELD_3D damp(uRes, vRes, wRes);

    for (int w = 0; w < wRes; w++) {
      for (int v = 0; v < vRes; v++) {
        for (int u = 0; u < uRes; u++) {
          damp(u, v, w) = 1 + u + v + w;
        }
      }
    }
    _dampingArray = damp;
    _dampingArrayBuilt = true;
}

void COMPRESSION_DATA::set_zigzagArray() {
    TIMER functionTimer(__FUNCTION__);
    if (_zigzagArrayBuilt) { return; }

    int xRes = BLOCK_SIZE;
    int yRes = BLOCK_SIZE;
    int zRes = BLOCK_SIZE;
    INTEGER_FIELD_3D zigzagArray(xRes, yRes, zRes);
    int i = 0;
    for (int sum = 0; sum < xRes + yRes + zRes; sum++) {
      for (int z = 0; z < zRes; z++) {
        for (int y = 0; y < yRes; y++) {
          for (int x = 0; x < xRes; x++) {
            if (x + y + z == sum) {
              zigzagArray(x, y, z) = i;
              i++;
            }
          }
        }
      }
    }
    _zigzagArray = zigzagArray;

    // cache the reverse too
    _reverseZigzag = _zigzagArray;
    for (int x = 0; x < zigzagArray.totalCells(); x++)
      _reverseZigzag[zigzagArray[x]] = x;

    _zigzagArrayBuilt = true;
}
