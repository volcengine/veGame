# veGame SDK Demo

## 说明

本项目是火山引擎云游戏客户端 SDK 的快速演示 demo。获取项目以后，开发者可以快速构建应用，体验云游戏服务的主要功能；也能参考其中的代码，在实际的客户端应用中实现相似的功能。

## 环境要求

### 硬件要求

支持 Android 4.3（Android-19+）及以上系统的真机设备，支持 armeabi-v7a。

### 软件要求

- IDE：Android Studio（推荐使用最新版本）
- 搭建 Java 环境，使用 Java 作为开发语言，JDK版本需要1.8+

## 接入流程

### 添加Maven仓库地址

1. 在project根目录下的build.gradle文件中的repositories闭包中配置Maven仓库地址

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

2. 在应用模块的build.gradle文件中的dependencies中加入依赖项

```java
dependencies {
    implementation fileTree(include: ['*.jar'], dir: 'libs')
    // 云游戏 SDK
    implementation 'com.volcengine.vegame:vegame:1.13.0'
    implementation 'androidx.annotation:annotation:1.1.0'
        
    // 选择引用以下三种框架中的任意一种
    implementation 'com.google.code.gson:gson:2.8.5' // gson
        
    implementation 'com.alibaba:fastjson:1.1.72.android' // fastjson
        
    implementation 'com.fasterxml.jackson.core:jackson-databind:2.11.1' // jackson
    implementation 'com.fasterxml.jackson.core:jackson-core:2.11.1' //jackson
}
```

3. 设置java版本到1.8

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

根据实际使用场景在AndroidManifest.xml文件中声明SDK需要的权限

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
2. 如果APP指向Android 10及以上（targetSdkVersion >= 29），而且并未适配 Scoped Storage，那么需要
    将AndroidManifest.xml文件中的requestLegacyExternalStorage设置为true。参考：https://developer.android.com/training/data-storage/use-cases#opt-out-scoped-storage
```java
<manifest>
    <application android:requestLegacyExternalStorage="true">
    </application>
</manifest>
```
### 快速开始

#### 一、初始化VeGameEngine

建议在 Application 初始的时候调用。

```java
/**
 * veGameEngine class 
 * @param application Android上下文
 */
public void init(Application application)
```

#### 二、配置火山引擎账号ID

在申请云游戏服务之前，需要在 \app\src\main\AndroidManifest.xml 文件的 meta-data 中填入注册的火山引擎用户账号（VOLC_ACCOUNT_ID）。参考以下示例：

```xml
<meta-data
    android:name="VOLC_ACCOUNT_ID"
    android:value="21000xxxxx" />
````

#### 三、配置GamePlayConfig

```java
GamePlayConfig.Builder builder = new GamePlayConfig.Builder();

builder.userId(userId) // 用户userid
    .ak(ak) // 必填参数，用于鉴权的 ak
    .sk(sk)  // 必填参数，用于鉴权的 sk
    .token(token) // 必填参数，用于鉴权的 session
    .container(mContainer) // 必填参数，用来承载画面的 Container, 参数说明: layout 需要是FrameLayout或者FrameLayout的子类
    .roundId(intent.getStringExtra(KEY_ROUND_ID)) // 必填参数，自定义roundId
    .videoStreamProfileId(intent.getIntExtra(KEY_ClARITY_ID, 1)) // 选填参数，清晰度ID
    .gameId(intent.getStringExtra(KEY_PARAM_GAME_ID)) // 必填, 游戏Id
    .enableAcceleratorSensor(true) // 打开加速度传感器开关
    .enableGravitySensor(true) // 打开重力传感器开关
    .enableGyroscopeSensor(true) // 打开陀螺仪开关
    .enableMagneticSensor(true) // 打开磁力传感器开关
    .enableOrientationSensor(true) // 打开方向传感器开关
    .enableVibrator(true) // 打开本地振动开关
    .enableLocationService(true) // 打开本地定位功能开关
    .enableLocalKeyboard(true) // 打开本地键盘开关
    .enableFileChannel(true) // 打开文件通道开关
    .streamListener(IStreamListener streamListener); // 获取音视频流信息回调监听

GamePlayConfig gamePlayConfig = builder.build();
```

#### 四、开始游戏
```java
veGameEngine.start(gamePlayConfig, IGamePlayerListener playerListener);
```


## 目录结构

```
main
├── AndroidManifest.xml
├── java
│   └── com
│       └── volcengine
│           └── vegameengine
│               ├── FeatureActivity.kt // 用于指定gameId以体验SDK的不同特性
│               ├── GameActivity.java // 显示游戏的Activity
│               ├── GsonConverter.java // 用于SDK 传入的JSON转换的实现 
│               ├── InitApplication.java // 工程的Application 负责初始化SDK等
│               ├── MainActivity.java // 用于展示SDK的特性列表
│               ├── TestBean.kt
│               ├── WebViewActivity.kt // 用于展示火山引擎的官网
│               ├── base
│               │   ├── BaseListActivity.java
│               │   └── BaseSampleActivity.kt
│               ├── feature // 用于体验SDK不同的特性
│               │   ├── AudioServiceView.java
│               │   ├── CamaraManagerView.java
│               │   ├── ClarityServiceView.java
│               │   ├── ClipBoardServiceManagerView.java
│               │   ├── FileChannelView.java
│               │   ├── GamePadServiceView.java
│               │   ├── GroundManagerView.java
│               │   ├── LocationServiceView.java
│               │   ├── MessageChannelView.java
│               │   ├── PodControlServiceView.java
│               │   ├── SensorView.java
│               │   └── UnclassifiedView.java
│               └── util
│                   ├── DialogUtils.java 
│                   ├── Feature.java // 声明不同的特性id
│                   ├── FileUtil.java
│                   ├── ScreenUtil.java // 屏幕工具类，用于适配挖孔屏
│                   └── SizeUtils.java 
```

## 参考资料

客户端 SDK 下载：https://www.volcengine.com/docs/6512/75594

注：如果不能访问以上链接，请参考 [开通云游戏服务](https://www.volcengine.com/docs/6512/75577) 说明文档。
