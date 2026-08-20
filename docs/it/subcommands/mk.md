---
outline: deep
---

# MK

`fima mk` è un comando che crea file e/o directory,
è una combinazione di `touch` e `mkdir`.

## Utilizzo

`fima mk -d [DIRECTORIES] -f [FILES]`

**Opzioni:**

- `-f, --file TEXT ...` File da creare
- `-d, --dir TEXT ...` Directory da creare

## Esempi

```sh
# Creare un file
fima mk -f file.txt

# Creare una directory
fima mk -d directory

# Creare una directory e un file
fima mk -d directory -f file.txt

# Creare una directory e un file al suo interno
fima mk -d directory -f directory/file.txt

# Creare più file e più directory
fima mk -d directory1 directory2 directory3 -f directory1/file.txt directory2/file.txt file.txt
```
