---
outline: deep
---

# Get Started

Let's install FIMA!

## Why FIMA?

Before installing FIMA we want to tell you why you _should_.

Pros:

- **simple**: FIMA is a simple tool, with simple commands
  and it tries to be as much as user-friendly as possible.
- **customizable**: FIMA is customizable and configurable,
  you don't adapt your workflow to FIMA's one, FIMA adapts
  to your workflow.
- **cross platform**: FIMA supports Linux, Windows and MacOS.
- **self-contained**: FIMA is distributed as a single binary and does not
  require installing multiple external tools.
- **single binary**: FIMA provides all of its features in
  a single binary, while other workflows may require
  installing and managing multiple programs.

Cons:

- **slow with big amounts of data**: when you pass to FIMA
  medium to big amounts of data it may become slower.
- **early stage of development**: FIMA is in its early stages
  of development so it has some bugs.

---

There are really great alternatives to FIMA:

- [`ranger`](https://github.com/ranger/ranger)
- [`superfile`](https://github.com/yorukot/superfile)
- [`yazi`](https://github.com/sxyazi/yazi)
- [`nnn`](https://github.com/jarun/nnn)
- [`scc`](https://github.com/boyter/scc)
- [`eza`](https://github.com/eza-community/eza)

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
