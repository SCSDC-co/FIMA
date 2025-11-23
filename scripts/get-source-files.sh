#!/usr/bin/env bash

# This is a script for getting all the source files of FIMA

touch ../scripts/cmake/files.cmake

cd ..

FILES=$(find src/ -type f | awk '{ print length, $0 }' | sort -nr | cut -d" " -f2-)

cat <<EOF >./scripts/cmake/files.cmake
set(SOURCE_FILES
$FILES
)
EOF

sed -i -e '/set(SOURCE_FILES/,/)/{
    /set(SOURCE_FILES/!{
        /)/! s/^/    /
    }
}' ./scripts/cmake/files.cmake

echo ""
echo "Output: "
echo ""

cat ./scripts/cmake/files.cmake

echo ""
