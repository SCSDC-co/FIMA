<div align=center>

# FIMA

<img src="https://skillicons.dev/icons?i=bash,cpp,cmake" >

---

[![build and test the program](https://github.com/SCSDC-co/FIMA/actions/workflows/ci.yml/badge.svg)](https://github.com/SCSDC-co/FIMA/actions/workflows/ci.yml)
[![codecov](https://codecov.io/github/SCSDC-co/FIMA/graph/badge.svg?token=ZO0IOFUOJD)](https://codecov.io/github/SCSDC-co/FIMA)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/1a9d219ee673455a9a953d475843bdbb)](https://app.codacy.com/gh/SCSDC-co/FIMA/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)

</div>

<br>

FIMA (File Manager) is a simple and fast file manager, written in C++.

> [!WARNING]
> This program is in its early stage of development, so there might be some bugs.

## Customization

### Config file

FIMA supports a config file in TOML format, it's located here:

- linux: `$HOME/.config/fima/config.toml`
- windows: `$APPDATA/fima/config.toml`

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

### Icons

The icons can be customized.

You customize them in the config file like this:

```toml
[icons.files]
# <extension without the ., it can be a custom one> = "<icon>"

[icons.dirs]
# <name of the dir> = "<icon>"
```

**Exemple**:

```toml
[icons.files]
cpp = "󰙲"

[icons.dirs]
custom_name = ""
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
- [x] version 0.1.0:
  - [x] general improvement
  - [x] polish
  - [x] add verbose mode to `info`

See [CONTRIBUTING.md](./CONTRIBUTING.md) if you want to contribute!

## License

This product is under the GPL 3.0 license, see [LICENSE](./LICENSE) for more details.
