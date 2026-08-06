---
outline: deep
prev: false
---

# Configuration and Customization

FIMA includes a wide range of filesystem utilities,
each designed for a specific workflow.

## Configuration

In FIMA you can configure a lot of options with the config file.

The config file is located here:

- Linux: `$HOME/.config/fima/config.toml`
- Windows: `$APPDATA/fima/config.toml`

You can dump the full default config using `--dump-default-config`.

## Customization

You can customize FIMA by using a theme file or
by changing the icons in the config file.

**Theming:**

The theme file is located here:

- Linux: `$HOME/.config/fima/theme.toml`
- Windows: `$APPDATA/fima/theme.toml`

Colors can be specified in one of the following formats:

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

You can dump the full default theme using `--dump-default-theme`.

**Icons:**

The icons can be customized in the config file like this:

```toml
[icons.files]
# <extension without the ., it can be a custom one> = "<icon>"

[icons.dirs]
# <name of the dir> = "<icon>"

[icons.name]
# <name in quotes (support glob, all the comparisons are case-insensitive)> = "<icon>"
```

**Example:**

```toml
[icons.files]
cpp = "󰙲"

[icons.dirs]
custom_name = ""

[icons.name]
"*cat*" = "" # Matches every filename containing "cat"
```
