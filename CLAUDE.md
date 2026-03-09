# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Reference implementation of the algorithm from [*Compressing Fluid Subspaces*](https://www.tkim.graphics/COMPRESSING/JonesSenKim2016.pdf) (Jones, Sen, Kim 2016). A DCT-based codec for compressing reduced-order fluid simulation bases. Written entirely in C++.

## Build Commands

```bash
make                # Build all 8 binaries (macOS, default)
make ubuntu         # Build for Linux (uses Makefile.ubuntu per project)
make clean          # Remove all .o files and binaries
make objclean       # Remove only .o files (keep binaries)
make ctags          # Generate ctags
```

Binaries are output to `./bin/`. Each project has its own Makefile in `projects/<name>/`. Shared compiler/linker config lives in `projects/include.mk`.

To rebuild a single binary:
```bash
cd projects/<name> && make -f Makefile
```

## Dependencies (Homebrew)

`fftw3`, `zlib`, `libpng`, `libjpeg`, `GLUT/OpenGL`, macOS Accelerate framework. Eigen is bundled in `src/Eigen/`.

## Execution Pipeline

The system has two phases: offline preprocessing and online simulation.

**Preprocessing** (run sequentially via `./scripts/runPreprocess.sh`):
1. `fluid3D` — Generate full-rank simulation snapshots (100 timesteps default)
2. `svdOutOfCoreMultiple` — Compute POD basis via out-of-core SVD
3. `cubatureGeneratorStamStaged` — Build semi-Lagrangian cubature scheme
4. `compressMatrices` — Compress basis matrices using DCT
5. `buildProducts` — Precompute runtime matrix products

**Simulation** (run independently):
- `./scripts/runUncompressed.sh` — Reduced-order simulation without compression (ground truth)
- `./scripts/runCompressed.sh` — Compressed reduced-order simulation (~5:1 compression)

All scripts use config file `cfg/stam.64.cfg`. Both simulation scripts load original snapshots and report error metrics per timestep.

**Important:** Changing `simulation snapshots` requires a full re-run of `./scripts/runPreprocess.sh` — the SVD produces a different-sized basis. `buildProducts` auto-detects stale projected matrices and rebuilds them, but cubature and compression must also be regenerated.

## Architecture

### Source Libraries (`src/`)

| Directory | Purpose |
|---|---|
| `compression/` | DCT-based block compression codec (BLOCK_SIZE=8, configurable in `SETTINGS.h`) |
| `integrators/` | Fluid solvers: full-rank (`FLUID_3D_MIC`), reduced (`SUBSPACE_FLUID_3D_EIGEN`), compressed (`SUBSPACE_FLUID_3D_COMPRESSED_EIGEN`) |
| `geometry/` | 3D scalar/vector fields (`FIELD_3D`, `VECTOR3_FIELD_3D`), obstacles (`BOX`) |
| `linearalgebra/` | Matrix I/O, sparse matrices, out-of-core SVD, NNLS solver |
| `cubature/` | Semi-Lagrangian cubature scheme generation |
| `glvu/` | OpenGL visualization framework |
| `util/` | Config parsing (`SIMPLE_PARSER`), timers, image output |

### Global Settings

`SETTINGS.h` at the project root defines:
- `BLOCK_SIZE 8` — DCT compression block size
- `Real` type — defaults to `double` (can switch to `float` via `#define`)

### Configuration (`cfg/stam.64.cfg`)

Key parameters: `snapshot path`, `reduced path`, `compression path`, `percent` (compression ratio, e.g. 0.99 = 1% retained), `nBits` (quantization bit depth), `maxIterations`, `simulation snapshots`.

### Data Flow

Full-rank snapshots → POD eigenvectors (SVD) → cubature points → compressed matrices (DCT blocks) → runtime simulation

## Compiler Notes

- Auto-detects GCC 14/13 or falls back to system default (see `include.mk`)
- Auto-detects Homebrew prefix (`brew --prefix`) for ARM64 (`/opt/homebrew`) and Intel (`/usr/local`) Macs
- Flags: `-O3`, `-DNO_FFT`, `-flax-vector-conversions`, `-w` (warnings suppressed)
- Links against Accelerate framework for BLAS/LAPACK on macOS

## Tests

Build and run from the project root (tests use relative paths to `./data/`):
```bash
cd tests && make && cd .. && ./tests/test_compression_data
```

31 tests covering decode pipeline, codec round-trip accuracy (2.6e-9 near-lossless error), project/unproject hot path, SVD coordinate transform, multi-block RLE, DCT energy compaction, SparseBlockDiagonal, and arithmetic helpers.

## Validation

Compression error is measured as compressed vs uncompressed q-vectors (not vs full-rank ground truth, which measures subspace approximation quality instead).

With 100 snapshots and `percent=0.9995` (5:1 compression), relative L2 error between compressed and uncompressed q-vectors stabilizes around 1% through all 100 steps.

## Additional Binaries

Not in the top-level Makefile; build separately:
```bash
cd projects/reduced3DWriteVelocities && make -f Makefile           # Write uncompressed q-vectors
cd projects/reducedCompressed3DCompareVelocities && make -f Makefile  # Compare compressed vs uncompressed
cd projects/viewModes && make -f Makefile                          # Visualize POD eigenvector modes
```

## Important Conventions

- `SIMPLE_PARSER` does NOT handle inline `#` comments—comments must be on their own line in `.cfg` files
- `compressMatrices` rewrites `cfg/stam.64.cfg` via `cfg/findReplace.py` to update compression/movie paths
- Movie output goes to `./movies/` in legacy MJPEG format; use `./scripts/convert-movies.sh` to convert to H.264
- All binaries must be run from the project root (relative paths throughout)
