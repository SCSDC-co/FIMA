import { defineConfig, UserConfig } from "vitepress";
import { withSidebar } from "vitepress-sidebar";

const rootLocale = "en";
const supportedLocales = [rootLocale, "it"];

const vitePressOptions: UserConfig = {
    title: "FIMA",
    description: "Fast, Incredible, Minimal & Awesome File Manager",

    cleanUrls: true,
    appearance: "force-dark",
    lastUpdated: true,
    base: "/FIMA/",

    locales: {
        root: { label: "English", lang: "en-US" },
        it: {
            label: "Italiano",
            lang: "it-IT",
            description:
                "Veloce, Incredibile, Minimale e Fantastico File Manager",
            themeConfig: {
                nav: [
                    { text: "Pagina Iniziale", link: "/it/" },
                    { text: "Per iniziare", link: "/it/per-iniziare" },
                ],

                footer: {
                    message: "Distribuito con la licenza GPL 3.0.",
                    copyright: "Copyright © 2026-presente Giuliano De Amicis",
                },

                docFooter: {
                    prev: "Pagina precedente",
                    next: "Pagina successiva",
                },

                outline: {
                    label: "In questa pagina",
                },

                lastUpdated: {
                    text: "Ultimo aggiornamento",
                },

                returnToTopLabel: "Ritorna in cima",
            },
        },
    },

    themeConfig: {
        // https://vitepress.dev/reference/default-theme-config
        nav: [
            { text: "Home", link: "/" },
            { text: "Get Started", link: "/get-started" },
        ],

        logo: { src: "/F.svg", alt: "FIMA's F" },

        socialLinks: [
            { icon: "github", link: "https://github.com/SCSDC-co/FIMA" },
        ],

        search: {
            provider: "local",

            options: {
                locales: {
                    it: {
                        translations: {
                            button: {
                                buttonText: "Cerca",
                                buttonAriaLabel: "Cerca",
                            },
                            modal: {
                                displayDetails: "Mostra elenco dettagliato",
                                resetButtonTitle: "Reimposta ricerca",
                                backButtonTitle: "Chiudi ricerca",
                                noResultsText: "Nessun risultato per",
                                footer: {
                                    selectText: "per selezionare",
                                    selectKeyAriaLabel: "invio",
                                    navigateText: "per navigare",
                                    navigateUpKeyAriaLabel: "freccia su",
                                    navigateDownKeyAriaLabel: "freccia giù",
                                    closeText: "per chiudere",
                                    closeKeyAriaLabel: "esc",
                                },
                            },
                        },
                    },
                },
            },
        },

        editLink: {
            pattern: "https://github.com/SCSDC-co/fima/edit/main/docs/:path",
        },

        footer: {
            message: "Released under the GPL 3.0 License.",
            copyright: "Copyright © 2026-present Giuliano De Amicis",
        },

        externalLinkIcon: true,
    },

    head: [["link", { rel: "icon", href: "/FIMA/favicon.ico" }]],

    rewrites: {
        "en/:rest*": ":rest*",
    },

    transformHead(context) {
        return [
            [
                "meta",
                {
                    property: "og:title",
                    content: context.title,
                },
            ],
            [
                "meta",
                {
                    property: "og:description",
                    content: context.description,
                },
            ],
            [
                "meta",
                {
                    property: "og:image",
                    content: "https://scsdc-co.github.io/FIMA/banner.png",
                },
            ],
            [
                "meta",
                {
                    property: "og:type",
                    content: "website",
                },
            ],
            [
                "meta",
                {
                    property: "og:url",
                    content: `https://scsdc-co.github.io/FIMA/${context.pageData.relativePath.replace(/\.md$/, "")}`,
                },
            ],
            [
                "meta",
                {
                    name: "twitter:card",
                    content: "summary_large_image",
                },
            ],
            [
                "meta",
                {
                    property: "og:site_name",
                    content: "FIMA",
                },
            ],
        ];
    },
};

const commonSidebarConfigs = {
    rootGroupText: "Contents",
    documentRootPath: "docs/",
    capitalizeFirst: true,
    hyphenToSpace: true,
    useTitleFromFileHeading: true,
    excludeByGlobPattern: ["it"],
    manualSortFileNameByPriority: [
        "get-started.md",
        "features.md",
        "customization-and-configuration.md",
        "trash-bin.md",
        "per-iniziare.md",
        "funzionalita.md",
        "configurazione-e-personalizzazione.md",
        "cestino.md",
    ],
    sortMenusOrderByDescending: false,
};

const vitePressSidebarOptions = supportedLocales.map((lang) => {
    const isRootLocale = rootLocale === lang;

    return {
        ...commonSidebarConfigs,

        documentRootPath: "docs",
        scanStartPath: isRootLocale ? undefined : lang,

        resolvePath: isRootLocale ? "/" : `/${lang}/`,
        basePath: isRootLocale ? "/" : `/${lang}/`,
    };
});

export default defineConfig(
    withSidebar(vitePressOptions, vitePressSidebarOptions),
);
