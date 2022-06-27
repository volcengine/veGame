# 云游戏 Web SDK demo

这个开源项目展示了火山引擎云游戏 Web SDK 的以下功能：

1. 开始游戏和结束游戏

方便用户快速接入云游戏 Web SDK。

## 运行示例程序

1. 将代码下载到本地；
2. 将 Web SDK 复制到 `lib` 文件夹下，重命名为 `veGameSDK-Web.js`，在`config.js` 中补全以下配置信息：

```js

// 实例化 veGameSDK 的参数
const initConfig = {
    userId: '',
    appId: '',
};

// 调用 veGameSDK.start 的参数
// 以下只列出调用参数的必传参数，更多参数参考 Web SDK 的使用文档
const startConfig = {
    roundId: '',
    gameId: '',
    rotation: 'landscape', // landscape：横屏启动; portrait：竖屏启动
    token: {
        CurrentTime: '',
        ExpiredTime: '',
        SessionToken: '',
        AccessKeyID: '',
        SecretAccessKey: ''
    },
};

```
   
3. 在浏览器中打开 `index.html` 文件；
   
4. 点击开始游戏。

