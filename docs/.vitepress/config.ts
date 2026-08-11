import { defineConfig, UserConfig } from "vitepress";
import { withSidebar } from "vitepress-sidebar";
import { VitePressSidebarOptions } from "vitepress-sidebar/types";

const vitePressOptions: UserConfig = {
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

        logo: { src: "/F.svg", alt: "FIMA's F" },

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

        externalLinkIcon: true,
    },

    head: [["link", { rel: "icon", href: "/FIMA/favicon.ico" }]],
};

const vitePressSidebarOptions: VitePressSidebarOptions = {
    documentRootPath: "docs/",
    collapsed: true,
    capitalizeFirst: true,
    hyphenToSpace: true,
    useTitleFromFileHeading: true,
    manualSortFileNameByPriority: [
        "get-started.md",
        "features.md",
        "customization-and-configuration.md",
        "trash-bin.md",
    ],
    sortMenusOrderByDescending: false,
};

export default defineConfig(
    withSidebar(vitePressOptions, vitePressSidebarOptions),
);
