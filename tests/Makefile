# Test Makefile for CFS_PLUME
# Modeled on projects/compressMatrices/Makefile but without OpenGL/GLUT

include ../projects/include.mk

# Auto-detect Homebrew prefix (ARM64: /opt/homebrew, Intel: /usr/local)
HOMEBREW_PREFIX := $(shell brew --prefix 2>/dev/null || echo /opt/homebrew)

LDFLAGS = -framework Accelerate -framework GLUT -framework OpenGL \
          -L$(HOMEBREW_PREFIX)/lib -lstdc++ -lz -lm \
          -L$(HOMEBREW_PREFIX)/opt/libpng/lib -lpng \
          -L$(HOMEBREW_PREFIX)/opt/jpeg/lib -ljpeg \
          -L$(HOMEBREW_PREFIX)/opt/fftw/lib -lfftw3

CFLAGS = -c -w -Wno-deprecated \
         -I../src/integrators -I../src/linearalgebra -I../src/geometry \
         -I../src/util -I../src/compression -I../src/glvu \
         -I../ -I../src/Eigen/ \
         -I$(HOMEBREW_PREFIX)/include -I/opt/local/include/ \
         -I. \
         -DDO_PNG_OUT=0 -DNO_FFT -O2 -flax-vector-conversions \
         -Wno-c++11-narrowing

# Source files needed for compression data validation.
# We need the compression codec + supporting geometry + linear algebra + GLVU stubs.
LIB_SOURCES = ../src/compression/COMPRESSION.cpp \
              ../src/compression/COMPRESSION_DATA.cpp \
              ../src/compression/MATRIX_COMPRESSION_DATA.cpp \
              ../src/linearalgebra/EIGEN.cpp \
              ../src/linearalgebra/VECTOR.cpp \
              ../src/linearalgebra/MATRIX.cpp \
              ../src/linearalgebra/MATRIX3.cpp \
              ../src/linearalgebra/SPARSE_MATRIX.cpp \
              ../src/linearalgebra/SPARSE_MATRIX_ARRAY.cpp \
              ../src/linearalgebra/NNLS.cpp \
              ../src/linearalgebra/BIG_MATRIX.cpp \
              ../src/geometry/FIELD_3D.cpp \
              ../src/geometry/FIELD_2D.cpp \
              ../src/geometry/VECTOR3_FIELD_3D.cpp \
              ../src/geometry/VECTOR3_FIELD_2D.cpp \
              ../src/geometry/INTEGER_FIELD_3D.cpp \
              ../src/geometry/BOX.cpp \
              ../src/integrators/FLUID_3D_MIC.cpp \
              ../src/integrators/SUBSPACE_FLUID_3D_COMPRESSED_EIGEN.cpp \
              ../src/util/TIMER.cpp \
              ../src/util/SIMPLE_PARSER.cpp \
              ../src/util/MIN_HEAP.cpp \
              ../src/glvu/glvu.cpp

# Test sources
TEST_SOURCES = test_compression_data.cpp

# All sources
SOURCES = $(TEST_SOURCES) $(LIB_SOURCES)
OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = test_compression_data

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

# Run tests from project root so data paths resolve correctly
test: $(EXECUTABLE)
	cd .. && ./tests/$(EXECUTABLE)

clean:
	rm -f *.o $(EXECUTABLE)
	rm -f ../src/compression/*.o ../src/linearalgebra/*.o ../src/geometry/*.o
	rm -f ../src/integrators/*.o ../src/util/*.o ../src/glvu/*.o
