# Auto-detect GCC version if not set
CC := $(shell command -v gcc-14 2>/dev/null || command -v gcc-13 2>/dev/null || command -v gcc)
CXX := $(shell command -v g++-14 2>/dev/null || command -v g++-13 2>/dev/null || command -v g++)

# Create bin directory if it doesn't exist
$(shell mkdir -p ../../bin)

# Auto-detect Homebrew prefix (ARM64: /opt/homebrew, Intel: /usr/local)
HOMEBREW_PREFIX := $(shell brew --prefix 2>/dev/null || echo /opt/homebrew)

LDFLAGS_COMMON = -framework Accelerate -framework GLUT -framework OpenGL -L$(HOMEBREW_PREFIX)/lib -lstdc++ -lz -lm
LDFLAGS_COMMON += -L$(HOMEBREW_PREFIX)/opt/libpng/lib -lpng
LDFLAGS_COMMON += -L$(HOMEBREW_PREFIX)/opt/jpeg/lib -ljpeg
LDFLAGS_COMMON += -L$(HOMEBREW_PREFIX)/opt/fftw/lib -lfftw3
CFLAGS_COMMON = -c -w -Wno-deprecated -Wno-c++11-narrowing -I../../src/integrators -I../../src/linearalgebra -I../../src/geometry -I../../src/util -I../../src/glvu -I../../src/compression -DDO_PNG_OUT=0 -I./ -I$(HOMEBREW_PREFIX)/include -I../../ -I../../src/Eigen/ -O3 -DNO_FFT -lstdc++ -I/opt/local/include/ -flax-vector-conversions



