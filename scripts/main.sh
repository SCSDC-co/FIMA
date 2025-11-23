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

format-lint() {
    ../scripts/utils/fix-compile_commands.sh >/dev/null

    for file in $(find ../src/ -type f -name "*.cpp"); do
        clang-tidy -p=compile_commands_clean.json "$file"

        echo "Linted: $file"

        clang-format "$file"

        echo "Formatted: $file"
    done
}

help() {
    echo
    echo -e "\033[32mUSAGE\033[0m"
    echo "    $0 {1|2|3|4|5} | -h|--help"
    echo
    echo -e "\033[32mDESCRIPTION\033[0m"
    echo "    This is the script for run, build, lint, format or do everything for FIMA"
    echo
    echo -e "\033[32mOPTIONS\033[0m"
    echo -e "    1           build"
    echo -e "    2           run"
    echo -e "    3           build & run"
    echo -e "    4           format & lint"
    echo -e "    5           does everything"
    echo -e "    -h|--help   print the help message"
    echo

    exit 0
}

if [ -z "$1" ]; then
    echo
else
    case "$1" in
    1)
        echo
        get-files-cli11-latest
        echo

        build

        echo
        exit 0
        ;;
    2)
        run
        exit 0
        ;;
    3)
        echo
        get-files-cli11-latest
        echo

        build
        run

        echo
        exit 0
        ;;
    4)
        echo
        format-lint
        echo
        exit 0
        ;;
    5)
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
options=("Build" "Run" "Build and Run" "Format and lint" "All" "Quit" "Help")

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
    "Format and lint")
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
