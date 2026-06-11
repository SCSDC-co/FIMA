#!/usr/bin/env bash

# This script is used for building fima

set -eo pipefail

echo

cd "$(dirname "$0")" || exit 1

awk '{print "\033[32m" $0 "\033[0m"}' utils/ascii-art.txt

[ ! -d ../build ] && mkdir ../build

cd ../build || exit 1

conan install .. --output-folder=. --build=missing -s compiler.cppstd=23 || {
    echo "Conan setup failed"
    exit 1
}

if [ "$1" == "tests" ]; then
    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DFIMA_BUILD_TESTS=True || {
        echo "CMake configuration failed"
        exit 1
    }
else
    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release || {
        echo "CMake configuration failed"
        exit 1
    }
fi

cmake --build . || {
    echo "Build failed"
    exit 1
}

echo
