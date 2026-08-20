---
outline: deep
---

# Configurazione e personalizzazione

FIMA cerca di essere il più personalizzabile e configurabile possibile

## Configurazione

In FIMA puoi configurare molte opzioni tramite il file di configurazione.

Il file di configurazione si trova qui:

- Linux: `$HOME/.config/fima/config.toml`
- Windows: `$APPDATA/fima/config.toml`

Puoi stampare l'intera configurazione predefinita usando `--dump-default-config`.

## Personalizzazione

Puoi personalizzare FIMA usando un file di tema oppure
modificando le icone nel file di configurazione.

**Temi:**

Il file di tema si trova qui:

- Linux: `$HOME/.config/fima/theme.toml`
- Windows: `$APPDATA/fima/theme.toml`

I colori possono essere specificati in uno dei seguenti formati:

- stringa HEX (`#76946a`)
- array RGB (`[118, 148, 106]`)
- nome del colore (`green`)

<details>
<summary>Nomi dei colori disponibili:</summary>

- `black`
- `dark grey`
- `light grey`
- `dark gray`
- `light gray`
- `white`
- `blue`
- `light blue`
- `cyan`
- `light cyan`
- `green`
- `light green`
- `magenta`
- `light magenta`
- `red`
- `light red`
- `yellow`
- `light yellow`

</details>

Puoi stampare il tema predefinito completo usando `--dump-default-theme`.

**Icone:**

Le icone possono essere personalizzate nel file di configurazione in questo modo:

```toml
[icons.files]
# <estensione senza il punto, può essere anche personalizzata> = "<icona>"

[icons.dirs]
# <nome della directory> = "<icona>"

[icons.name]
# <nome tra virgolette (supporta i glob, i confronti non distinguono maiuscole e minuscole)> = "<icona>"
```

**Esempio:**

```toml
[icons.files]
cpp = "󰙲"

[icons.dirs]
custom_name = ""

[icons.name]
"*cat*" = "" # Corrisponde a ogni nome di file contenente "cat"
```
