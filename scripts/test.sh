#!/usr/bin/env bash

###########
# This script is used for testing fima
###########

set -eo pipefail

echo

cd "$(dirname "$0")" || exit 1

[ ! -d ../build ] && mkdir ../build

cd ../build || exit 1

echo "Fima version:"

./fima -v

echo "Fima ls:"

./fima ls

echo "Fima info:"

./fima info

echo "Fima cloc:"

./fima cloc

echo "Fima help:"

./fima --help
