#!/bin/bash
set -e

CFG=${1:-cfg/stam.64.cfg}

if [ ! -f "$CFG" ]; then
    echo "Error: Config file not found: $CFG"
    exit 1
fi

# Extract paths from config
SNAPSHOT_PATH=$(grep "^snapshot path" "$CFG" | cut -d'=' -f2 | tr -d ' ')
REDUCED_PATH=$(grep "^reduced path" "$CFG" | cut -d'=' -f2 | tr -d ' ')

echo "Config: $CFG"
echo "Snapshot path: $SNAPSHOT_PATH"
echo "Reduced path: $REDUCED_PATH"

# Create all directories the pipeline needs
mkdir -p "$SNAPSHOT_PATH"
mkdir -p "$REDUCED_PATH"
mkdir -p ./scratch
mkdir -p ./movies
mkdir -p ./bin

# Pipeline executables (run sequentially)
executables=("fluid3D" "svdOutOfCoreMultiple" "cubatureGeneratorStamStaged" "compressMatrices" "buildProducts")

for exe in "${executables[@]}"; do
    if [ ! -f "./bin/$exe" ]; then
        echo "Error: ./bin/$exe not found. Run 'make' first."
        exit 1
    fi
    echo ""
    echo "==== Running $exe ===="
    ./bin/$exe "$CFG"
    echo "==== $exe completed ===="
done

echo ""
echo "All preprocessing steps completed successfully."
