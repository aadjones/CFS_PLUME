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

**Preprocessing** (run sequentially via `./runPreprocess`):
1. `fluid3D` — Generate full-rank simulation snapshots (50 timesteps)
2. `svdOutOfCoreMultiple` — Compute POD basis via out-of-core SVD
3. `cubatureGeneratorStamStaged` — Build semi-Lagrangian cubature scheme
4. `compressMatrices` — Compress basis matrices using DCT
5. `buildProducts` — Precompute runtime matrix products

**Simulation** (run independently):
- `./runUncompressed` — Reduced-order simulation without compression (ground truth)
- `./runCompressed` — Compressed reduced-order simulation (~6:1 compression)

All scripts use config file `cfg/stam.64.cfg`. Both simulation scripts load original snapshots and report error metrics per timestep.

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
- Flags: `-O3`, `-DNO_FFT`, `-flax-vector-conversions`, `-w` (warnings suppressed)
- Links against Accelerate framework for BLAS/LAPACK on macOS

## Tests

Build and run from the project root (tests use relative paths to `./data/`):
```bash
cd tests && make && cd .. && ./tests/test_compression_data
```

17 tests covering decode pipeline, codec round-trip accuracy (2.6e-9 near-lossless error), project/unproject hot path (1.08e-10 relative error), and arithmetic helpers.

## Validation

Integration-based validation via simulation error metrics:
- Uncompressed simulation: velocity relative error should stay below 1%
- Compressed simulation (6:1): velocity relative error should stay below 10%
- Success example at frame 49: `velocity relative error: 0.0529028`

## Comparison Binaries

Two additional binaries for per-step subspace comparison (not in top-level Makefile):
```bash
cd projects/reduced3DWriteVelocities && make -f Makefile
cd projects/reducedCompressed3DCompareVelocities && make -f Makefile
```

## Important Conventions

- `SIMPLE_PARSER` does NOT handle inline `#` comments—comments must be on their own line in `.cfg` files
- `compressMatrices` rewrites `cfg/stam.64.cfg` via `cfg/findReplace.py` to update compression/movie paths
- Movie output goes to `./movies/` in legacy MJPEG format; use `./movies/convert.sh` to convert to H.264
- All binaries must be run from the project root (relative paths throughout)
