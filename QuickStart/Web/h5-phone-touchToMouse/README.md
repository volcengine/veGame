# 云游戏客户端 Web SDK Sample - H5 玩手游 - 触控转鼠标示例

## 功能说明

这个示例展示了如何在 H5 端玩手游场景下触控转鼠标。

移动端 H5，touchstart + touchend 可以认为是一次点击操作。但在实际场景下，touchstart 与 touchend 触发过程中，难以避免会顺带触发 touchmove。为了区分这两种情况，推荐参考 touchend 与 touchstart 的触发间隔辅助判断，如果间隔短，可以认为是鼠标点击。如果间隔较长，可以认为是鼠标滑动。

由于在 H5 上，【鼠标拖拽效果】与【鼠标滑动效果】触发的事件序列一致，只能刻意地去引导用户做出不同的行为。例如：想拖拽，手指长按后滑动；仅滑动，手指触屏后立马滑动。因为，就可以根据手指触屏和手指第一次滑动的时间间隔对这两种情况进行判断

## 目录结构

```bash
├── src/
│   ├── assets/           # 静态资源
│   ├── styles/           # 全局样式
│   ├── constants/        # 常量
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

3. 虚拟手柄只支持 H5 端使用，根据您的游戏类型调整`index.ts`中的初始化参数`mode`，默认是 H5 玩手游

```
| 实际场景       | mode（手游/端游）   |
| h5玩端游       | MODE.CLOUD_PC        |
| h5玩手游       | MODE.CLOUD_PHONE_GAME|
```

4. 启动项目：

```bash
cd QuickStart/Web/h5-phone-touchToMouse
npm run dev
```

## 问题反馈

如果您在使用过程中遇到问题，欢迎在 [Issues](https://github.com/volcengine/veGame/issues) 中反馈
