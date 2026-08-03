---
outline: deep
---

# Get Started

Let's install FIMA!

## Requirements

FIMA has no external dependencies, but we recommend the following for the best experience:

- A [nerd font](https://www.nerdfonts.com/#home) for showing correctly the icons
- A terminal emulator with True Color support to fully customize FIMA's color scheme

They are completely optional, but they make the experience much better

## Installation

::: code-group

```sh [build from source]
git clone --depth 1 https://github.com/SCSDC-co/FIMA

cd FIMA

./scripts/release/build.sh
# or if you use PowerShell:
./scripts/release/build.ps1
```

:::

The build script automatically installs dependencies and builds the project.

Tools needed:

- Conan (dependency management)
- CMake (build system generation)
- Ninja or Make (project compilation)

The generated binary will be available in `build/fima`.

::: info
We are slowly deploying the app to all the major package managers.
If you don't see your package manager here, it's probably coming!
:::

## Verify the installation

```sh
fima --version
```
