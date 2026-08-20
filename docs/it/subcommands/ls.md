---
outline: deep
---

# LS

`fima ls` è una reimplementazione moderna di `ls`
con funzionalità aggiuntive e opzioni di personalizzazione.

## Utilizzo

`fima ls [PATHS] [OPTIONS]`

**Opzioni:**

- `-i, --icons` Mette un'icona accanto al nome dell'elemento
- `-a, --all` Mostra anche i file nascosti
- `-1, --one-line` Mostra una voce per riga
- `--group-directories-first` Elenca le directory prima dei file
- `--group-directories-last` Elenca le directory dopo i file
- `-l, --long` Mostra i metadati del file

**Opzioni di output esteso:**

- `-v, --verbose` Mostra il numero di directory e file
- `-H, --headers` Aggiunge un'intestazione a ogni colonna

## Esempi

```sh
# Elenco di base
fima ls

# Elenco dettagliato con icone
fima ls -li

# Mostra i file nascosti e raggruppa le directory per prime
fima ls -lia --group-directories-first

# Più percorsi e pattern glob
fima ls path/ file.txt "**/glob[.txt]*"
```

## Anteprime

<Showcase src="/ls_showcase_1.png" />

---

<Showcase src="/ls_showcase_2.png" />

---

<Showcase src="/ls_showcase_3.png" />
