#!/usr/bin/env bash

# This script is used for building fima with tests

set -eo pipefail

echo

cd "$(dirname "$0")" || exit 1

[ ! -d ../../build ] && mkdir ../../build

cd ../../build || exit 1

conan install .. --output-folder=. --build=missing -s compiler.cppstd=23 --settings=build_type=Debug || {
    echo "Conan setup failed"
    exit 1
}

cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DFIMA_BUILD_TESTS=ON -DFIMA_CODE_COVERAGE=ON || {
    echo "CMake configuration failed"
    exit 1
}

cmake --build . --config Debug -- -j "$(nproc)" || {
    echo "Build failed"
    exit 1
}

echo

ctest -j "$(nproc)" --output-on-failure -T Test -T Coverage
