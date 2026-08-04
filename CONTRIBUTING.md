# Contributing to FIMA

First of all, thanks for contributing to FIMA!

## Where to notify bugs

If you find a bug or have a suggestion, just create an [issue](https://github.com/SCSDC-co/FIMA/issues).

## Environment setup

FIMA development dependencies are divided into two categories:

### Essential

These dependencies are required to build FIMA and must be installed on your system.

- `CMake` – build system generator
- A build system
- `Conan` – dependency manager

### Recommended

These dependencies are optional but recommended.

- `Bash` or `PowerShell` – required to run the scripts in `scripts/`
- `clang-format` – C++ code formatter

### Scripts

There are helper scripts (located in `scripts/`) that help you build the program,
there are release and tests ones, the names are pretty much self explanatory

## Branches

There are 3 main branches:

- `main` this branch is protected and you can't directly push on it
- `dev` this branch is used for developing FIMA
- `website-docs` this branch is used for develop the website docs

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
- And maintain general consistency in the codebase

## Commit messages style

- Use the present tense ("Add feature" not "Added feature")
- Use the imperative mood ("Move cursor to..." not "Moves cursor to...")
- Limit the first line to 72 characters or less
- Use the correct naming conventions for commit messages:
  - `feat:` For adding features
  - `fix:` For fixing bugs
  - `docs:` For updating the docs
  - `chore:` When making changes that don't change the code behavior
  - `refactor:` When refactoring the code
  - `perf:` For changes that upgrade the performance
  - `ci:` When changing the CI/CD workflow
  - `style:` When changing the code style
  - `revert:` When reverting to the previous commit

## Branches name style

If you have the permission you can create a branch,
but make sure to follow this naming conventions:

- `feat/` For adding features
- `fix/` For fixing bugs
- `docs/` For updating the docs
- `chore/` When making changes that don't change the code behavior
- `refactor/` When refactoring the code
- `perf/` For changes that upgrade the performance
- `ci/` When changing the CI/CD workflow
- `style/` When changing the code style
- `revert/` When reverting to the previous commit

## Code of conduct

By contributing to FIMA, you agree to follow the [Code of Conduct](/CODE_OF_CONDUCT.md).
