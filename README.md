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
```

## Road Map

- [x] add the following commands:
  - [x] `create`
  - [x] `remove`
  - [x] `copy`
  - [x] `rename`
  - [x] `permissions`
  - [x] `cloc`
  - [x] `info`
- [x] add config file support (TOML format)
- [x] add `zip` and `unzip` functionality
- [ ] version 0.1.0:
  - [ ] general improvement
  - [ ] polish
  - [ ] add verbose mode to `info`

See [CONTRIBUTING.md](./CONTRIBUTING.md) if you want to contribute!

## License

This product is under the GPL 3.0 license, see [LICENSE](./LICENSE) for more details.
