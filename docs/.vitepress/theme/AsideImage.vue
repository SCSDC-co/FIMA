<script setup>
import DefaultTheme from "vitepress/theme";

const { Layout } = DefaultTheme;
</script>

<template>
    <Layout>
        <template #aside-ads-after>
            <div id="f-after-aside-container">
                <img id="f-after-aside" src="/F.svg" />
            </div>
        </template>
    </Layout>
</template>

<style>
#f-after-aside-container {
    width: 100%;
    height: 100%;

    display: flex;

    justify-content: center;
}

#f-after-aside {
    width: 70px;
}
</style>
