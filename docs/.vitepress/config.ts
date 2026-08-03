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
            { text: "Examples", link: "/markdown-examples" },
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

        sidebar: [
            {
                text: "Examples",
                items: [
                    { text: "Markdown Examples", link: "/markdown-examples" },
                    { text: "Runtime API Examples", link: "/api-examples" },
                ],
            },
        ],
    },
});
