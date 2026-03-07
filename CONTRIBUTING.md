# Contributing to FIMA

First of all, thanks for contributing to FIMA!

## Where to notify bugs

If you find a bug or have a suggestion, just create an [issue](https://github.com/SCSDC-co/FIMA/issues).

## Environment setup

FIMA development dependencies are divided into two categories:

### Essential

These dependencies are required to build FIMA and must be installed on your system.

- `CMake` – build system generator
- `Ninja` – build system
- `Conan` – dependency manager

### Recommended

These dependencies are optional but recommended.

- `Bash` – required to run the scripts in `scripts/`
- `clang-format` – C++ code formatter

### Scripts

The `scripts/` directory contains helper scripts that are useful for building,
running, and formatting the FIMA source code.

> Run `./scripts/main.sh -h` to see all available commands.

These scripts also require some dependencies:

- `curl` – used to make requests to GitHub for the latest version of CLI11
- `jq` – used to parse the JSON response returned by GitHub

## Coding style

FIMA has a set of coding styles, make sure to follow them.

- Follow the style determined in the [clang-format config file](./.clang-format)
- Use **list initialization** to initialize variables
- Create a header to every source code file (`.cpp` and `.h`) using the following
  template:

```cpp
/*
 * <C++ file name>
 * <header file name>
 *
 * <short description of the file>
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */
```

- Use `.h` for FIMA headers and `.hpp` for headers from dependencies

## Code of conduct

By contributing to FIMA, you agree to follow the [Code of Conduct](/CODE_OF_CONDUCT.md).
