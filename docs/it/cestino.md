---
outline: deep
---

# Cestino

FIMA ha un cestino integrato.

Si trova in:

- Linux: `$HOME/.local/share/fima/trash/`
- Windows: `$LOCALAPPDATA/fima/trash/`

## Come funziona?

Quando un file viene eliminato con `rm` senza il flag `-t, --no-trash`
viene spostato qui: `TRASH_DIRECTORY/files/` e riceve un nuovo nome
seguendo questa regola: `ID_<nome originale del file>`.

`ID` ha questa forma: `YYYYMMDD_HHMMSS_COUNTER`.
`COUNTER` serve per evitare la duplicazione degli `ID`.

Quindi, se eliminiamo un file chiamato `file.txt`, il suo nome nel
cestino può apparire così:
`20260806_195324_0_file.txt`.

::: info
Le directory vengono gestite allo stesso modo dei file
:::

---

Dopo lo spostamento del file, viene creato un `.trashinfo` qui: `TRASH_DIRECTORY/trashinfo/`.

Il nome completo di un file `.trashinfo` è questo: `<nome del file nel cestino>.trashinfo`,
quindi il nome del file `.trashinfo` per il nostro `file.txt` sarà:
`20260806_195324_0_file.txt.trashinfo`.

Il suo contenuto appare così:

```toml
[metadata]
deleted_at = 2026-08-06T19:53:24
path = '/home/giuliano/Programmazione/Cpp/FIMA/file.txt'
```

`path` è il percorso originale del file.

## Come gestirlo?

Per rimuovere i file usi `rm` senza il flag `-t, --no-trash`.

FIMA ha il sottocomando [`trash`](sottocomandi/trash.md) per:

- `list` elencare tutti i file.
- `restore` e `remove` ripristinare e rimuovere i file.
- `empty` svuotare il cestino.
