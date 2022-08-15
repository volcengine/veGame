// 实例化 veGameSDK 的参数
const initConfig = {
    userId: ''
};

// 调用 veGameSDK.start 的参数
const startConfig = {
    gameId: '',
    token: {
        CurrentTime: '',
        ExpiredTime: '',
        SessionToken: '',
        AccessKeyID: '',
        SecretAccessKey: ''
    },
    roundId: ''
};

window.initConfig = initConfig;
window.startConfig = startConfig;