import type { Theme } from "vitepress";
import DefaultTheme, { VPBadge } from "vitepress/theme";
import Contributors from "@cssnr/vitepress-plugin-contributors";
import "@cssnr/vitepress-plugin-contributors/style.css";
import "./style.css";

import contributors from "../contributors.json";
import Showcase from "../../components/Showcase.vue";
import AsideImage from "./AsideImage.vue";

export default {
    extends: DefaultTheme,
    enhanceApp({ app }) {
        app.component("Badge", VPBadge);
        app.component("Contributors", Contributors);
        app.component("Showcase", Showcase);

        app.config.globalProperties.$contributors = contributors;
    },
    Layout: AsideImage,
};
