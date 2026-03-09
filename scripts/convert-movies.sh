#!/bin/bash
# Convert legacy MJPEG .mov files to H.264 .mp4 for modern playback
# Requires: ffmpeg (brew install ffmpeg)
#
# Usage: ./scripts/convert-movies.sh [input.mov]
#   If no argument given, converts all .mov files in ./movies/

set -e

convert_one() {
    local input="$1"
    local output="${input%.mov}.mp4"
    if [ -f "$output" ]; then
        echo "  skip: $output already exists"
        rm -f "$input"
        return
    fi
    echo "  converting: $input -> $output"
    ffmpeg -i "$input" -c:v libx264 -crf 18 -preset fast -pix_fmt yuv420p -y "$output" 2>/dev/null
    rm -f "$input"
    echo "  removed: $input"
}

if [ -n "$1" ]; then
    convert_one "$1"
else
    for f in movies/*.mov; do
        [ -f "$f" ] || continue
        convert_one "$f"
    done
fi
