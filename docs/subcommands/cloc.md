---
outline: deep
---

# CLOC

`fima cloc` is a utility for getting the LOC (Lines Of Code) of a program.

## Usage

`fima cloc [PATHS] [OPTIONS]`

**options:**

- `-i, --ignore TEXT ...`  Paths to ignore
- `-S, --sort TEXT`  Type of sorting (total (default), files, code, comments, blank)
- `-q, --quiet`  Enables quiet output
- `-G, --no-gitignore`  Ignore .gitignore
- `-s, --show-languages`  Shows all the languages that cloc supports

## Examples

```sh
# Basic CLOC
fima cloc

# Quiet CLOC
fima cloc -q

# Ignoring files and different sorting
fima cloc -i "*.h" -S files

# Ignore .gitignore rules
fima cloc -G
```

## Showcase

<Showcase src="/cloc_showcase_1.png" />

---

<Showcase src="/cloc_showcase_2.png" />

---

<Showcase src="/cloc_showcase_3.png" />
