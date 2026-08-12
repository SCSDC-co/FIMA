---
outline: deep
---

# Trash

`fima trash` is a subcommand used to manage the trash.

## Usage

`fima trash SUBCOMMAND [OPTIONS]`

**Subcommands:**

- `list` Lists all the files in the trash
- `empty` Empties the trash
- `restore IDs` Restore an item given an ID (supports multiple IDs)
- `remove IDs` Remove an item given an ID (supports multiple IDs)

**List Options:**

- `-p, --plain` Plain output

**Empty Options:**

- `-y, --yes` Don't ask for confirmation

## Examples

```sh
# put some items in the trash
fima rm file.txt file2.txt file3.txt file4.txt file5.txt

# lists the content of the trash
fima trash list

# restore a file
fima trash restore ID1 ID2

# remove a file
fima trash remove ID1 ID2

# empties the trash
fima trash empty -y
```

## Showcase

<Showcase src="/trash_list_showcase.png" />
