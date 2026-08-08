---
outline: deep
---

# Info

`fima info` is a useful command to get info from a directory or a file or a git repository.

## Usage

`fima [DIRECTORY PATH] info [FILE PATH] [OPTIONS]`

**Options:**

- `-g, --git` Display git information instead of file/directory

**Directory output options:**

- `-v, --verbose` Display additional information

**Git output options:**

- `-t, --tags` Show all the repo tags
- `-r, --remotes` Show all the repo remotes

## Examples

```sh
# Get info of the current directory
fima info

# Get info from a file
fima info file.txt

# Get info of a git repository
fima ingo -g
```

## Showcase

<Showcase src="/info_showcase_1.png" />

---

<Showcase src="/info_showcase_2.png" />

---

<Showcase src="/info_showcase_3.png" />
