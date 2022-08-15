# veGame SDK Demo

## 说明

本项目是火山引擎云游戏客户端 SDK 的快速演示 demo。获取项目以后，开发者可以快速构建应用，体验云游戏服务的主要功能；也能参考其中的代码，在实际的客户端应用中实现相似的功能。

## 环境要求

### 硬件要求

支持 Android 4.3（Android-19+）及以上系统的真机设备，支持 armeabi-v7a。

### 软件要求

- IDE：Android Studio（推荐使用最新版本）
- 搭建 Java 环境，使用 Java 作为开发语言

## 快速开始

1. 克隆或下载 demo 源文件到本地；
2. 在 Android Studio 中引入项目；运行工程文件，编译生成 APK 文件，在 Android 设备上运行预览；
3. 相关的运行信息，在打印的log中查看。

## 目录结构

```
├── main
│       │   ├── AndroidManifest.xml
│       │   ├── java
│       │   │   └── com
│       │   │       └── volcengine
│       │   │           └── vegameengine
│       │   │               ├── GameActivity.java  //显示游戏的Activity
│       │   │               ├── GsonConverter.java  // 用于SDK 传入的JSON转换的实现 
│       │   │               ├── InitApplication.java // 工程的Application 负责初始化SDK等
│       │   │               ├── MainActivity.kt // 首页指定 gameId、ak、sk、token
│       │   │               ├── ui
│       │   │               │   └── MessageChannelDialog.java
│       │   │               └── util
│       │   │                   ├── DialogUtils.java
│       │   │                   ├── ScreenUtil.java  //屏幕工具类用于适配挖孔屏
│       │   │                   └── SizeUtils.java
     
```

## 接入的流程

1. 在 Application 中可以选择初始化一些埋点信息；
2. 在 MainActivity 中填写游戏的 gameId、ak、sk 和 token，并初始化云游戏 SDK 实例（`veGameEngine`）；
3. 初始化成功后，调用 `start()` 接口开始播放；
4. 切到后台，可以选择调用 `pause()` 接口停止音视屏流；
5. 切回前台，可以选择调用 `resume()` 接口恢复音视频流；
6. 结束，调用`stop()` 接口结束游戏并释放资源。

## 参考资料

客户端 SDK 下载：https://www.volcengine.com/docs/6512/75594

注：如果不能访问以上链接，请参考 [开通云游戏服务](https://www.volcengine.com/docs/6512/75577) 说明文档。
