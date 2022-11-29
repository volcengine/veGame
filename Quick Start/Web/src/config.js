// 实例化 veGameSDK 的参数
const initConfig = {
  userId: '', // 必填参数，开发者可进行自定义，可以用来标识游戏玩家或账号等
  accountId:'', // 必填参数，火山引擎账号ID，可通过火山引擎官网页面右上角 用户 > 账号管理 > 主账号信息 获取
  enableLocalKeyboard: true, // 是否开启本地键盘输入。前提：需要联系运营同学给游戏所在业务开启「拉起本地输入法配置」
};

// 调用 veGameSDK.start 的参数
const startConfig = {
  gameId: '', // 游戏ID
  token: {
      CurrentTime: '',    // Token创建时间
      ExpiredTime: '',    // Token过期时间
      SessionToken: '',   // 用于鉴权的临时Token
      AccessKeyID: '',    // 用于鉴权的临时AccessKey
      SecretAccessKey: '' // 用于鉴权的临时SecretKey
  },
  roundId: '',
  rotation: 'landscape', // landscape：横屏启动; portrait：竖屏启动
};

window.initConfig = initConfig;
window.startConfig = startConfig;
