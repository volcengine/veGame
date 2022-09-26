// 实例化 veGameSDK 的参数
const initConfig = {
  userId: '',
  enableLocalKeyboard: true, // 是否开启本地键盘输入。前提：需要联系运营同学给游戏所在业务开启「拉起本地输入法配置」
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
  roundId: '',
  rotation: 'landscape', // landscape：横屏启动; portrait：竖屏启动
};

window.initConfig = initConfig;
window.startConfig = startConfig;
