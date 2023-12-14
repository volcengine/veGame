﻿# veGame SDK Demo

## 说明

本项目是火山引擎云游戏客户端 SDK 的快速演示 demo。获取项目以后，开发者可以快速构建应用，体验云游戏服务的主要功能；也能参考其中的代码，在实际的客户端应用中实现相似的功能。

## 错误码

在接入 SDK 的过程中遇到**错误码**时，可参考 [ErrorCode.md](ErrorCode.md) 中的操作建议进行下一步操作。

## 环境要求

### 硬件要求

支持 Android 4.3（Android-19+）及以上系统的真机设备，支持 armeabi-v7a。

### 软件要求

- IDE：Android Studio（推荐使用最新版本）
- 搭建 Java 环境，使用 Java 作为开发语言，JDK版本需要1.8+

## 接入流程

### 添加 Maven 仓库地址

1. 在 project 根目录下的 build.gradle 文件中的 repositories 闭包中配置 Maven 仓库地址：

```java
buildscript {
    repositories {
        maven {
            url 'https://artifact.bytedance.com/repository/Volcengine/'
        }
    }
}
allprojects {
    repositories {
        maven {
            url 'https://artifact.bytedance.com/repository/Volcengine/'
        }
    }
}
```

2. 在应用模块的 build.gradle 文件中的 dependencies 中加入依赖项：

```java
dependencies {
    implementation fileTree(include: ['*.jar'], dir: 'libs')
    // 云游戏 SDK
    implementation 'com.volcengine.vegame:vegame:1.30.0'
    // 手柄 SDK
    implementation 'com.volcengine.vegame:VeGameConsole:1.0.2'
    
    implementation 'androidx.annotation:annotation:1.1.0'
        
    // 选择引用以下三种框架中的任意一种
    implementation 'com.google.code.gson:gson:2.8.5' // gson
        
    implementation 'com.alibaba:fastjson:1.1.72.android' // fastjson
        
    implementation 'com.fasterxml.jackson.core:jackson-databind:2.11.1' // jackson
    implementation 'com.fasterxml.jackson.core:jackson-core:2.11.1' //jackson

    // 大文件传输特性（FileChannelExt）需要以下依赖项
    implementation 'com.squareup.okhttp3:okhttp:4.9.0'
}
```

3. 设置 Java 版本到 1.8：

```java
android {
    // ...
    compileOptions {
        sourceCompatibility JavaVersion.VERSION_1_8
        targetCompatibility JavaVersion.VERSION_1_8
    }
}
```

### 权限声明

根据实际使用场景在 AndroidManifest.xml 文件中声明 SDK 需要的权限：

```java
//网络权限，使用场景：音视频传输等
<uses-permission android:name="android.permission.INTERNET" />
//WiFi网络状态，使用场景：用户手机网络状态变化监听
<uses-permission android:name="android.permission.CHANGE_WIFI_STATE" />
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
<uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
//录音权限，使用场景：[开启/关闭] 麦克风
<uses-permission android:name="android.permission.RECORD_AUDIO" />
//设置播放模式的权限：外放 / 听筒
<uses-permission android:name="android.permission.MODIFY_AUDIO_SETTINGS" />
//同步定位信息，使用场景：当有些游戏需要获取用户的地理位置时，需要获取用户的地理位置信息
//并传送给远端Pod
<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
<uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
<uses-permission android:name="android.permission.ACCESS_LOCATION_EXTRA_COMMANDS" />
//读写存储
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
```

#### 说明
1. 存储写入权限需动态申请。参考：https://developer.android.com/training/permissions/requesting
2. 如果 APP 指向 Android 10 及以上（targetSdkVersion >= 29），而且并未适配 Scoped Storage，那么需要
    将AndroidManifest.xml文件中的requestLegacyExternalStorage设置为true。参考：https://developer.android.com/training/data-storage/use-cases#opt-out-scoped-storage
```java
<manifest>
    <application android:requestLegacyExternalStorage="true">
    </application>
</manifest>
```
### 快速开始

#### 鉴权相关

1. 在接入云游戏 SDK 之前，需要获取火山引擎账号对应原始的 AccessKey（ak）和 SecretKey（sk），用于生成临时鉴权密钥（登录火山引擎控制台后，点击页面右上角用户信息，选择 账号 > API访问密钥）。

