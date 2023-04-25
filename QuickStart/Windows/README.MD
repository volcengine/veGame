# veGame PC SDK Demo

## 说明

本项目是火山引擎云游戏客户端 SDK 的快速演示 demo。获取项目以后，开发者可以快速构建应用，体验火山引擎云游戏服务的主要功能；也能参考其中的代码，在实际的客户端应用中实现相似的功能。

## 环境要求

 - Windows 10 x64 / x86（推荐）
 - Visual Studio 2019（推荐）
 - Visual Studio 2022（需手动安装 v142 x64/x86 生成工具）

## 接入流程

1. 将 `include` 目录与 `veGameEngine.lib` 文件添加到工程中，以 VS 为例。

2. 添加 `veGameEngine.lib`。

3. 再添加库依赖项。

### 依赖库

#### bin\

 - libcrypto-3-x64.dll
 - libEGL.dll
 - libGLESv2.dll
 - libprotobuf.dll
 - libssl-3-x64.dll
 - openh264-4.dll
 - pthreadVC2.dll
 - reportSDK.dll
 - RTCFFmpeg.dll
 - veGameEngine.dll
 - VolcEngineRTC.dll

#### 头文件
 - inclue\

#### libs
 - lib\

### 快速开始

#### 账号、鉴权相关

1. 在接入云游戏 SDK 之前，需要获取火山引擎账号对应的原始 AccessKey（ak）和 SecretKey（sk），用于生成临时鉴权密钥（登录火山引擎控制台后，点击页面右上角用户信息，选择 账号 > API访问密钥）。

2. 通过云游戏控制台获取用于鉴权的临时密钥（ak/sk/token 的获取方式请参考 [控制台指南](https://www.volcengine.com/docs/6512/136537#%E4%B8%B4%E6%97%B6%E9%89%B4%E6%9D%83%E5%AF%86%E9%92%A5)）。

3. 获取火山引擎用户账号 ID，将其填入 [一、初始化vegame::IGameEngine] 对应信息中（登录火山引擎控制台后，点击页面右上角用户信息，选择 账号管理 > 主账号信息）。

4. 获取到用于鉴权的临时密钥 ak/sk/token 之后，将其填入 [二、参数配置] 的对应位置。

#### 一、初始化 vegame::IGameEngine

```C++
vegame::EngineConfig engineConfig = {0};
// 必须填写，火山引擎用户账号ID
engineConfig.account_id = "";
engineConfig.user_id = "";

vegame::IGameEngine* gameEngine = vegame::CreateGameEngine();
gameEngine->init(engineConfig , dynamic_cast<vegame::IEngineEventHandler*>(this));
```

#### 二、参数配置

```C++
    // ak、sk、token，通过云游戏控制台获取临时密钥，详情见https://www.volcengine.com/docs/6512/136537
    vegame::GameConfig gameConfig = { 0 };
    gameConfig.canvas = DemoCore::instance().getMainWnd();  // 必填字段
    gameConfig.ak = "";                                     // 必填字段
    gameConfig.sk = "";                                     // 必填字段
    gameConfig.token = "";                                  // 必填字段
    gameConfig.round_id = "";                               // 必填字段
    gameConfig.game_id = "";                                // 必填字段
    gameConfig.custom_game_id = "";                         // 选填字段
    gameConfig.profile_path_list = "";                      // 选填字段
    gameConfig.auto_recycle_time = 0;                       // 选填字段
    gameConfig.video_stream_profile_id = 24;                // 选填字段

```
#### 三、开始游戏
```C++
// 启动云游戏
    vegame_engine_->start(gameConfig);
```

## 参考资料

火山引擎云游戏客户端 SDK 下载：[https://www.volcengine.com/docs/6512/75594](https://www.volcengine.com/docs/6512/75594)。

注：如果不能访问以上链接，请参考 [开通云游戏服务](https://www.volcengine.com/docs/6512/75577) 说明文档。
