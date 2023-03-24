# Message Channel SDK Demo

## 说明

Message Channel SDK为部署在火山引擎云游戏环境的游戏与客户端 APP 提供消息传输的系统解决方案，可以支持免登、支付和客户端与云端应用的互动等场景。

Message Channel SDK的具体功能如下：
- 云游戏客户端APP与云端游戏之间收发文本消息(有关客户端veGame SDK的使用方法，参考[veGame Android SDK接口说明](https://www.volcengine.com/docs/6512/75595))

本项目是火山引擎云游戏客户端Message Channel SDK的快速演示Demo。获取项目以后，开发者可以快速构建应用，体验Message Channel的主要功能；也能参考其中的代码，在实际的客户端应用中实现相似的功能。


## 环境要求

### 说明

Message Channel SDK 目前不支持本地调试，您可以参考demo自行搭建测试环境。

### 硬件要求

支持 Android 4.1（Android-16+）及以上系统。

### 软件要求

- IDE：Android Studio（推荐使用最新版本）
- 搭建 Java 环境，使用 Java 作为开发语言，JDK版本需要1.8+

## 接入流程

### 添加Maven仓库地址

1. 在project根目录下的build.gradle文件中的repositories闭包中配置Maven仓库地址，参考以下示例：

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

2. 在app模块的build.gradle文件中的dependencies中加入SDK依赖项，参考以下示例：

```java
dependencies {
    implementation 'com.bytedance.message:VeMsgChannelClient:1.0.8'
}
```

或(去除gson依赖版本)

```java
dependencies {
    implementation 'com.bytedance.message:VeMsgChannelClient:1.0.7-lite'
}
```

3. 设置java版本到1.8，参考以下示例

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

根据实际使用场景在 AndroidManifest.xml 文件中声明SDK需要的权限，参考以下示例：

```java
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

## 参考资料

Message Channel SDK 官方文档：https://www.volcengine.com/docs/6512/75598

注：如果不能访问以上链接，请参考 [开通云游戏服务](https://www.volcengine.com/docs/6512/75577) 说明文档。