#!/usr/bin/env bash

set -eo pipefail

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

get-files-cli11-latest() {
    ../scripts/get-source-files.sh
    ../scripts/get-cli11-latest.sh
}

help() {
    echo "USAGE"
    echo "    $0 {1|2|3} | -h|--help"
    echo ""
    echo "OPTIONS"
    echo "    This is the script for running, build or both FIMA"
    echo ""
    echo "OPTIONS"
    echo "    1            build"
    echo "    2            run"
    echo "    3            build & run"
    echo "    -h|--help    print the help message"

    exit 0
}

if [ -z "$1" ]; then
    echo ""
else
    case "$1" in
    1)
        get-files-cli11-latest
        build
        echo ""
        exit 0
        ;;
    2)
        run
        exit 0
        ;;
    3)
        get-files-cli11-latest
        build
        run
        echo ""
        exit 0
        ;;
    "-h" | "--help")
        help
        ;;
    *)
        echo "Invalid option $1"
        help
        ;;
    esac
fi

get-files-cli11-latest

cat ../scripts/utils/ascii-art.txt

echo ""

PS3="Please enter your choice: "
options=("Build" "Run" "Build and Run" "Quit" "Help")

select opt in "${options[@]}"; do
    case $opt in
    "Build")
        echo ""
        build
        break
        ;;
    "Run")
        echo ""
        run
        break
        ;;
    "Build and Run")
        echo ""
        build
        run
        break
        ;;
    "Quit")
        exit 0
        ;;
    "Help")
        echo ""
        help
        ;;
    *)
        echo "invalid option $REPLY"
        help
        ;;
    esac
done

echo ""
