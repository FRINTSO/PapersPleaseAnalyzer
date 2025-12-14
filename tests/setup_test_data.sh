#!/bin/bash
#
# setup_test_data.sh - Copy test images from old assets
#
# Run this once to populate tests/data/ with sample images.
#

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

SRC_DIR="$PROJECT_ROOT/before_resturcture/core/assets/images"
DST_DIR="$PROJECT_ROOT/tests/data/screenshots"

echo "Setting up test data..."
echo "Source: $SRC_DIR"
echo "Destination: $DST_DIR"

# Create destination directory if it doesn't exist
mkdir -p "$DST_DIR"

# Copy a few representative game screenshots
IMAGES=(
    "game_1.png"
    "game_10.png"
    "game_20.png"
    "game_30.png"
    "game_44.png"
)

for img in "${IMAGES[@]}"; do
    if [ -f "$SRC_DIR/$img" ]; then
        cp "$SRC_DIR/$img" "$DST_DIR/"
        echo "  Copied: $img"
    else
        echo "  Not found: $img"
    fi
done

echo ""
echo "Test data setup complete."
echo "Files in $DST_DIR:"
ls -la "$DST_DIR" 2>/dev/null || echo "  (directory empty)"

