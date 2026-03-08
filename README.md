# Compressing Fluid Subspaces Reference Implementation

<img width="600" alt="compressions" src="https://github.com/user-attachments/assets/087128ff-8f71-458c-80cc-ca705e8693f6">

Reference implementation of the algorithm from [*Compressing Fluid Subspaces*](https://www.tkim.graphics/COMPRESSING/JonesSenKim2016.pdf) (Jones, Sen, Kim 2016). A DCT-based codec for compressing reduced-order fluid simulation bases.

## Dependencies

Install via Homebrew (macOS):

```bash
brew install fftw libpng jpeg zlib
```

Also requires: OpenGL, GLUT, and the Accelerate framework (included with macOS). Eigen is bundled in `src/Eigen/`.

## Building

```bash
make          # Build all 8 binaries to ./bin/
make clean    # Remove all .o files and binaries
```

Binaries: `fluid3D`, `svdOutOfCoreMultiple`, `cubatureGeneratorStamStaged`, `cubatureMemory`, `reduced3D`, `compressMatrices`, `buildProducts`, `reducedCompressed3D`.

Two additional comparison binaries are not in the top-level Makefile. Build them separately if needed:

```bash
cd projects/reduced3DWriteVelocities && make -f Makefile
cd projects/reducedCompressed3DCompareVelocities && make -f Makefile
```

## Running

All commands run from the project root. The config file `cfg/stam.64.cfg` defines the small (48x64x48) test case.

### 1. Preprocess (generate data, build subspace, compress)

```bash
./runPreprocess
```

This runs the full 5-step pipeline: full-rank simulation (50 timesteps), SVD, cubature generation, matrix compression, and product precomputation. Takes several minutes.

### 2. Simulate

```bash
./runUncompressed          # Reduced-order simulation (ground truth)
./runCompressed            # Compressed simulation (~6:1 compression)
```

Both load original snapshots and report per-timestep error metrics. Expected results at frame 49 for 6:1 compression:

```
velocity relative error: 0.0529028
density relative error: 0.0480764
```

Uncompressed should stay below 1% relative error; compressed below 10%.

### 3. Movies

Simulation movies are written to `./movies/` in legacy MJPEG QuickTime format. Convert to H.264 for modern playback:

```bash
./movies/convert.sh
```

## Tests

```bash
cd tests && make           # Build test binary
cd .. && ./tests/test_compression_data   # Run from project root (needs ./data/)
```

17 tests covering the decode pipeline, codec round-trip accuracy, and arithmetic helpers.

## Configuration

Edit `cfg/stam.64.cfg` to change parameters. Key settings:

| Parameter | Default | Description |
|---|---|---|
| `percent` | 0.999999 | Compression ratio (0.99 = keep 99%, more lossy) |
| `nBits` | 32 | Quantization bit depth |
| `simulation snapshots` | 50 | Number of timesteps |
| `xRes/yRes/zRes` | 48/64/48 | Grid resolution |

Note: `compressMatrices` rewrites `compression path` and `preview movie` in the config file automatically based on the achieved compression ratio.

## Project Structure

```
CFS_PLUME/
  cfg/              Config files
  src/              Source libraries (compression, integrators, geometry, etc.)
  projects/         Per-binary build directories with Makefiles
  tests/            Compression codec tests
  bin/              Compiled binaries (generated)
  data/             Simulation data (generated)
  movies/           Output videos (generated)
  scratch/          Temporary files for out-of-core SVD (generated)
```

## Credits

Aaron Demby Jones, Pradeep Sen, and Theodore Kim
