---
outline: deep
---

# Trash Bin

FIMA has a builtin trash bin.

It is located at:

- Linux: `$HOME/.local/share/fima/trash/`
- Windows: `$LOCALAPPDATA/fima/trash/`

## How does it work?

When a file is deleted with `rm` without the `-t, --no-trash`
it moves the file here: `TRASH_DIRECTORY/files/` and it gives it a new name
following these rules: `ID_<original file name>`.

`ID` looks like this: `YYYYMMDD_HHMMSS_COUNTER`.
`COUNTER` is used for avoiding `ID`s duplication.

So if we delete a file named `file.txt` its name in
the trash bin can look like this:
`20260806_195324_0_file.txt`.

::: info
Directories are handled the same way as files
:::

---

After the file is moved a `.trashinfo` is created here: `TRASH_DIRECTORY/trashinfo/`.

A `.trashinfo` file complete name is this: `<trash file name>.trashinfo`,
so the name of the `.trashinfo` file for our `file.txt` will be this:
`20260806_195324_0_file.txt.trashinfo`.

The contents of it looks like this:

```toml
[metadata]
deleted_at = 2026-08-06T19:53:24
path = '/home/giuliano/Programmazione/Cpp/FIMA/file.txt'
```

`path` is the original path of the file.

## How to manage it?

For removing files you use `rm` without the `-t, --no-trash` flag.

FIMA has the [`trash`](subcommands/trash.md) subcommand to:

- `list` all the files.
- `restore` and `remove` files.
- `empty` the trash.
