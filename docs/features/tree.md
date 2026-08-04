---
outline: deep
---

# Tree

`fima tree` is a reimplementation of `tree`.

## Usage

`fima [PATH] tree [OPTIONS]`

**Options:**

- `-a, --all` Show dotfiles
- `-G, --no-gitignore` Ignore .gitignore
- `-v, --verbose` Verbose output

## Examples

```sh
# Simple tree
fima tree

# Tree of a different directory
fima directory tree

# All options tree
fima tree -avG
```

## Showcase

<Showcase src="/tree_showcase_1.png" />

---

<Showcase src="/tree_showcase_2.png" />
