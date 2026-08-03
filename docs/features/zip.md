---
outline: deep
---

# Zip

`fima zip` is a simple command that adds/zips files inside `.zip` archives.

## Usage

`fima zip PATHS -o OUTPUT`

**Options:**

- `-o, --output TEXT REQUIRED` The output archive

## Examples

```sh
# Create an archive
fima zip directory file.txt -o archive.zip

# Add a file into an existing archive
fima zip new_file.txt -o archive.zip
```
