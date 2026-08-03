---
outline: deep
---

# RM

`fima rm` is a recreation of `rm` that support glob.

## Usage

`fima rm PATHS [OPTIONS]`

**Opions:**

- `-r, --recursive` Remove directories and their contents recursively

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
