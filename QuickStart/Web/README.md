# 云游戏客户端 Web SDK demo

这个开源项目展示了火山引擎云游戏客户端 Web SDK 的以下功能：

1. 配置接入云游戏服务的必要参数
2. 开始游戏和结束游戏
3. 大文件传输
4. 清晰度切换
5. 自定义本地键盘 Input 框
6. 虚拟手柄
7. 挂机
8. 单触点转鼠标消息
9. 设置保活时间
10. 设置重连时间
11. 坐标旋转
12. 切换角色
13. 客户端前后台切换
14. 同步本地键盘状态到云端
15. 更新画面放缩比
16. 虚拟键鼠
17. 本地输入法显示中文合成过程
18. 自动回收时长

方便用户快速接入云游戏客户端 Web SDK。

## 目录结构

```bash
├── src
│   ├── config.js # 导出 Web SDK 需要的配置
│   ├── index.html # 页面布局
│   ├── main.js # 主要实现逻辑
│   ├── libs # 项目依赖的sdk
│   ├── utils.js # 公共方法
│   ├── constant.js # 公共常量
│   ├── features # 功能模块
│   │   ├── file-channel.js # 大文件传输
│   ├── features # 功能模块
│   │   ├── index.js # 导出模块
│   │   ├── clarity.js # 清晰度切换
│   │   ├── clear-custom-local-keyboard-input.js # 清除自定义本地键盘 Input 框的状态
│   │   ├── game-pad.js # 虚拟手柄
│   │   ├── get-auto-recycle-time.js # 获取自动回收时长
│   │   ├── get-user-profile-path.js # 获取存档路径
│   │   ├── idle-time.js # 设置保活时间
│   │   ├── ime-composition-visible.js # 本地输入法显示中文合成过程
│   │   ├── reconnect-time.js # 设置重连时间
│   │   ├── rotate-coordinate.js # 坐标旋转
│   │   ├── session-mode.js # 挂机
│   │   ├── set-auto-recycle-time.js # 自动回收时长
│   │   ├── set-idle-time.js # 设置保活时间
│   │   ├── set-custom-local-keyboard-input-id.js # 设置自定义本地键盘 Input 框
│   │   ├── set-role.js # 切换角色
│   │   ├── set-touch-to-mouse.js # 单触点转鼠标消息
│   │   ├── set-user-profile-path.js # 设置存档路径
│   │   ├── switch-background.js # 客户端前后台切换
│   │   ├── sync-local-keyboard-close-status.js # 同步本地键盘状态到云端
│   │   ├── update-video-scale.js # 更新画面放缩比
│   │   ├── virtual-input-suite.js # 虚拟键鼠
│   └── style.css # 样式文件
└── vite.config.js # vite 配置文件
└── .env #  Web SDK 启动需要的配置
```

## 运行示例程序

> 首先需要确保拥有 `node` 环境，如果没有，请前往 [nodejs 官网](https://nodejs.org/zh-cn/download)下载并安装 `nodejs`

1. 下载项目到本地

```bash
git clone https://github.com/volcengine/veGame.git
```

2. 进入 Web Quick Start 目录

```bash
cd vePhone/QuickStart/WebNpm
```

3. 安装依赖

```bash
npm install
```

4. 在`.env`文件中填写启动云游戏需要的配置，配置如下：

```bash
# init config
VEGAME_ACCOUNT_ID="your accountId" # 火山引擎用户账号，可通过火山引擎官网页面右上角 用户 > 账号管理 > 主账号信息 获取

# start config
VEGAME_GAME_ID="your gameId" # 游戏ID，可通过火山引擎云游戏控制台『游戏管理』页面获取，例如：1428112352161312345

# start token 启动云游戏的令牌（通过调用服务端 STSToken 接口获取），有关服务端 STSToken 接口的详细信息，参考 [签发临时 Token](https://www.volcengine.com/docs/6512/75588)
# 实际业务场景中根据接口获取，就没必要放在环境变量中，start 之前先调用一遍接口，获取 token 即可
 # Token 创建时间
VEGAME_TOKEN_CURRENT_TIME=""
 #Token 过期时间
VEGAME_TOKEN_EXPIRED_TIME=""
# 用于鉴权的临时 Token
VEGAME_TOKEN_SESSION_TOKEN=""
# 用于鉴权的临时 Access Key
VEGAME_TOKEN_ACCESS_KEY_ID=""
# 用于鉴权的临时 Secret Key
VEGAME_TOKEN_SECRET_ACCESS_KEY=""
```

5. 执行启动命令

```bash
npm run dev
```

6. 打开浏览器，访问 `localhost:8080` 即可

## 参考资料

注：如果不能访问以上链接，请参考 [开通云游戏服务](https://www.volcengine.com/docs/6512/75577) 说明文档。
