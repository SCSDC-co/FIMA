#!/usr/bin/env bash

# This is a script for getting all the source files of FIMA

touch ./cmake/files.cmake

cd ..

FILES=$(find src/ -type f)

cat <<EOF >./scripts/cmake/files.cmake
set(SOURCE_FILES
$FILES
)
EOF
