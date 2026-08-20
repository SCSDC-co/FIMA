---
outline: deep
---

# Zip

`fima zip` è un semplice comando che aggiunge/zippa file dentro archivi `.zip`.

## Utilizzo

`fima zip PERCORSI -o OUTPUT`

**Opzioni:**

- `-o, --output TEXT OBBLIGATORIO` L'archivio di output

## Esempi

```sh
# Creare un archivio
fima zip directory file.txt -o archive.zip

# Aggiungere un file a un archivio esistente
fima zip new_file.txt -o archive.zip
```
