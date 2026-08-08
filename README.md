<div align=center>

# FIMA

[Documentation](https://scsdc-co.github.io/FIMA/)

<img src="https://skillicons.dev/icons?i=cpp,cmake" >

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

If the file doesn't exists it will create it.

<details>
<summary>Default config:</summary>

You can dump it using `--dump-default-config`.

```toml
depth = 8
process_directory_size = false

[ls]
icons = false
all = false
one-line = false
group-directories-first = false
group-directories-last = false
long = false
verbose = false
headers = false

[tree]
all = false
no-gitignore = false
verbose = false

[rm]
recursive = false

[cloc]
ignore = [ "" ]
sort = "total"
quiet = false
no-gitignore = false

[info]
verbose = false
tags = false
remotes = false
```

</details>

### Icons

The icons can be customized.

You customize them in the config file like this:

```toml
[icons.files]
# <extension without the ., it can be a custom one> = "<icon>"

[icons.dirs]
# <name of the dir> = "<icon>"

[icons.name]
# <name in quotes (support glob, all the comparisons are case-insensitive)> = "<icon>"
```

**Example**:

```toml
[icons.files]
cpp = "󰙲"

[icons.dirs]
custom_name = ""

[icons.name]
"*cat*" = "" # *cat* converts to ^.*cat.*$
```

### Theme

The FIMA's colors can be changed how you want.

The theme file is located here:

- linux: `$HOME/.config/fima/theme.toml`
- windows: `$APPDATA/fima/theme.toml`

All the colors can be written in 3 different formats:

- HEX string (`#76946a`)
- RGB array (`[118, 148, 106]`)
- Color name (`green`)

<details>
<summary>Available color names:</summary>

- `black`
- `dark grey`
- `light grey`
- `dark gray`
- `light gray`
- `white`
- `blue`
- `light blue`
- `cyan`
- `light cyan`
- `green`
- `light green`
- `magenta`
- `light magenta`
- `red`
- `light red`
- `yellow`
- `light yellow`

</details>

<details>
<summary>Default theme:</summary>

You can dump it using `--dump-default-theme`.

```toml
[general]
primary = "green"
secondary = "white"
border = "green"
info = "green"
warning = "yellow"
error = "red"

[specific]
directory = "green"
executable = "red"
symlink = "blue"
archive = "blue"
media = "yellow"
normal_file = "white"

[permissions]
read = "green"
write = "yellow"
execute = "red"
null = "light gray"

[ls]
permissions = "yellow"
size = "green"
user = "red"
date_modified = "blue"
name = "green"
```

</details>

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
- [x] add theming features
- [x] create docs website using `vitepress`
- [x] add trash features

See [CONTRIBUTING.md](./CONTRIBUTING.md) if you want to contribute!

## License

This product is under the GPL 3.0 license, see [LICENSE](./LICENSE) for more details.
