---
outline: deep
---

# LS

`fima ls` is a modern reimplementation of `ls`
with additional features and customization options.

## Usage

`fima ls [PATHS] [OPTIONS]`

**Options:**

- `-i, --icons` Put an icon next to the name of the item
- `-a, --all` Will also show the hidden files
- `-1, --one-line` Display one entry per line
- `--group-directories-first` List directories before files
- `--group-directories-last` List directories after files
- `-l, --long` Display the file metadata

**Long output options:**

- `-v, --verbose` Display the number of directories and files
- `-H, --headers` Add a header to each column

## Examples

```sh
# Basic listing
fima ls

# Long listing with icons
fima ls -li

# Show hidden files and group directories first
fima ls -lia --group-directories-first

# Multiple paths and glob patterns
fima ls path/ file.txt "**/glob[.txt]*"
```

## Showcase

<Showcase src="/ls_showcase_1.png" />

---

<Showcase src="/ls_showcase_2.png" />

---

<Showcase src="/ls_showcase_3.png" />
