#ifndef COMPRESSION_DATA_H
#define COMPRESSION_DATA_H

#include "EIGEN.h"
#include <iostream>
#include <fftw3.h>
#include "FIELD_3D.h"
#include "INTEGER_FIELD_3D.h"

// pushed to SETTINGS.h
//#define BLOCK_SIZE 8
//#define BLOCK_SIZE 16

using std::cout;
using std::endl;


class COMPRESSION_DATA {
  public:
    COMPRESSION_DATA();
    COMPRESSION_DATA(const VEC3I& dims, int numCols, int nBits, double percent);
    //COMPRESSION_DATA(COMPRESSION_DATA& data);
    ~COMPRESSION_DATA();

    //COMPRESSION_DATA& operator=(COMPRESSION_DATA& data);

    // Function declarations
    void set_dampingArray();
    void set_dampingArrayList();
    void set_zigzagArray();
    void dct_setup(int direction);
    void dct_cleanup();

    // Inline getters
    const VEC3I& get_dims() const { return _dims; }
    const VEC3I& get_paddedDims() const { return _paddedDims; }
    int get_numCols() const { return _numCols; }
    int get_numBlocks() const { return _numBlocks; }
    int get_currBlockNum() const { return _currBlockNum; }
    double get_percent() const { return _percent; }
    int get_nBits() const { return _nBits; } 
    int get_maxIterations() const { return _maxIterations; } 
    const VectorXi& get_blockLengths() const { return _blockLengths; }
    const VectorXi& get_blockIndices() const { return _blockIndices; }
    const FIELD_3D& get_dampingArray() const { return _dampingArray; }
    const INTEGER_FIELD_3D& get_zigzagArray() const { return _zigzagArray; }
    const INTEGER_FIELD_3D& get_reverseZigzag() const { return _reverseZigzag; }
    double* get_dct_in() const { return _dct_in; }
    double* get_dct_out() const { return _dct_out; }
    fftw_plan get_dct_plan() const { return _dct_plan; }
    bool get_arrayListBuilt() const { return _dampingArrayListBuilt; }
    const vector<FIELD_3D>& get_dampingArrayList() const { return _dampingArrayList; }

    // Add getters for SVD data
    vector<Vector3d>* get_singularList() { return &_singularList; }
    vector<Matrix3d>* get_vList() { return &_vList; }
    MatrixXd* get_sListMatrix() { return &_sListMatrix; }
    MatrixXd* get_gammaListMatrix() { return &_gammaListMatrix; }
    MatrixXi* get_blockLengthsMatrix() { return &_blockLengthsMatrix; }
    MatrixXi* get_blockIndicesMatrix() { return &_blockIndicesMatrix; }

    // Inline setters
    void set_dims(const VEC3I& dims) { _dims = dims; }
    void set_paddedDims(const VEC3I& paddedDims) { _paddedDims = paddedDims; }
    void set_numCols(int numCols) { _numCols = numCols; }
    void set_numBlocks(int numBlocks) { _numBlocks = numBlocks; }
    void set_currBlockNum(int currBlockNum) { _currBlockNum = currBlockNum; }
    void set_percent(double percent) { _percent = percent; }
    void set_nBits(int nBits) { _nBits = nBits; }
    void set_maxIterations(int maxIterations) { _maxIterations = maxIterations; }
    void set_blockLengths(const VectorXi& blockLengths) { 
      int length = blockLengths.size();
      assert(length == _numBlocks);
      _blockLengths = blockLengths; 
    }
    void set_blockIndices(const VectorXi& blockIndices) { 
      int length = blockIndices.size();
      assert(length == _numBlocks);
      _blockIndices = blockIndices;
    }
    void set_sList(const VectorXd& sList) { 
      int length = sList.size();
      assert(length == _numBlocks);
      _sList = sList;
    }
    void set_vList(const vector<Matrix3d>& vList) {
      assert(vList.size() == _numCols);
      _vList = vList;
    }

  private:
    VEC3I _dims;
    VEC3I _paddedDims;
    int _numCols = 0;
    int _numBlocks = 0;
    int _currBlockNum = 0;
    int _maxIterations = 0;
    double _nBits = 0;
    double _percent = 0;

    // NOTE: not thread-safe. If OpenMP is added to decode paths,
    // these guards need std::call_once (requires making COMPRESSION_DATA non-copyable).
    bool _dampingArrayBuilt = false;
    bool _dampingArrayListBuilt = false;
    bool _zigzagArrayBuilt = false;

    VectorXi _blockLengths;
    VectorXi _blockIndices;
    VectorXd _sList;
    VectorXd _gammaList;

    MatrixXi _blockLengthsMatrix;
    MatrixXi _blockIndicesMatrix;
    MatrixXd _sListMatrix;
    MatrixXd _gammaListMatrix;

    vector<Matrix3d> _vList;
    vector<Vector3d> _singularList;
    vector<FIELD_3D> _dampingArrayList;

    FIELD_3D _dampingArray;
    INTEGER_FIELD_3D _zigzagArray;
    INTEGER_FIELD_3D _reverseZigzag;

    double* _dct_in = nullptr;
    double* _dct_out = nullptr;
    fftw_plan _dct_plan = nullptr;
};

#endif

