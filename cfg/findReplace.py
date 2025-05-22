import fileinput
import subprocess
import sys
import re

def findReplace(filename, toReplace, replacement):
    # Ensure arguments are strings
    toReplace = toReplace.decode('utf-8') if isinstance(toReplace, bytes) else toReplace
    replacement = replacement.decode('utf-8') if isinstance(replacement, bytes) else replacement
    
    for line in fileinput.input(filename, inplace=True):
        # Only replace if the line starts with the pattern (ignoring whitespace)
        # This prevents matching comments or partial matches
        if line.strip().startswith(toReplace.split('=')[0].strip()):
            print(replacement, end='')
        else:
            print(line, end='')

def getCompressionLine(filename, pattern):
    try:
        with open(filename, 'r') as f:
            for line in f:
                # Strip comments and whitespace for comparison
                clean_line = line.split('#')[0].strip()
                if re.search(pattern, clean_line, re.IGNORECASE):
                    return clean_line
        return None
    except Exception as e:
        print(f"Error reading file: {e}")
        return None

################################################################################
argc = len(sys.argv)
if argc != 3:
    print("You must pass two command line arguments: a filename, and a compression ratio!")
    sys.exit(-1)

# Get the arguments list 
filename = sys.argv[1]
compressionRatio = sys.argv[2]

# Find and replace the compression path
compressionPattern = r"compression path\s*=\s*\./data/reduced\.stam\.64/\d+to1/"
replacement = f"compression path = ./data/reduced.stam.64/{compressionRatio}to1/"
toReplace = getCompressionLine(filename, compressionPattern)

if toReplace is None:
    print(f"Warning: Could not find compression path pattern in {filename}")
    print("Skipping compression path update...")
else:
    findReplace(filename, toReplace, replacement)
    print(f"Updated compression path to: {replacement}")

# Try to find and replace the movie path - make this optional
moviePattern = r"preview movie\s*=\s*\./movies/compressed\.\d+to1\.mov"
replacement = f"preview movie = ./movies/compressed.{compressionRatio}to1.mov"
toReplace = getCompressionLine(filename, moviePattern)

if toReplace is None:
    print(f"Note: Could not find movie path pattern in {filename}")
    print("Skipping movie path update...")
else:
    findReplace(filename, toReplace, replacement)
    print(f"Updated movie path to: {replacement}")

print("Config file update complete!")
