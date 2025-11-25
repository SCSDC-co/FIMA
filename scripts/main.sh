#!/usr/bin/env bash

###########
# THIS IS THE MAIN SCRIPT OF FIMA
# This script is used for everything
###########

export GUM_CHOOSE_HEADER_FOREGROUND="#76946A"
export GUM_CHOOSE_CURSOR_FOREGROUND="#76946A"
export GUM_CHOOSE_ITEM_FOREGROUND="#2A2A2A"

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
        format
        echo
        exit 0
        ;;
    "-h" | "--help")
        help
        ;;
    *)
        echo
        echo -e "\033[31mInvalid option:\033[0m $1"
        help
        ;;
    esac
fi

if command -v gum >/dev/null 2>&1; then
    SELECTION=$(
        gum choose \
            --header "What do you want to do?" \
            --height=7 \
            "Build" "Run" "Build and run" "Format" "All" "Quit" "Help"
    )

    case "$SELECTION" in
    "Build")
        echo

        get-files-cli11-latest
        build
        ;;
    "Run")
        echo

        run
        ;;
    "Build and Run")
        echo

        get-files-cli11-latest
        build
        run
        ;;
    "Format")
        echo

        format
        ;;
    "All")
        echo

        format
        get-files-cli11-latest
        build
        run
        ;;
    "Quit")
        exit 0
        ;;
    "Help")
        help
        ;;
    esac
else
    echo

    PS3="Please enter your choice: "
    options=("Build" "Run" "Build and Run" "Format" "All" "Quit" "Help")

    select opt in "${options[@]}"; do
        case $opt in
        "Build")
            echo

            get-files-cli11-latest
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

            get-files-cli11-latest
            build
            run

            break
            ;;
        "Format")
            echo

            format

            echo
            break
            ;;
        "All")
            echo

            format
            get-files-cli11-latest
            build
            run

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
fi

echo
