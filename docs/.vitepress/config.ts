import { defineConfig } from "vitepress";

// https://vitepress.dev/reference/site-config
export default defineConfig({
    title: "FIMA",
    description: "Fast, Incredible, Minimal & Awesome File Manager",

    cleanUrls: true,
    appearance: "force-dark",
    lastUpdated: true,
    base: "/FIMA/",

    themeConfig: {
        // https://vitepress.dev/reference/default-theme-config
        nav: [
            { text: "Home", link: "/" },
            { text: "Get Started", link: "/get-started" },
        ],

        socialLinks: [
            { icon: "github", link: "https://github.com/SCSDC-co/FIMA" },
        ],

        search: {
            provider: "local",
        },

        editLink: {
            pattern: "https://github.com/SCSDC-co/fima/edit/main/docs/:path",
        },

        footer: {
            message: "Released under the GPL 3.0 License.",
            copyright: "Copyright © 2026-present Giuliano De Amicis",
        },

        sidebar: [
            {
                text: "Docs",
                items: [
                    { text: "Get Started", link: "/get-started" },
                    {
                        text: "Features",
                        items: [
                            { text: "Home Page", link: "/features" },
                            { text: "LS", link: "/features/ls" },
                            { text: "Tree", link: "/features/tree" },
                            { text: "MK", link: "/features/mk" },
                            { text: "RM", link: "/features/rm" },
                            { text: "CP", link: "/features/cp" },
                            { text: "MV", link: "/features/mv" },
                            { text: "Perms", link: "/features/perms" },
                            { text: "CLOC", link: "/features/cloc" },
                            { text: "Info", link: "/features/info" },
                            { text: "Zip", link: "/features/zip" },
                            { text: "Unzip", link: "/features/unzip" },
                        ],
                    },
                ],
            },
        ],
    },
});
