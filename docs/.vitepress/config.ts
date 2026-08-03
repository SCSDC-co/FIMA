import { defineConfig } from "vitepress";

// https://vitepress.dev/reference/site-config
export default defineConfig({
    title: "FIMA",
    description: "Fast, Incredible, Minimal & Awesome File Manager",

    cleanUrls: true,
    appearance: "force-dark",
    lastUpdated: true,
    base: "/fima/",

    themeConfig: {
        // https://vitepress.dev/reference/default-theme-config
        nav: [
            { text: "Home", link: "/" },
            { text: "Get Started", link: "/get-started" },
        ],

        socialLinks: [
            { icon: "github", link: "https://github.com/SCSDC-co/fima" },
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
                            { text: "ls", link: "/features/ls" },
                            { text: "tree", link: "/features/tree" },
                            { text: "mk", link: "/features/mk" },
                            { text: "rm", link: "/features/rm" },
                            { text: "cp", link: "/features/cp" },
                            { text: "mv", link: "/features/mv" },
                            { text: "perms", link: "/features/perms" },
                            { text: "cloc", link: "/features/cloc" },
                            { text: "info", link: "/features/info" },
                            { text: "zip", link: "/features/zip" },
                            { text: "unzip", link: "/features/unzip" },
                        ],
                    },
                ],
            },
        ],
    },
});
