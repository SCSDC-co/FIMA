<div align=center>

# FIMA

</div>

FIMA (File Manager) is a simple and fast file manager, written in C++.

## Specs

| Spec | Purpose |
|-|-|
| C++ | Main language |
| CMake | Build system configuration generator |
| Make | Build system |
| FTXUI | TUI library |


## Scripts

There is a script directory, where there are all the scripts are located.

These are the most important ones (so far):
- `scripts/get-source-files.sh`
  - This is the script used to get all the `.cpp` files in the `src/` directory

- `scripts/build-run.sh`
  - This is the script used to build, run or both the program

> [!IMPORTANT]
> All the scripts should be run in the `scripts/` directory, so make sure to cd into it

