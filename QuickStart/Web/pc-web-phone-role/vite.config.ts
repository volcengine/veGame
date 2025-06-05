import { defineConfig } from 'vite'

export default defineConfig({
    root: "src",
    envDir: "../",
    envPrefix: "VEGAME_",
    server: {
        port: 8080
    }
}) 