2. 调用 “签发临时Token” 接口，获取用于鉴权的临时密钥（ak/sk/token 的获取方式请参考 [快速入门](https://www.volcengine.com/docs/6512/75577)）。

3. 获取到 ak/sk/token 之后，将其填入 [二、配置GamePlayConfig](#二、配置GamePlayConfig) 的对应位置。

4. 除此之外，需要在 [app/src/main/AndroidManifest.xml](app/src/main/AndroidManifest.xml) 文件的 meta-data 中填入注册的火山引擎用户账号，参考以下示例：

```java
<meta-data
    android:name="VOLC_ACCOUNT_ID"
    android:value="21000xxxxx" />
```

#### 一、初始化 VeGameEngine

```java
VeGameEngine.getInstance().prepare();
```

注：VeGameSDK版本在1.28.0之后，init()方法会废弃，请使用prepare()方法进行VeGameEngine的初始化。

#### 二、配置 GamePlayConfig

```java
GamePlayConfig.Builder builder = new GamePlayConfig.Builder();

builder.userId(userId) // 必填参数，用户userid
    .ak(ak) // 必填参数，临时鉴权 ak
    .sk(sk)  // 必填参数，临时鉴权 sk
    .token(token) // 必填参数，临时鉴权 token
    .container(container) // 必填参数，用来承载画面的 Container, 参数说明: layout 需要是 FrameLayout 或者 FrameLayout 的子类
    .roundId(roundId) // 必填参数，自定义roundId
    .gameId(gameId) // 必填参数, gameId
    .streamListener(streamListener); // 必填参数，用于获取音视频流信息回调监听

GamePlayConfig gamePlayConfig = builder.build();
```

#### 三、开始游戏
```java
veGameEngine.start(gamePlayConfig, IGamePlayerListener playerListener);
```

## 目录结构

```
main
├── AndroidManifest.xml
├── assets // 该目录及文件需要自行创建
│   └── sts.json // 保存鉴权相关的 gameId/ak/sk/token
├── java
│   └── com
│       └── volcengine
│           └── vegameengine
│               ├── GsonConverter.java // 用于SDK传入的JSON转换
│               ├── InitApplication.java // 工程的Application 负责初始化SDK等
│               ├── MainActivity.java // 用于展示SDK的特性列表，并进入对应特性的体验界面
│               ├── WebViewActivity.kt // 用于展示火山引擎的官网
│               ├── base
│               │   ├── BaseListActivity.java
│               │   ├── BasePlayActivity.java
│               │   └── BaseSampleActivity.kt
│               ├── feature // 用于体验SDK不同的特性
│               │   ├── AudioServiceActivity.java // 音频
│               │   ├── CamaraManagerActivity.java // 相机
│               │   ├── ClarityServiceActivity.java // 清晰度
│               │   ├── ClipBoardServiceManagerActivity.java // 剪切板
│               │   ├── FileChannelExtActivity.java // 大文件通道
│               │   ├── GroundManagerActivity.java // 前后台切换
│               │   ├── IODeviceManagerActivity.java // IO设备(仅支持端游)
│               │   ├── LocalInputManagerActivity.java // 本地输入
│               │   ├── LocationServiceActivity.java // 定位服务
│               │   ├── MessageChannelActivity.java // 消息通道
│               │   ├── MultiUserManagerActivity.java // 多用户
│               │   ├── PadConsoleManagerActivity.java // 游戏手柄
│               │   ├── PodControlServiceActivity.java // 实例控制
│               │   ├── ProbeNetworkActivity.java // 网络探测
│               │   ├── SensorActivity.java // 传感器
│               │   └── OthersActivity.java // 其他
│               └── util
│                   ├── AssetsUtil.java // 用于读取并解析sts.json文件中的gameId/ak/sk/token
│                   ├── FileUtil.java // 用于文件传输功能的工具类
│                   ├── PrefUtils.kt // 用于保存应用的设置选项
│                   └── ScreenUtil.java // 屏幕工具类，用于适配挖孔屏
```


其中, **sts.json** 的格式如下：
```java
{
    "gameId": "your_game_id",
    "ak": "your_ak",
    "sk": "your_sk",
    "token": "your_token"
}
```

注：FileChannel(文件通道)已经下线，请使用FileChannelExt(大文件通道)进行文件传输。

## 参考资料

火山引擎云游戏客户端 SDK 下载：https://www.volcengine.com/docs/6512/75594。

注：如果不能访问以上链接，请参考 [开通云游戏服务](https://www.volcengine.com/docs/6512/75577) 说明文档。
