import type { Theme } from "vitepress";
import DefaultTheme from "vitepress/theme";
import "./style.css";

import ImageViewerP from "@davidingplus/vitepress-image-viewer";
import "@davidingplus/vitepress-image-viewer/style.css";

export default {
    extends: DefaultTheme,
    enhanceApp(ctx) {
        ImageViewerP(ctx.app);
    },
};
