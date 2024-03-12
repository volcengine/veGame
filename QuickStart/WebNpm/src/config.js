import { MODE } from '@volcengine/vegame';
console.log(MODE, import.meta.env);
const isPC =
  !/Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(
    navigator.userAgent
  );

// 实例化 veGameSDK 的参数
// 初始化配置由环境变量中获得
// 由于采用的是 vite 构建的，所以环境变量在 import.meta.env 中。详见 https://cn.vitejs.dev/guide/env-and-mode.html
export const initConfig = {
  userId: 'vegame_github_quick_start', // 必填参数，开发者可进行自定义，可以用来标识游戏玩家或账号等
  accountId: import.meta.env.VEGAME_ACCOUNT_ID, // 必填参数，火山引擎账号ID，可通过火山引擎官网页面右上角 用户 > 账号管理 > 主账号信息 获取
  enableLocalKeyboard: true, // 是否开启本地键盘输入。前提：需要联系运营同学给游戏所在业务开启「拉起本地输入法配置」
  mode: MODE.CLOUD_PHONE_GAME, // 默认为云游戏手游 MODE.CLOUD_PHONE_GAME，如果需要控制云游戏端游，请传 MODE.CLOUD_PC
  isPC, // 是否为 PC Web 环境下使用。默认非 PC Web 环境下，将会监听 Touch 事件，而 PC Web 环境下将监听鼠标以及键盘事件
  domId: 'player',
  isDebug: true,
};

// 调用 veGameSDK.start 的参数
export const startConfig = {
  gameId: import.meta.env.VEGAME_GAME_ID, //必填参数，游戏ID，可通过火山引擎云游戏控制台『游戏管理』页面获取，例如：1428112352161312345
  token: {
    CurrentTime: import.meta.env.VEGAME_TOKEN_CURRENT_TIME, // Token创建时间
    ExpiredTime: import.meta.env.VEGAME_TOKEN_EXPIRED_TIME, // Token过期时间
    SessionToken: import.meta.env.VEGAME_TOKEN_SESSION_TOKEN, // 用于鉴权的临时Token
    AccessKeyID: import.meta.env.VEGAME_TOKEN_ACCESS_KEY_ID, // 用于鉴权的临时AccessKey
    SecretAccessKey: import.meta.env.VEGAME_TOKEN_SECRET_ACCESS_KEY, // 用于鉴权的临时SecretKey
  }, // 必填参数，启动云游戏的令牌，通过调用服务端 STSToken 接口获取，实际业务场景中根据接口获取，就没必要放在环境变量中，start 之前先调用一遍接口，获取 token 即可
  roundId: 'vegame_github_quick_start_round_id', //必填参数，当次游戏生命周期的标识符，请根据实际业务需求做修改
  audioAutoPlay: true,
  mute: true, // audioAutoPlay 和 mute 设置为 true 是针对部分浏览器对网页上的自动播放功能做了限制而做的特殊处理，请根据实际业务场景选择是否设置以及使用哪种自动播放处理策略。云游戏自动播放策略处理详见文档：https://www.volcengine.com/docs/6512/129586
};
