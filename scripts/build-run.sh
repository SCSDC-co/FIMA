#!/usr/bin/env bash

set -euo pipefail

./get-source-files.sh
./get-cli11-latest.sh

cat ./utils/ascii-art.txt

dir=$(basename "$(pwd)")

if [ "$dir" = "scripts" ]; then
    [ ! -d ../build ] && mkdir ../build

    cd ../build || exit 1
fi

build() {
    cmake --preset default .. || {
        echo "CMake configuration failed"
        exit 1
    }

    ninja || {
        echo "Build failed"
        exit 1
    }
}

run() {
    if [ ! -f ./fima ]; then
        echo "Binary not found! Building first..."
        build
    fi

    ./fima
}

PS3="Please enter your choice: "
options=("Build" "Run" "Build and Run" "Quit")

select opt in "${options[@]}"; do
    case $opt in
    "Build")
        build
        break
        ;;
    "Run")
        run
        break
        ;;
    "Build and Run")
        build
        run
        break
        ;;
    "Quit")
        break
        ;;
    *)
        echo "invalid option $REPLY"
        ;;
    esac
done
