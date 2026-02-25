<div align=center>

# FIMA

<img src="https://skillicons.dev/icons?i=bash,cpp,cmake" >

</div>

<br>

FIMA (File Manager) is a simple and fast file manager, written in C++.

> [!WARNING]
> This program is in its early stage of development, so there might be some bugs.

## Customization

### Mappings

All the mappings of FIMA are stored in a JSON format and thus they can be changed
to be however you want without changing the program source code.

The same applies for languages specifications.

They are located here:

- `FIMA_CONFIG_PATH/langauges_spec.json`
- `FIMA_CONFIG_PATH/mappings/directory_icon.json`
- `FIMA_CONFIG_PATH/mappings/language_family.json`
- `FIMA_CONFIG_PATH/mappings/language_name.json`
- `FIMA_CONFIG_PATH/mappings/language_icon.json`

### Config file

FIMA supports a config file in TOML format, it will be located here in
`FIMA_CONFIG_PATH/config.toml`.

If the file doesn't exists it will create it

Example file:

```toml
[ls]
icons = true
long = true
verbose = true

[cloc]
ignore = ["CLI11.hpp"]

[tree]
no-tui = true
```

## Specs

- **C++**: Main language
- **CMake**: Build system configuration generator
- **Ninja**: Build system
- **FTXUI**: TUI library
- **CLI11**: CLI parsing

## Scripts

There is a script directory, where there are all the scripts are located.

These are the most important ones (so far):

- `scripts/utils/get-source-files.sh`
  - This is the script used to get all the `.cpp` files in the `src/` directory

- `scripts/utils/get-cli11-latest.sh`
  - This is the script used the get the latest version of `cli11`

- `scripts/main.sh`
  - This is the script used to build, run, format or everything the program (supports arguments, and includes all the other scripts in it)

> [!IMPORTANT]
> All the scripts should be run in the `scripts/` directory, so make sure to cd into it, or if you run the main script you won't need to do that

## Development Requirements

**Main dependencies**:

- **CMake**: Build system configuration generator
- **Ninja**: Build System

**Other dependencies**:

- **Curl**: for downloading the latest version of CLI11
- **jq**: for parsing the json that github provides

## Road Map

- [ ] add the following commands:
  - [x] `create`
  - [x] `remove`
  - [x] `copy`
  - [x] `rename`
  - [x] `permissions`
  - [x] `cloc`
  - [x] `info`
- [x] add config file support (TOML format)

## License

This product is under the GPL 3.0 license, see [LICENSE](./LICENSE) for more details.
