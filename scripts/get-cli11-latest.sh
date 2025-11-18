#!/bin/bash

# This is a script used for downloading the last version of CLI11,
# the library used for CLI parsing

OWNER="CLIUtils"
REPO="CLI11"

OUTPUT_DIR="../include/cli/CLI11.hpp"

API_URL="https://api.github.com/repos/$OWNER/$REPO/releases/latest"

FILE_URL=$(curl -s $API_URL | jq -r '.assets[] | select(.name=="CLI11.hpp") | .browser_download_url')

curl -sSL "$FILE_URL" -o "$OUTPUT_DIR"

echo ""

echo "Downloaded CLI11.hpp from latest release!"
echo "Output: $OUTPUT_DIR"
