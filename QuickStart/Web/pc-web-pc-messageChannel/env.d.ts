interface ImportMetaEnv {
    VEGAME_ACCOUNT_ID: string
    VEGAME_GAME_ID: string
    VEGAME_TOKEN_CURRENT_TIME: string
    VEGAME_TOKEN_EXPIRED_TIME: string
    VEGAME_TOKEN_SESSION_TOKEN: string
    VEGAME_TOKEN_ACCESS_KEY_ID: string
    VEGAME_TOKEN_SECRET_ACCESS_KEY: string
}

interface ImportMeta {
    readonly env: ImportMetaEnv
}