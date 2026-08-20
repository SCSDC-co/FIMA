#!/usr/bin/env bash

# This script is used for building fima

set -eo pipefail

echo

cd "$(dirname "$0")" || exit 1

[ -d ../../build ] && rm -rf ../../build/

mkdir ../../build

cd ../../build || exit 1

conan install .. --output-folder=. --build=missing -s compiler.cppstd=23 -s build_type=Release --lockfile-out=../conan.lock || {
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

echo

sudo cmake --install . || {
    echo "Install failed"
    exit 1
}

echo

ctest --output-on-failure

echo
