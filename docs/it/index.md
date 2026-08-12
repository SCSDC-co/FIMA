---
# https://vitepress.dev/reference/default-theme-home-page
layout: home

hero:
  name: "FIMA"
  text: "Veloce, Incredibile, Minimale e Fantastico File Manager"
  tagline: Pensato per essere veloce e facile da usare
  actions:
    - theme: brand
      text: Per Iniziare
      link: /it/per-iniziare
    - theme: alt
      text: Funzionalità
      link: /it/funzionalita
  image:
    src: /FIMA.svg
    alt: Logo di FIMA

features:
  - title: Cassetta degli attrezzi del filesystem
    details: FIMA è più di un semplice file manager. Fornisce una cassetta degli attrezzi completa per il filesystem, pensata per sviluppatori, utenti esperti e utenti quotidiani
    icon: 📁
  - title: Personalizzabile
    details: Personalizza ogni aspetto di FIMA, dalle icone ai temi, fino alle impostazioni specifiche degli strumenti
    icon: ⚙
  - title: Open source
    details: FIMA è completamente open source e distribuito con licenza GPL 3.0
    link: https://github.com/SCSDC-co/FIMA/
    linkText: Guarda il repository GitHub
---

<br>

::: warning
FIMA è ancora in una fase iniziale di sviluppo, quindi potrebbe avere alcuni bug
:::

<Contributors heading="CONTRIBUTORI" :contributors="$contributors" />
