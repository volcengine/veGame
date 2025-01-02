# 云游戏客户端 Web SDK demo

这个开源项目展示了 PC 端玩手游的基本配置，方便用户快速接入云游戏客户端 Web SDK。

## 目录结构

```bash
├── src
│   ├── config.js # 导出 Web SDK 需要的配置
│   ├── index.html # 页面布局
│   ├── main.js # 主要实现逻辑
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
cd vePhone/QuickStart/Web/PcWeb_Phone
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
