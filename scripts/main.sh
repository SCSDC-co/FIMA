#!/usr/bin/env bash

###########
# THIS IS THE MAIN SCRIPT OF FIMA
# This script is used for everything
###########

set -eo pipefail

echo

cd "$(dirname "$0")" || exit 1

awk '{print "\033[32m" $0 "\033[0m"}' utils/ascii-art.txt

[ ! -d ../build ] && mkdir ../build

cd ../build || exit 1

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
    ../scripts/utils/get-source-files.sh >/dev/null
    ../scripts/utils/get-cli11-latest.sh >/dev/null
    echo "Updated sources + downloaded CLI11 latest"
}

format() {
    for file in $(find ../src/ -type f -name "*.cpp"); do
        clang-format -i "$file"

        echo -e "\033[32mFormatted:\033[0m $file"
    done
}

help() {
    echo
    echo -e "\033[32mUSAGE\033[0m"
    echo "    $0 [OPTIONS]"
    echo
    echo -e "\033[32mDESCRIPTION\033[0m"
    echo "    Build, run, format and manage the FIMA project from a single script."
    echo
    echo -e "\033[32mOPTIONS\033[0m"
    echo "    -b | --build         Build the project"
    echo "    -r | --run           Run the executable"
    echo "    -br| --build-run     Build and then run"
    echo "    -f | --format        Format all source files"
    echo "    -a | --all           Build, format and run everything"
    echo "    -h | --help          Show this help message"
    echo

    exit 0
}

if [ -z "$1" ]; then
    echo
else
    case "$1" in
    "-b" | "--build")
        echo
        get-files-cli11-latest
        echo

        build

        echo
        exit 0
        ;;
    "-r" | "--run")
        run
        exit 0
        ;;
    "-br" | "--build-run")
        echo
        get-files-cli11-latest
        echo

        build
        run

        echo
        exit 0
        ;;
    "-f" | "--format")
        echo
        format
        echo
        exit 0
        ;;
    "-a" | "--all")
        echo
        build
        run
        format-lint
        echo
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

echo

PS3="Please enter your choice: "
options=("Build" "Run" "Build and Run" "Format" "All" "Quit" "Help")

select opt in "${options[@]}"; do
    case $opt in
    "Build")
        echo
        build
        break
        ;;
    "Run")
        echo
        run
        break
        ;;
    "Build and Run")
        echo
        build
        run
        break
        ;;
    "Format")
        echo
        format-lint
        echo
        break
        ;;
    "All")
        echo
        build
        run
        format-lint
        break
        ;;
    "Quit")
        exit 0
        ;;
    "Help")
        help
        ;;
    *)
        echo "invalid option $REPLY"
        help
        ;;
    esac
done

echo
