---
outline: deep
---

# MK

`fima mk` is a command that creates files and/or directories,
it's a combination of `touch` and `mkdir`.

## Usage

`fima mk -d [DIRECTORIES] -f [FILES]`

**Options:**

- `-f, --file TEXT ...` Files to create
- `-d, --dir TEXT ...` Directories to create

## Examples

```sh
# Make a file
fima mk -f file.txt

# Make a directory
fima mk -d directory

# Make a directory and a file
fima mk -d directory -f file.txt

# Make a directory and a file inside it
fima mk -d directory -f directory/file.txt

# Make multiple files and multiple directories
fima mk -d directory1 directory2 directory3 -f directory1/file.txt directory2/file.txt file.txt
```
