---
outline: deep
---

# Trash

`fima trash` è un sottocomando usato per gestire il cestino.

## Utilizzo

`fima trash SOTTOCOMANDO [OPZIONI]`

**Sottocomandi:**

- `list` Elenca tutti i file nel cestino
- `empty` Svuota il cestino
- `restore ID` Ripristina un elemento dato un ID (supporta più ID)
- `remove ID` Rimuove un elemento dato un ID (supporta più ID)

**Opzioni di list:**

- `-p, --plain` Output semplice

**Opzioni di empty:**

- `-y, --yes` Non chiedere conferma

## Esempi

```sh
# metti alcuni elementi nel cestino
fima rm file.txt file2.txt file3.txt file4.txt file5.txt

# elenca il contenuto del cestino
fima trash list

# ripristina un file
fima trash restore ID1 ID2

# rimuovi un file
fima trash remove ID1 ID2

# svuota il cestino
fima trash empty -y
```

## Anteprima

<Showcase src="/trash_list_showcase.png" />
