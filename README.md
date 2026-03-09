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

Additional binaries not in the top-level Makefile (build separately if needed):

```bash
cd projects/reduced3DWriteVelocities && make -f Makefile           # Write uncompressed q-vectors
cd projects/reducedCompressed3DCompareVelocities && make -f Makefile  # Compare compressed vs uncompressed
cd projects/viewModes && make -f Makefile                          # Visualize POD eigenvector modes
```

## Running

All commands run from the project root. The config file `cfg/stam.64.cfg` defines the small (48x64x48) test case.

### 1. Preprocess (generate data, build subspace, compress)

```bash
./scripts/runPreprocess.sh
```

This runs the full 5-step pipeline: full-rank simulation (100 timesteps), SVD, cubature generation, matrix compression, and product precomputation. Takes several minutes.

### 2. Simulate

```bash
./scripts/runUncompressed.sh       # Reduced-order simulation (no compression)
./scripts/runCompressed.sh         # Compressed simulation
```

### Comparison pipeline

To measure compression error directly (compressed vs uncompressed subspace), build and run the comparison binaries:

```bash
cd projects/reduced3DWriteVelocities && make -f Makefile && cd ../..
cd projects/reducedCompressed3DCompareVelocities && make -f Makefile && cd ../..
./bin/reduced3DWriteVelocities cfg/stam.64.cfg
./bin/reducedCompressed3DCompareVelocities cfg/stam.64.cfg
```

With the default config (`percent=0.9995`, 100 snapshots), per-step relative L2 error between compressed and uncompressed q-vectors at 5:1 compression:

```
step  1: 0.25%     step 50: 0.91%     step 100: 1.06%
```

Error stabilizes around 1%—the 52-mode basis (from 100 snapshots) is rich enough to absorb compression perturbation without chaotic divergence.

To try different compression ratios, edit `percent` in `cfg/stam.64.cfg` and re-run `compressMatrices` then `buildProducts`.

### 3. Movies

Simulation movies are written to `./movies/` in legacy MJPEG QuickTime format. Convert to H.264 for modern playback:

```bash
./scripts/convert-movies.sh
```

## Tests

```bash
cd tests && make           # Build test binary
cd .. && ./tests/test_compression_data   # Run from project root (needs ./data/)
```

31 tests covering the decode pipeline, codec round-trip accuracy, RLE encode/decode, block decomposition, DCT, zigzag scan, SVD coordinate transform, multi-block RLE, DCT energy compaction, SparseBlockDiagonal, and arithmetic helpers.

## Configuration

Edit `cfg/stam.64.cfg` to change parameters. Key settings:

| Parameter | Default | Description |
|---|---|---|
| `percent` | 0.9995 | Energy retention (0.99 = keep 99%, more lossy; 0.9995 ≈ 5:1) |
| `nBits` | 32 | Quantization bit depth |
| `simulation snapshots` | 100 | Number of timesteps |
| `xRes/yRes/zRes` | 48/64/48 | Grid resolution |

Note: `compressMatrices` rewrites `compression path` and `preview movie` in the config file automatically based on the achieved compression ratio.

## Project Structure

```
CFS_PLUME/
  cfg/              Config files
  scripts/          Shell scripts (preprocess, simulate, convert movies)
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
