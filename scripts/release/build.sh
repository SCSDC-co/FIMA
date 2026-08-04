#!/usr/bin/env bash

# This script is used for building fima

set -eo pipefail

echo

cd "$(dirname "$0")" || exit 1

[ -d ../../build ] && rm -r ../../build/

mkdir ../../build

cd ../../build || exit 1

conan install .. --output-folder=. --build=missing -s compiler.cppstd=23 -s build_type=Release || {
    echo "Conan setup failed"
    exit 1
}

cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DFIMA_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release || {
    echo "CMake configuration failed"
    exit 1
}

cmake --build . -- -j "$(nproc)" || {
    echo "Build failed"
    exit 1
}

ctest -j "$(nproc)" --output-on-failure

echo
