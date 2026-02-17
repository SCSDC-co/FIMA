#!/usr/bin/env bash

# This is a script for getting all the source files of FIMA

touch ../scripts/cmake/files.cmake

cd ..

FILES=$(find src/ -type f | awk '{ print length, $0 }' | sort -nr | cut -d" " -f2-)

cat <<EOF >./scripts/cmake/files.cmake
add_library(fima_lib
$FILES
)
EOF

sed -i -e '/add_library(fima_lib/,/)/{
    /add_library(fima_lib/!{
        /)/! s/^/    /
    }
}' ./scripts/cmake/files.cmake

sed -i -e '/src\/main.cpp/d' ./scripts/cmake/files.cmake

echo ""
echo "Output: "
echo ""

cat ./scripts/cmake/files.cmake

echo ""
