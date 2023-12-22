# 云游戏客户端 Web SDK demo

这个开源项目展示了火山引擎云游戏客户端 Web SDK 的以下功能：

1. 配置接入云游戏服务的必要参数
2. 开始游戏和结束游戏

方便用户快速接入云游戏客户端 Web SDK。

## 运行示例程序

1. 将 Web SDK demo 代码下载到本地；
2. 前往火山引擎 [云游戏文档中心]( https://www.volcengine.com/docs/6512/75594)，下载 Web/H5 SDK；解压后将其复制到 `lib` 文件夹下，并确认文件名为 `veGameSDK-Web.js`；
3. 在 `src/config.js` 配置文件中补全以下配置信息：

```js

// 实例化 veGameSDK 的参数
const initConfig = {
  userId: '', // 必填参数，开发者可进行自定义，可以用来标识游戏玩家或账号等
  accountId: '', // 必填参数，火山引擎账号ID，可通过火山引擎官网页面右上角 用户 > 账号管理 > 主账号信息 获取
  enableLocalKeyboard: true, // 是否开启本地键盘输入。前提：需要联系运营同学给游戏所在业务开启「拉起本地输入法配置」
  mode: window.veGameSDK.MODE.CLOUD_PHONE_GAME, // 默认为云游戏手游，如果需要控制云游戏端游，请传 MODE.CLOUD_PC 
  isPC, // 是否为 PC Web 环境下使用。默认非 PC Web 环境下，将会监听 Touch 事件，而 PC Web 环境下将监听鼠标以及键盘事件
  domId: 'player',
  isDebug: true,
};

// 调用 veGameSDK.start 的参数
const startConfig = {
  gameId: '', //必填参数，游戏ID，可通过火山引擎云游戏控制台『游戏管理』页面获取，例如：1428112352161312345
  token: {
      SessionToken: '',   // 用于鉴权的临时Token
      AccessKeyID: '',    // 用于鉴权的临时AccessKey
      SecretAccessKey: '' // 用于鉴权的临时SecretKey
  }, // 必填参数，启动云游戏的令牌，通过调用服务端 STSToken 接口获取，接口参考：https://www.volcengine.com/docs/6512/75588
  roundId: '', //必填参数，当次游戏生命周期的标识符，请根据实际业务需求做修改
  audioAutoPlay: true,
  mute: true, // audioAutoPlay 和 mute 设置为 true 是针对部分浏览器对网页上的自动播放功能做了限制而做的特殊处理，请根据实际业务场景选择是否设置以及使用哪种自动播放处理策略。云游戏自动播放策略处理详见文档：https://www.volcengine.com/docs/6512/129586
};

```

3. 在浏览器中打开 `src/index.html` 文件；
   
4. 点击开始游戏，体验云游戏效果。

## 参考资料

客户端 SDK 下载：https://www.volcengine.com/docs/6512/75597

注：如果不能访问以上链接，请参考 [开通云游戏服务](https://www.volcengine.com/docs/6512/75577) 说明文档。
