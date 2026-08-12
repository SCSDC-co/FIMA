---
outline: deep
---

# Tree

`fima tree` è una reimplementazione di `tree`.

## Utilizzo

`fima [PATH] tree [OPTIONS]`

**Opzioni:**

- `-a, --all` Mostra i file nascosti
- `-G, --no-gitignore` Ignora il .gitignore
- `-v, --verbose` Output dettagliato

## Esempi

```sh
# Albero semplice
fima tree

# Albero di un'altra directory
fima directory tree

# Albero con tutte le opzioni
fima tree -avG
```

## Anteprime

<Showcase src="/tree_showcase_1.png" />

---

<Showcase src="/tree_showcase_2.png" />
