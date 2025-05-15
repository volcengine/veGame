# 云游戏客户端 Web SDK Sample - PC 玩手游 - 设置保活时间示例

## 功能说明

这个示例展示了如何在 PC 端玩手游场景下设置保活时间。

在游戏中动态设置客户端切后台或者客户端断连之后，云端游戏服务的保活时间（如不设置，则使用默认保活时长 300 秒；支持设置的上限值为 7200，即 2 小时；如需设置更长时间，可联系火山引擎云游戏技术支持）。

当客户端切后台或客户端断连时开始计时，如用户在设置的保活时间到达时未切回前台或者未恢复连接，服务端将断开游戏连接。

## 目录结构

```bash
├── src/
│   ├── styles/           # 全局样式
│   ├── index.html
│   ├── core.ts           # sdk初始化
│   └── index.ts          # 统一入口
├── .env                 # 环境变量
├── README.md
├── .gitignore
├── package.json
├── vite.config.ts
└── tsconfig.json
```

## 前置条件

- Node.js 无版本要求
- SDK 版本 >= 1.62.0
- 浏览器需支持 WebRTC

## ⚙️ 配置项说明

配置项保存在 `.env` 文件中：

```env
# 必填参数，火山引擎账号ID
VEGAME_ACCOUNT_ID="your accountId"

# 必填参数，游戏ID （⚠️ 前往火山引擎控制台选择某个业务下获取到游戏ID，区分手游/端游）
VEGAME_GAME_ID="your gameId"

# Token相关配置：启动云游戏的令牌
VEGAME_TOKEN_CURRENT_TIME=""
VEGAME_TOKEN_EXPIRED_TIME=""
VEGAME_TOKEN_SESSION_TOKEN=""
VEGAME_TOKEN_ACCESS_KEY_ID=""
VEGAME_TOKEN_SECRET_ACCESS_KEY=""
```

临时 Token 可通过 [火山引擎控制台](https://console.volcengine.com/veGame/region:veGame+cn-north-1/guidepage?activeStep=sdk&collapse=false) 获取。实际业务场景中根据接口获取，就没必要放在环境变量中，（通过调用服务端 STSToken 接口获取），有关服务端 STSToken 接口的详细信息，参考 [签发临时 Token](https://www.volcengine.com/docs/6512/75588)，start 之前先调用一遍接口，获取 token 即可

## 快速运行

1. 安装依赖：

```bash
npm install
```

2. 配置环境变量：
   修改 `.env` 文件，填入您的配置信息

3. 根据您的实际场景调整`index.ts`中的初始化参数`initConfig`，默认是 PC 玩手游

```
| 实际场景       | isPC（是否是PC端） | mode（手游/端游）   |
| pc玩端游       | true             | MODE.CLOUD_PC        |
| pc玩手游       | true             | MODE.CLOUD_PHONE_GAME|
| h5玩端游       | false            | MODE.CLOUD_PC        |
| h5玩手游       | false            | MODE.CLOUD_PHONE_GAME|
```

4. 启动项目：

```bash
npm run dev
cd QuickStart/Web/pc-web-phone-idleTime
```

## 问题反馈

如果您在使用过程中遇到问题，欢迎在 [Issues](https://github.com/volcengine/veGame/issues) 中反馈
