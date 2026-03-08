#!/bin/bash
set -e

CFG=${1:-cfg/stam.64.cfg}

if [ ! -f "$CFG" ]; then
    echo "Error: Config file not found: $CFG"
    exit 1
fi

if [ ! -f "./bin/reduced3D" ]; then
    echo "Error: ./bin/reduced3D not found. Run 'make' first."
    exit 1
fi

mkdir -p ./movies

echo "Using config file: $CFG"
./bin/reduced3D "$CFG"
