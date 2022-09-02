// 实例化 veGameSDK 的参数
const initConfig = {
  userId: '',
};

// 调用 veGameSDK.start 的参数
const startConfig = {
  gameId: '',
  token: {
    CurrentTime: '',
    ExpiredTime: '',
    SessionToken: '',
    AccessKeyID: '',
    SecretAccessKey: '',
  },
  roundId: '',
  rotation: 'landscape', // landscape：横屏启动; portrait：竖屏启动
};

window.initConfig = initConfig;
window.startConfig = startConfig;
