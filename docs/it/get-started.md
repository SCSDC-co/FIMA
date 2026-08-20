---
outline: deep
---

# Per Iniziare

Installiamo FIMA!

## Perché FIMA?

Prima di installare FIMA vogliamo dirti perché _dovresti_.

Pro:

- **semplice**: FIMA è uno strumento semplice, con comandi semplici,
  e cerca di essere intuitivo il più possibile.
- **personalizzabile**: FIMA è personalizzabile e configurabile,
  non sei tu ad adattare il tuo flusso di lavoro a quello di FIMA,
  è FIMA ad adattarsi al tuo.
- **multipiattaforma**: FIMA supporta Linux, Windows e MacOS.
- **auto-contenuto**: FIMA è distribuito come un singolo binario e non
  richiede l'installazione di più strumenti esterni.
- **binario unico**: FIMA offre tutte le sue funzionalità in
  un unico binario, mentre altri flussi di lavoro potrebbero richiedere
  l'installazione e la gestione di più programmi.

Contro:

- **lento con grandi quantità di dati**: quando passi a FIMA
  quantità di dati medio-grandi, potrebbe diventare più lento.
- **fase iniziale dello sviluppo**: FIMA è nelle prime fasi
  dello sviluppo, quindi ha alcuni bug.

---

Esistono delle ottime alternative a FIMA:

- [`ranger`](https://github.com/ranger/ranger)
- [`superfile`](https://github.com/yorukot/superfile)
- [`yazi`](https://github.com/sxyazi/yazi)
- [`nnn`](https://github.com/jarun/nnn)
- [`scc`](https://github.com/boyter/scc)
- [`eza`](https://github.com/eza-community/eza)

## Requisiti

FIMA non ha dipendenze esterne, ma per la migliore esperienza consigliamo:

- Un [font Nerd](https://www.nerdfonts.com/#home) per visualizzare correttamente le icone
- Un emulatore di terminale con supporto True Color per personalizzare completamente la combinazione di colori di FIMA

Sono del tutto opzionali, ma rendono l'esperienza molto migliore

## Installazione

::: code-group

```sh [build dai sorgenti]
git clone --depth 1 https://github.com/SCSDC-co/FIMA

cd FIMA

./scripts/release/build.sh
# oppure se usi powershell
./scripts/release/build.ps1
```

:::

Lo script di build installa automaticamente le dipendenze e compila il progetto.

Strumenti necessari:

- Conan (gestione delle dipendenze)
- CMake (generazione del sistema di build)
- Ninja o Make (compilazione del progetto)

Il binario generato sarà disponibile in `build/fima`.

::: info
Stiamo distribuendo gradualmente l'app su tutti i principali gestori di pacchetti.
Se non vedi il tuo gestore qui, probabilmente arriverà presto!
:::

## Verifica dell'installazione

```sh
fima --version
```
