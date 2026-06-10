#!/usr/bin/env bash

###########
# This script is used for testing fima
###########

set -eo pipefail

./build/fima version

echo
echo "Fima ls:"

./build/fima ls

echo
echo "Fima info:"

./build/fima info

echo
echo "Fima cloc:"

./build/fima cloc

echo
echo "Fima help:"

./build/fima --help
