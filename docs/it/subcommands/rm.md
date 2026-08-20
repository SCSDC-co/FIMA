---
outline: deep
---

# RM

`fima rm` è una reimplementazione di `rm` con il supporto ai glob.

## Utilizzo

`fima rm PERCORSI [OPZIONI]`

**Opzioni:**

- `-r, --recursive` Rimuove le directory e il loro contenuto ricorsivamente
- `-v, --verbose` Stampa i nomi degli elementi rimossi
- `-t, --no-trash` Rimuove l'elemento direttamente

## Esempi

```sh
# Rimuovere un file
fima rm file.txt

# Rimuovere una directory vuota
fima rm empty_directory

# Rimuovere una directory con contenuto al suo interno
fima rm -r directory

# Non puoi rimuovere la root o qualsiasi percorso che contenga la directory corrente
fima rm /
fima rm ..
```
