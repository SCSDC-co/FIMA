---
# https://vitepress.dev/reference/default-theme-home-page
layout: home

hero:
  name: "FIMA"
  text: "Fast, Incredible, Minimal & Awesome File Manager"
  tagline: Aimed to be fast and easy to use
  actions:
    - theme: brand
      text: Get Started
      link: /get-started
    - theme: alt
      text: Features
      link: /features

features:
  - title: Filesystem Toolbox
    details: FIMA is more than a file manager. It provides a complete filesystem toolbox for developers, power users, and everyday users
    icon: 📁
  - title: Customizable
    details: Customize every aspect of FIMA, from icons and themes to tool-specific settings
    icon: ⚙
  - title: Open source
    details: FIMA is fully open source and licensed under the GPL 3.0 license
    link: https://github.com/SCSDC-co/FIMA/tree/main/LICENSE
    linkText: View the license
---

<br>

::: warning
FIMA is in its early stages of development so it may have some bugs
:::

<Contributors heading="CONTRIBUTORS" :contributors="$contributors" />
