---
outline: deep
---

# Info

`fima info` è un comando utile per ottenere informazioni da una directory, da un file o da un repository git.

## Utilizzo

`fima [DIRECTORY PATH] info [FILE PATH] [OPTIONS]`

**Opzioni:**

- `-g, --git` Mostra le informazioni git invece di quelle di file/directory

**Opzioni di output per le directory:**

- `-v, --verbose` Mostra informazioni aggiuntive

**Opzioni di output per git:**

- `-t, --tags` Mostra tutti i tag del repository
- `-r, --remotes` Mostra tutti i remote del repository

## Esempi

```sh
# Ottieni informazioni sulla directory corrente
fima info

# Ottieni informazioni da un file
fima info file.txt

# Ottieni informazioni su un repository git
fima info -g
```

## Anteprime

<Showcase src="/info_showcase_1.png" />

---

<Showcase src="/info_showcase_2.png" />

---

<Showcase src="/info_showcase_3.png" />
