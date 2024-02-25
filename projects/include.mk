LDFLAGS_COMMON = -framework Accelerate -framework GLUT -framework OpenGL -L/opt/homebrew/lib -lstdc++ -lz -lm -fopenmp
LDFLAGS_COMMON += -L/opt/homebrew/opt/libpng/lib -lpng
LDFLAGS_COMMON += -L/opt/homebrew/opt/jpeg/lib -ljpeg
LDFLAGS_COMMON += -L/opt/homebrew/opt/fftw/lib -lfftw3
CFLAGS_COMMON = -v -c -Wall -Wno-deprecated -I../../src/integrators -I../../src/linearalgebra -I../../src/geometry -I../../src/util -I../../src/glvu -DDO_PNG_OUT=0 -I./ -I/opt/homebrew/include -I../../ -I../../src/Eigen/ -O3 -DNO_FFT -fopenmp -lstdc++ -I/opt/local/include/ -flax-vector-conversions
CFLAGS_COMMON += -fopenmp

