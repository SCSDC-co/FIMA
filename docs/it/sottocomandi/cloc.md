---
outline: deep
---

# CLOC

`fima cloc` è un'utilità per ottenere le LOC (Lines Of Code, righe di codice) di un programma.

## Utilizzo

`fima cloc [PATHS] [OPTIONS]`

**Opzioni:**

- `-i, --ignore TEXT ...`  Percorsi da ignorare
- `-S, --sort TEXT`  Tipo di ordinamento (total (predefinito), files, code, comments, blank)
- `-q, --quiet`  Abilita l'output silenzioso
- `-G, --no-gitignore`  Ignora il .gitignore
- `-s, --show-languages`  Mostra tutti i linguaggi supportati da cloc

## Esempi

```sh
# CLOC di base
fima cloc

# CLOC silenzioso
fima cloc -q

# Ignorare file e ordinamento diverso
fima cloc -i "*.h" -S files

# Ignora le regole del .gitignore
fima cloc -G
```

## Anteprime

<Showcase src="/cloc_showcase_1.png" />

---

<Showcase src="/cloc_showcase_2.png" />

---

<Showcase src="/cloc_showcase_3.png" />
