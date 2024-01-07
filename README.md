# Compressing Fluid Subspaces Reference Implementation

This is a reference implementation of the algorithm described in the paper *Compressing Fluid Subspaces*. It is intended to be the simplest possible working example of a DCT-based fluid subspace compression codec. In keeping with this simplicity, extra features such as obstacle handling and multithread support have been removed.

## Building the Code

We have successfully built this code in Mac OS X (10.9, Mavericks). The default `Makefile` is for OS X. We have made an effort to remove dependencies on external libraries, so aside from some commonly available libraries (`fftw3`, `zlib`, `libpng`, `GLUT`), you should not need to download and install anything special in order to successfully compile. Notably, CFS requires the `Eigen` library, but it comes included in the download. You should be able to build using the following sequence:

    gunzip CFS_PLUME.tar.gz
    tar -xvf CFS_PLUME.tar
    cd CFS_PLUME
    make
