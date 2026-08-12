---
outline: deep
---

# RM

`fima rm` is a reimplementation of `rm` that support glob.

## Usage

`fima rm PATHS [OPTIONS]`

**Options:**

- `-r, --recursive` Remove directories and their contents recursively
- `-v, --verbose` Outputs the names of the removed items
- `-t, --no-trash` Remove the item directly

## Examples

```sh
# Remove a file
fima rm file.txt

# Remove an empty directory
fima rm empty_directory

# Remove a directory with content in it
fima rm -r directory

# You can't remove the root or any path that contains the current directory
fima rm /
fima rm ..
```
