# Compressing Fluid Subspaces Reference Implementation

This is a reference implementation of the algorithm described in the paper *Compressing Fluid Subspaces*. It is intended to be the simplest possible working example of a DCT-based fluid subspace compression codec. In keeping with this simplicity, extra features such as obstacle handling and multithread support have been removed.

## Building the Code

We have successfully built this code in Mac OS X (10.9, Mavericks). The default `Makefile` is for OS X. We have made an effort to remove dependencies on external libraries, so aside from some commonly available libraries (`fftw3`, `zlib`, `libpng`, `GLUT`), you should not need to download and install anything special in order to successfully compile. Notably, CFS requires the `Eigen` library, but it comes included in the download. You should be able to build using the following sequence:

    gunzip CFS_PLUME.tar.gz
    tar -xvf CFS_PLUME.tar
    cd CFS_PLUME
    make

If you are successful, the binaries:

- `buildProducts`
- `compressMatrices`
- `cubatureGeneratorStamStaged`
- `fluid3D`
- `reduced3D`
- `reducedCompressed3D`
- `svdOutOfCoreMultiple`

should now be in the `CFS_PLUME/bin` directory.

## Running the Code

The configuration file for a very small example is provided, `./cfg/stam.64.cfg`. Running the entire subspace pipeline on a large example can be very time consuming, so it is highly recommended that you first verify that this example can be run successfully. The following is the execution sequence.

From the `CFS_PLUME` directory, start by generating all the preprocess data, including compressed data:

    sudo ./runPreprocess


This will generate snapshot data, build the empirical eigenvectors in order to construct a subspace, generate a semi-Lagrangian cubature scheme for the subspace, precompute subspace run-time matrices, and generate compressed subspace matrix data.

Next, to do either the uncompressed subspace re-simulation or the compressed subspace re-simulation, run:

    sudo ./runUncompressed

    
or

    sudo ./runCompressed

respectively. The compressed simulation is set to approximately 6:1 compression.

By default, both `runUncompressed` and `runCompressed` load up the original simulation data and compare it to the subspace result. If the cubature has trained correctly, you should see the relative error of the velocity field in the uncompressed subspace simulation remain below 1%. Depending on the compression level, in the compressed simulation, this error should remain below 10% for results not to degrade in noticeable visual quality.

Note that the simulations will not run as fast as they otherwise could, since the time from pulling a comparison file from disk at each timestep is quite expensive. If your 6:1 compressed simulation gets to 50 timesteps and outputs something along the lines of:

    =====================================================
    Ground truth difference for frame 49
    =====================================================
    velocity abs error: 0.213755
    velocity relative error: 0.0529028
    density abs error: 1.13693
    density relative error: 0.0480764

then your installation has succeeded.

If you would like to vary the amount of compression, you must edit the percent parameter in the configuration ./cfg/stam.64.cfg. In practice, values quite close to 1.0, e.g., 0.999, 0.99, etc., are generally advised for high quality results.
