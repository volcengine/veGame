# 火山引擎云游戏 Android 客户端 SDK 接入说明

本文档描述了火山引擎云游戏 Android 客户端 SDK 的使用说明，方便开发者快速接入。

## 集成准备

### SDK 下载

|  **SDK 包文件**  |  **快速接入 Demo（GitHub 仓库）**  |
| --- | --- |
| [前往云游戏控制台](https://console.volcengine.com/veGame/guidepage?activeStep=sdk) | [Quick Start Demo](https://github.com/volcengine/veGame/tree/master/QuickStart/Android) |

### 环境要求

* Android 4.3 及以上系统
* JDK 环境 1.8+
* 本文提供的示例代码均为 Java 语言


### 快速接入

1. 在 Project 根目录下的 `build.gradle` 文件中的 `repositories` 中配置了 maven 仓库地址，参考以下示例：

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

2. 在项目中引入 veGameSDK：

- （方式一）解压下载的 veGameSDK 包文件，将 `cloudgame-release.aar` 文件拷贝到 `libs` 目录下。在应用模块的 `build.gradle` 文件中的 `dependencies` 中加入：

```java
repositories {
   flatDir {dirs 'libs'}
}
dependencies {
   implementation fileTree(include: ['*.jar'], dir: 'libs')
   // 云游戏 SDK
   implementation files('libs/cloudgame-release.aar')
   implementation 'androidx.annotation:annotation:1.1.0'
   // 选择引用以下三种框架中的任意一种
   implementation 'com.google.code.gson:gson:2.8.5'
   implementation 'com.alibaba:fastjson:1.1.72.android'
   implementation 'com.fasterxml.jackson.core:jackson-databind:2.11.1'
   implementation 'com.fasterxml.jackson.core:jackson-core:2.11.1'
}
```

- （方式二）在应用模块的 build.gradle 文件中的 dependencies 中添加 veGameSDK 的依赖，参考以下示例：

```java
implementation 'com.volcengine.vegame:vegame:1.22.0'
```

- （方式三，非必须）在应用模块的 build.gradle 文件中的 dependencies 中添加 veGameSDK 以及插件包的依赖，参考以下示例：

```java
implementation 'com.volcengine.vegame:vegame:1.22.0'
implementation 'com.volcengine.vegame:core-full:1.22.0'
```

3. 设置 Java 版本到 1.8，参考以下示例：

```java
android {
    // ...
    compileOptions {
        sourceCompatibility JavaVersion.VERSION_1_8
        targetCompatibility JavaVersion.VERSION_1_8
    }
}
```

**设置火山引擎账号**

在 `AndroidManifest.xml` 文件中设置注册的火山引擎用户账号（可通过火山引擎官网页面右上角 **用户 > 账号管理 > 主账号信息** 获取），参考以下示例：

```java
<meta-data
    android:name="VOLC_ACCOUNT_ID"
    android:value="21000xxxxx" />
```    

**权限声明**

根据实际场景在 `AndroidManifest.xml` 文件中声明 SDK 需要的权限，参考以下示例：

```java
//网络权限，使用场景：音视频传输等
<uses-permission android:name="android.permission.INTERNET" />

//WiFi网络状态，使用场景：用户手机网络状态变化监听
<uses-permission android:name="android.permission.CHANGE_WIFI_STATE" />
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
<uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />

//设置播放模式的权限：外放 / 听筒
<uses-permission android:name="android.permission.MODIFY_AUDIO_SETTINGS" />

//同步定位信息，使用场景：当有些游戏需要获取用户的地理位置时，我们需要获取用户的地理位置信息
//并传送给远端Pod
<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
<uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
<uses-permission android:name="android.permission.ACCESS_LOCATION_EXTRA_COMMANDS" />

//读写存储
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
```

> 说明：
>
> 1. 存储写入权限需动态申请。参考：[https://developer.android.com/training/permissions/requesting](https://developer.android.com/training/permissions/requesting)
> 2. 如果 App 指向 Android 10 以上 (targetSdkVersion >= 29)，而且还未适配 “Scoped Storage”。请将 `AndroidManifest.xml` 中的 `requestLegacyExternalStorage` 设置为 `true`。参考官方文档: [https://developer.android.com/training/data-storage/use-cases#opt-out-scoped-storage](https://developer.android.com/training/data-storage/use-cases#opt-out-scoped-storage)

```java
<manifest>
  <application android:requestLegacyExternalStorage="true">
  </application>
</manifest>
```

## 接口说明

### 获取 veGameEngine 的状态

描述：获取当前 veGameEngine 的状态，状态说明如下：

|  **状态**  |  **说明**  |
| --- | --- |
| 1. (0x0001) | 未初始化状态 |
| 2. (0x0002) | 初始化成功 |
| 3. (0x0004) | 正常调用 start 接口 |
| 4. (0x0008) | 收到云端首帧 |
| 5. (0x0010) | 停止拉取云端音视频流，但不会改变云端的运行状态 |

参考以下示例：

```java
// veGameEngine class

int getStatus()
```

```java
// veGameEngine class

void addCloudCoreManagerListener(@NonNull ICloudCoreManagerStatusListener listener)

void removeCloudCoreManagerListener(@NonNull ICloudCoreManagerStatusListener listener)

public interface ICloudCoreManagerStatusListener {
    void onInitialed();
}
```

#### addCloudCoreManagerListener(listener)

描述：设置 veGameEngine 生命周期回调监听。

#### removeCloudCoreManagerListener(listener)

描述：取消 veGameEngine 生命周期回调监听。

#### ICloudCoreManagerStatusListener

描述：veGameEngine 生命周期回调。

|  **状态**  |  **说明**  |
| --- | --- |
| onInitialed() | SDK 初始化成功回调 |


### 初始化

描述：初始化 veGameEngine，建议在 Application 初始的时候调用。

> 说明：
> - 第一次初始化时，SDK 会进行一次资源异步加载。如果在初始化时资源加载失败，不需要重新调用 `init`。SDK 会在调用 `start` 接口启动游戏时检查资源是否已成功加载，如果没有，则会重新加载。资源加载完成后会由 `ICloudCoreManagerStatusListener` 的 `onInitialed()` 回调。
> - 初始化只需要执行一次。特别注意：多进程会多次执行 Application 的 `onCreate`，需要在 `onCreate` 中添加当前进程判断，详情可参考 [Github Demo](https://github.com/volcengine/veGame/tree/master/QuickStart/Android)。

```java
/**
 * veGameEngine class
 * @param application Android上下文
 */
public void init(Application application)
```

#### 火山引擎账号

在申请云游戏服务之前，需要在 `\app\src\main\AndroidManifest.xml` 文件的 `meta-data` 中填入注册的火山引擎用户账号（`VOLC_ACCOUNT_ID`）。参考以下示例：

```java
<meta-data
    android:name="VOLC_ACCOUNT_ID"
    android:value="21000xxxxx" />
```    

> 可通过火山引擎官网页面右上角 **用户 > 账号管理 > 主账号信息** 获取用户账号。

### 获取设备唯一标识

描述：获取用户终端设备的唯一标识。

> 最佳实践：保存获取到的设备唯一标识到业务的服务端。在通过调用服务端 [PreAllocateResource](https://www.volcengine.com/docs/6512/107714) 接口锁定资源、请求游戏服务时，透传保存的设备唯一标识，用于确定是否支持游戏多开。

参考以下示例：

```java
// veGameEngine class

public String getServiceDeviceId()
```

### 网络探测

描述：在开始游戏之前，可使用云游戏 SDK 提供的网络测速服务对客户端网络质量进行探测，提示用户当前的网络状态。

#### 启动网速探测

使用 `veGameEngine` 类中的 `probeStart` 接口启动网速探测，探测过程以及探测结果通过 `IProbeNetworkListener` 回调接口进行监听。

参考示例：

```java
// veGameEngine class

public void probeStart(@NonNull GamePlayConfig config, @NonNull IProbeNetworkListener listener)
```

#### IProbeNetworkListener

接收网络探测流程和状态监听的回调。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onProbeStarted() | 启动网络测速回调 |
| onProbeProgress(ProbeStats stats) | 网络测速过程中检测状态的回调，此时 [ProbeStats](#测速结果描述) 是中间测试状态，仅供参考 |
| onProbeCompleted(ProbeStats stats, int quality) | 网络测速成功结束回调，此时 [ProbeStats](#测速结果描述) 标识最终的网络测试结果，quality 标识当前网络推荐值，有以下三个档位： <br> 1（网络极好，可以很流畅地玩游戏） <br> 2（网络较好，可以玩游戏） <br> 3（网络较差，不推荐玩游戏） |
| onProbeError(int err, String message) | 网络测速异常结束回调，err 标识错误码，message 标识错误信息；错误码说明如下：  <br>1（探测过程网络环境出错，无法完成探测）  <br>2（探测过程被终止取消）  <br>3（探测过程结束，但没有任何探测结果，通常情况下不会发生） |

参考示例：

```java
public interface IProbeNetworkListener {
    public static final int QUALITY_EXCELLENT = 1;
    public static final int QUALITY_GOOD = 2;
    public static final int QUALITY_POOR = 3;

    public static final int ERROR_BAD_NETWORK = 1;
    public static final int ERROR_CANCEL_BY_USER = 2;
    public static final int ERROR_EMPTY_STATS = 3;

    /**
     * 启动探测回调
     */
    void onProbeStarted();

    /**
     * 探测过程检测状态回调
     *
     * @param stats 探测过程的节点网速状态
     *
     */
    void onProbeProgress(ProbeStats stats);

    /**
     * 探测结束回调
     *
     * @param stats 最终的网络测试结果
     * @param quality 当前网络推荐值，有三个档位（Excellent/Good/Poor）
     *
     */
    void onProbeCompleted(ProbeStats stats, int quality);

    /**
     * 探测结束回调
     *
     * @param err 错误码
     * @param message 错误日志消息
     *
     */
    void onProbeError(int err, String message);
}
```

#### 测速结果描述

`ProbeStats` 描述测速的中间状态信息，包含以下字段：

|  **字段名称**  |  **类型**  |  **描述**  |
| --- | --- |  --- |
| rtt | Int | 往返时延时长；单位：毫秒 |
| downloadBandwidth | Int | 下行带宽；单位：Kbit/s |
| downloadJitter | Int | 下行网络抖动时长；若为无效数据，数值为-1；单位：毫秒 |
| downloadLossPercent | Double | 下行丢包率；百分比 |
| uploadBandwidth | Int | 上行带宽；单位：Kbit/s |
| uploadJitter | Int | 上行网络抖动时长；若为无效数据，数值为-1；单位：毫秒 |
| uploadLossPercent | Double | 上行丢包率；百分比 |

参考示例：

```java
public class ProbeStats {
    private int rtt;
    private int downloadBandwidth;
    private int downloadJitter;
    private double downloadLossPercent;
    private int uploadBandwidth;
    private int uploadJitter;
    private double uploadLossPercent;

    public ProbeStats() {}

    public int getRtt() {
        return rtt;
    }

    public void setRtt(int rtt) {
        this.rtt = rtt;
    }

    public int getDownBandwidth() {
        return downloadBandwidth;
    }

    public void setDownBandwidth(int downloadBandwidth) {
        this.downloadBandwidth = downloadBandwidth;
    }

    public int getDownloadJitter() {
        return downloadJitter;
    }

    public void setDownloadJitter(int downloadJitter) {
        this.downloadJitter = downloadJitter;
    }

    public double getDownloadLossPercent() {
        return downloadLossPercent;
    }

    public void setDownloadLossPercent(double downloadLossPercent) {
        this.downloadLossPercent = downloadLossPercent;
    }

    public int getUploadBandwidth() {
        return uploadBandwidth;
    }

    public void setUploadBandwidth(int uploadBandwidth) {
        this.uploadBandwidth = uploadBandwidth;
    }

    public int getUploadJitter() {
        return uploadJitter;
    }

    public void setUploadJitter(int uploadJitter) {
        this.uploadJitter = uploadJitter;
    }

    public double getUploadLossPercent() {
        return uploadLossPercent;
    }

    public void setUploadLossPercent(double uploadLossPercent) {
        this.uploadLossPercent = uploadLossPercent;
    }
}
```

#### 取消网速探测

描述：在网速探索过程中，可使用 `veGameEngine` 类中的 `probeInterrupt` 接口来取消网速探测。网速探测终止完成后，会通过 [IProbeNetworkListener](#iprobenetworklistener) 接口收到 `onProbeError` 回调，错误码是 2，标识探测过程被取消。

参考示例：

```java
// veGameEngine class

public void probeInterrupt()
```

### 开始播放

描述：当成功初始化完成后，可以调用 `start()`，拉流开始播放。

入参：`config`

返回值：开始播放触发的回调为 `IStreamListener` 的 `onFirstRemoteVideoFrame`

参考以下示例：

```java
public void start(@NonNull GamePlayConfig config, @NonNull IPlayerListener playerListener)
```

`config` 字段描述如下:

|  **名称**  |  **类型**  |  **是否必填**  |  **说明**  |
| --- | --- | --- | --- |
| userId | String | 是 | 自定义客户端用户 ID，用于标识用户在游戏房间中的身份，命名规则如下：  <br>-最大长度为64位的非空字符串，支持的字符集范围为:  <br>-大写字母 A ~ Z  <br>-小写字母 a ~ z  <br>-数字 0 ~ 9  <br>-下划线、减号 |
| ak | String | 是 | 用于用户鉴权的临时 Access Key，通过调用服务端 STSToken 接口获取，参考 [生成临时 Token 接口](https://www.volcengine.com/docs/6512/75588) |
| sk | String | 是 | 用于用户鉴权的临时 Secret Key，通过调用服务端 STSToken 接口获取，参考 [生成临时 Token 接口](https://www.volcengine.com/docs/6512/75588) |
| token | String | 是 | 用于用户鉴权的临时 Token，通过调用服务端 STSToken 接口获取，参考 [生成临时 Token 接口](https://www.volcengine.com/docs/6512/75588) |
| gameId | String | 是 | 游戏 ID，可通过火山引擎云游戏控制台『游戏管理』页面获取，例如：1428112352161312345；当传入 customGameId 时，可不传入（以 game_id 优先） |
| customGameId | String | 是 | 注册游戏时指定的用户自定义游戏 ID；当传入 gameId 时，可不传入 |
| roundId | String | 是 | 当次游戏生命周期的标识符，命名规则如下：  <br>-最大长度为128位的非空字符串，支持的字符集范围为:  <br>-大写字母 A ~ Z  <br>-小写字母 a ~ z  <br>-数字 0 ~ 9  <br>-下划线、减号 |
| roomType | Int | 否 | 启动游戏的场景，用于控制是否开启多人游戏及游戏控制权转移：  <br>0（单用户，默认）  <br>1（单房间多用户，不可转移游戏控制权）  <br>2（单房间多用户, 可转移游戏控制权） |
| role | Role | 否 | 启动游戏时，游戏玩家的角色：  <br>Role.PLAYER（操作者）  <br>Role.VIEWER（观看者）  <br>注：当游戏场景类型为1和2时，如不指定游戏玩家角色，则游戏玩家的角色默认为观看者（游戏场景类型为1或2时才有多用户之分） |
| container | FrameLayout 或其子类 | 是 | 用来承载画面的 Container |
| planId | String | 否 | 火山侧套餐 ID，可通过调用服务端 [ListResourceSet](https://www.volcengine.com/docs/6512/102116) 接口获取（configuration_code 字段） |
| enableFileChannel | Boolean | 否 | 是否使能文件传输通道（默认为 false） |
| enableLocalKeyboard | Boolean | 否 | 是否使能手机本地输入法，（默认为 false）  <>br>注：焦点拦截、以及 activity 设置能弹出软键盘等，都会导致本地键盘无法弹出 |
| keyBoardEnable | Boolean | 否 | 默认输入法开启，true ：开启， false: 关闭（默认为 true） |
| enableMagneticSensor | Boolean | 否 | 是否使能磁力传感器（默认为 false） |
| enableAcceleratorSensor | Boolean | 否 | 是否使能加速度传感器（默认为 false） |
| enableGravitySensor | Boolean | 否 | 是否使能重力传感器（默认为 false） |
| enableGyroscopeSensor | Boolean | 否 | 是否使能陀螺仪传感器（默认为 false） |
| enableLocationService | Boolean | 否 | 是否使能定位服务（默认为 false） |
| enableVibrator | Boolean | 否 | 是否开启震动（默认为 true） |
| videoStreamProfileId | Int | 否 | 游戏视频流清晰度 ID；如不传入，则使用默认清晰度（清晰度档位 ID 列表和详细说明，参考 [清晰度切换](#清晰度切换)） |
| userTag | String | 否 | 用户标签，由业务方指定，可通过调用服务端 CreateUserTag 接口创建，参考 [创建用户属性](https://www.volcengine.com/docs/6512/102176) |
| reservedId | String | 否 | 资源预锁定 ID，通过调用服务端 [PreAllocateResource](https://www.volcengine.com/docs/6512/107714) 接口获取（如不传入，将在开始播放成功 onPlaySuccess 回调中返回）；资源预锁定 ID 用于申请火山引擎云游戏的每次服务，建议业务记录此 ID，用于每次服务会话的唯一标识，可用于调试和问题排查 |
| autoRecycleTime | Int | 否 | 设置无操作自动回收服务时长，单位秒（如不设置或设置为0，则使用默认时长300秒，支持设置的上限值为7200，即2小时） |
| userProfilePath | String[] | 否 | 保存用户游戏配置文件的路径列表（如需在游戏进行中通过 [setUserProfilePath](#保存还原用户配置信息) 接口设置保存配置文件的路径，需要配置该参数） |
| extra | Map<String,String> | 否 | 根据业务需要，自定义的扩展参数；详细信息，参考以下 [自定义扩展参数列表](#自定义扩展参数列表) |

`config` 最简配置参考示例（必填参数）：

```java
GamePlayConfig.Builder builder = new GamePlayConfig.Builder();
builder.userId("your_user_id")
       .container(mContainer)
       .ak("your_ak")
       .sk("your_sk")
       .token("token")
       .gameId("gameId")
       .roundId("game_round_id");
GamePlayConfig config = builder.build();    
```

```java
config 完整配置参考示例：
GamePlayConfig.Builder builder = new GamePlayConfig.Builder();
GamePlayConfig config = builder.userId("your_user_id")
        .container(layout)
        .ak("your_ak")
        .sk("your_sk")
        .token("token")
        .gameId("gameId")
        .roundId("game_round_id")
        .roomType(0)
        .planId("ARMSoC_General")
        .userTag("your_user_tag")
        .reservedId("reserved_id")
        .enableFileChannel(true)
        .enableLocalKeyboard(true)
        .keyBoardEnable(true)
        .enableMagneticSensor(true)
        .enableAcceleratorSensor(true)
        .enableGravitySensor(true)
        .enableGyroscopeSensor(true)
        .enableLocationService(true)
        .enableOrientationSensor(true)
        .enableVibrator(true)
        .videoStreamProfileId(1)
        .autoRecycleTime(120)
        .userProfilePath("/a/b/", "/c/d/")
        .extra(new HashMap<String,String>());
```

#### 自定义扩展参数列表

根据业务需要，在申请云游戏服务时，可通过 `extra` 参数透传的自定义业务参数和相关说明如下：

|  **参数名**  |  **类型**  |  **用途**  |
| --- | --- | --- |
| debug_info | String | 开启日志上传功能所需要的对象存储信息  <br>注：开启日志上传至对象存储功能，需向云游戏技术支持提供火山引擎云游戏业务 ID（可通过云游戏控制台 **业务详情** 页面获取）  |
| serviceReserveTime | Int | 指定用户退出游戏后服务端保留游戏运行资源的时长（单位：秒）；即调用客户端本地 `stop` 接口将不会引起实例资源退出，需要用户手动调用服务端 [GameStop](https://www.volcengine.com/docs/6512/102175) 接口释放实例资源 |
| gameArchiveUrl | String | 下载用户游戏存档文件的完整链接，文件格式为 .tar.gz，解压内容：  <br>`data/data/<pkgname>/`  <br>`sdcard/Android/data/<pkgname>/` |
| gameArchiveKey | String | 使用对象存储时，保存和下载用户游戏存档文件的 Key |
| gameArchivePathList | String[] | 使用对象存储时，保存和下载用户游戏存档文件的绝对路径列表，例如：  <br>`["/data/data/com.xx/files/1.txt","/data/data/com.xx/cache/2.txt"]` |
| userFrameworkApp | String[] | 指定运行游戏需要的伴随程序列表  <br>注：已上传的伴随程序列表以及伴随程序包名可通过调用服务端 [伴随程序管理](https://www.volcengine.com/docs/6512/128774) 接口获取 |
| characteristics | String | 设备类型  |

参考示例：

```java
"extra": {
        "debug_info":{
           "schema":"ali_oss",
           "config":{
              "ak":"your_ak",
              "sk":"your_sk",
              "bucket":"your_bucket",
              "endpoint":"https://oss-cn-hangzhou.aliyuncs.com"
            },
            "key": "test/adc.txt"
         },
        "serviceReserveTime": 60,
        "gameArchiveUrl": "https://www.exapmple.com/data",
        "gameArchiveKey": "example.tar.gz",
        "gameArchivePathList": "["/data/data/com.demo/files","/data/data/com.demo/cache/preferences.xml"]",
        "userFrameworkApp":"["com.pkg1","com.pkg2","com.pkg3"]",
        "characteristics": "tablet"
    }
```

#### IGamePlayerListener

拉流播放状态回调监听：

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onServiceInit() | 加入房间前回调，用于获取并初始化各个功能服务，例如设置各种事件监听回调 |
| onPlaySuccess(String roundId, int videoStreamProfileId, Map<String, String> extra, String gameId, String reservedId) | 播放成功回调：  <br>roundId：当次游戏生命周期标识符  <br>videoStreamProfileId：当前游戏画面的清晰度，首帧渲染到画面时触发该回调  <br>extra：自定义的扩展参数  <br>gameId：游戏 ID  <br>reservedId：资源预锁定 ID  |
| onNetworkChanged(int type) | 网络连接类型和状态切换回调：  <br>-1: 网络连接类型未知0: 网络连接已断开  <br>1: 网络类型为 LAN  <br>2: 网络类型为 Wi-Fi（包含热点）  <br>3: 网络类型为 2G 移动网络  <br>4: 网络类型为 3G 移动网络  <br>5: 网络类型为 4G 移动网络  <br>6: 网络类型为 5G 移动网络 |
| onWarning(int code, String message) | SDK 内部产生告警回调，参考 [警告码](#警告码) |
| onError(int code, String message) | SDK 内部产生错误回调，参考 [错误码](错误码)；注意：在出现报错时，需要调用 stop() 接口，恢复到初始状态，排查问题后，再调用 start() |


参考示例：

```java
public interface IGamePlayerListener {

    void onServiceInit();

    void onPlaySuccess(String roundId, int videoStreamProfileId, Map<String, String> extra, String gameId, String reservedId);

    void onNetworkChanged(int type);

    void onWarning(int code, String message);

    void onError(int code, String message);

}
```

#### IStreamListener

获取音视频流信息回调监听。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onFirstAudioFrame(String uid) | 订阅视频流到收到音频首帧回调：  <br>uid：远端实例视频流 ID |
| onFirstRemoteVideoFrame(String uid) | 订阅视频流到收到视频首帧回调：  <br>uid：远端实例视频流 ID |
| onStreamStarted() | 开始播放回调 |
| onStreamPaused() | 调用 pause()，暂停播放后的回调 |
| onStreamResumed() | 调用 resume() 或 muteAudio(false)，恢复播放后的回调 |
| onStreamStats(StreamStats streamStats) | 视频流的当前性能状态回调（2秒周期内音视频网路状态的回调，可用于内部数据分析或监控）：  <br>streamStats：远端数据间视频流的性能状态，参考以下 [StreamStats 类型说明](#streamstats-类型说明) |
| onStreamConnectionStateChanged(int state) | 视频流连接状态变更回调：  <br>1：连接断开  <br>2：首次连接，正在连接中  <br>3：首次连接成功  <br>4：连接断开后重新连接中  <br>5：连接断开后重连成功  <br>6：网络连接断开超过 10 秒，仍然会继续重连 |
| onNetworkQuality(int quality) | 游戏中网络质量回调，每隔 2 秒上报一次网络质量评级：  <br>quality：网络质量评级（可根据当前返回的网络质量评级进行推流参数降级或者终止拉流；详细信息，参考以下 [onNetworkQuality 相关信息](#onNetworkquality-相关信息)） |
| onDetectDelay(long  elapse) | 当前操作延时回调，单位毫秒（操作延时获取是指在操作时发送到远端的消息，本地记录的时间戳，收到远端视频流会带上操作延时的标记，从而计算出来的一个值，该值可以理解为操作和对应画面渲染更新的一个差值） |
| onRotation(int rotation) | 客户端旋转回调 |
| onPodExit(int reason, String msg) | 实例端退出回调，参考以下 [onPodExit 相关信息](#onpodexit-相关信息) |

参考示例：

```java
public GamePlayConfig.Builder streamListener(IStreamListener listener)
```

```java
public interface IStreamListener {

    void onFirstAudioFrame(String uid);

    void onFirstRemoteVideoFrame(String uid);

    void onStreamStarted();

    void onStreamPaused();

    void onStreamResumed();

    void onStreamStats(StreamStats streamStats);

    void onStreamConnectionStateChanged(int state);

    void onNetworkQuality(int quality)

    void onDetectDelay(long elapse);

    void onRotation(int rotation);

    void onPodExit(int reason, String msg);
}
```

#### onPodExit 相关信息

|  **错误码**  |  **错误信息**  |  **说明**  |
| --- | --- | --- |
| 40000 | ERROR_GAME_ABNORMAL_EXIT | 游戏停止。原因：云端服务异常退出。 |
| 40001 | ERROR_POD_EXIT_CRASH  | 游戏停止。原因：服务端游戏崩溃。建议：请尝试复现游戏在本地设备是否运行异常，如本地未复现，可以联系火山引擎云游戏服务技术支持。 |
| 40002 | ERROR_GAME_OFFLINE | 游戏停止。原因：当前游戏被下架。建议：请通过火山引擎云游戏控制台『游戏管理』页面或调用服务端 [ListGame]() 接口查看当前游戏的上架状态。 |
| 40003 | ERROR_GAME_EXIT_TIMEOUT | 游戏停止。原因：超时退出。 |
| 40004 | ERROR_GAME_STOPPED_IDLE | 游戏停止。原因：用户长时间未操作，服务端自动断开游戏连接。建议：客户端需要提示用户长时间无操作被踢下线。 |
| 40005 | ERROR_GAME_STOPPED_RESET | 游戏停止。原因：服务端主动重置了游戏。建议：请联系火山引擎云游戏服务对接人员。 |
| 40006 | ERROR_GAME_STOPPED_API | 游戏停止。原因：服务端主动停止了游戏。 |
| 40007 | ERROR_GAME_STOPPED_DUPLICATE_START | 游戏停止。原因：某个 start() 请求使用了与当前游戏相同的 userId 和 gameId，导致当前游戏停止。建议：请检查调用 start() 接口的参数是否重复。如果需要重新启动被停止的游戏，请再次使用相同的 userId 和 gameId 调用 start() 接口。 |
| 40008 | ERROR_POD_STOPPED_BACKGROUND_TIMEOUT | 游戏停止。原因：游戏切换后台，超过指定的保活时长未切换前台，服务端自动断开游戏连接。建议：客户端需要提示用户因为游戏切后台超过保活时长，连接断开。 |

#### onNetworkQuality 相关信息

网络质量评级说明：

|  **网络质量评级**  |  **评级名称**  |  **说明**  |
| --- | --- | --- |
| 0 | NETWORK_QUALITY_UNKNOWN | 表示当前网络状况未知，无法判断网络质量 |
| 1 | NETWORK_QUALITY_EXCELLENT | 表示当前网络状况极佳，能够高质量承载当前业务 |
| 2 | NETWORK_QUALITY_GOOD | 表示当前网络状况良好，能够较好地承载当前业务 |
| 3 | NETWORK_QUALITY_POOR | 表示当前网络状况有轻微劣化，但不影响正常使用 |
| 4 | NETWORK_QUALITY_BAD | 表示当前网络质量欠佳，会影响当前业务的主观体验 |
| 5 | NETWORK_QUALITY_VERY_BAD | 表示当前网络已经无法承载当前业务的媒体流，需要采取相应策略，比如降低媒体流的码率或者更换网络 |
| 6 | NETWORK_QUALITY_DOWN | 表示当前网络完全无法正常通信 |

#### StreamStats 类型说明

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| getReceivedVideoBitRate() | 获取视频接收码率瞬时值（单位 kbps） |
| getReceivedAudioBitRate() | 获取音频接收码率瞬时值（单位 kbps） |
| getDecoderOutputFrameRate() | 获取解码器输出帧率（单位 fps） |
| getRendererOutputFrameRate() | 获取渲染帧率（单位 fps） |
| getReceivedResolutionHeight() | 获取远端视频流高度 |
| getReceivedResolutionWidth() | 获取远端视频流宽度 |
| getVideoLossRate() | 获取丢包率 |
| getRtt() | 获取客户端与服务端往返时延（单位 ms） |
| getStallCount() | 获取卡顿次数 |
| getStallDuration() | 获取卡顿时长（统计周期内的视频卡顿总时长，单位 ms） |
| getFrozenRate() | 获取卡顿率（视频卡顿的累计时长占视频总有效时长的百分比） |

参考示例：

```java
public class StreamStats {

    private int receivedVideoBitRate;

    private int receivedAudioBitRate;

    private int decoderOutputFrameRate;

    private int rendererOutputFrameRate;

    private int receivedResolutionHeight;

    private int receivedResolutionWidth;

    private float videoLossRate;

    private int rtt;

    private int stallCount;

    private int stallDuration;

    private int frozenRate;

    public int getReceivedVideoBitRate() {
        return receivedVideoBitRate;
    }

    public int getReceivedAudioBitRate() {
        return receivedAudioBitRate;
    }

    public int getDecoderOutputFrameRate() {
        return decoderOutputFrameRate;
    }

    public int getRendererOutputFrameRate() {
        return rendererOutputFrameRate;
    }

    public int getReceivedResolutionHeight() {
        return receivedResolutionHeight;
    }

    public int getReceivedResolutionWidth() {
        return receivedResolutionWidth;
    }

    public int getVideoLossRate() {
        return videoLossRate;
    }

    public int getRtt() {
        return rtt;
    }

    public int getStallCount() {
        return stallCount;
    }

    public int getStallDuration() {
        return stallDuration;
    }

    public int getFrozenRate() {
        return frozenRate;
    }
}
```

#### LocalStreamStats

本地音频流数据统计（LocalAudioStreamStats）：

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| audioLossRate | Float | 音频丢包率；此次统计周期内的音频上行丢包率，单位为 % ，取值范围为 [0,1]  |
| sendKBitrate | Float | 发送码率；此次统计周期内的音频发送码率，单位为 kbps  |
| recordSampleRate | Integer | 采集采样率；此次统计周期内的音频采集采样率信息，单位为 Hz  |
| statsInterval | Integer | 统计间隔；此次统计周期的间隔，单位为 ms  |
| rtt | Integer | 往返时延；单位为 ms  |
| numChannels | Integer | 音频声道数 |
| sentSampleRate | Integer | 音频发送采样率；此次统计周期内的音频发送采样率信息，单位为 Hz  |

本地视频流数据统计（LocalVideoStreamStats）：

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| sentKBitrate | Float | 发送码率；此次统计周期内的视频发送码率，单位为 kbps |
| inputFrameRate | Integer | 采集帧率；此次统计周期内的视频采集帧率，单位为 fps  |
| sentFrameRate | Integer | 发送帧率；此次统计周期内的视频发送帧率，单位为 fps  |
| encoderOutputFrameRate | Integer | 编码器输出帧率；当前编码器在此次统计周期内的输出帧率，单位为 fps  |
| rendererOutputFrameRate | Integer | 本地渲染帧率；此次统计周期内的本地视频渲染帧率，单位为 fps |
| targetKBitrate | Integer | 目标发送码率；此次统计周期内的视频目标发送码率，单位为 kbps  |
| targetFrameRate | Integer | 目标发送帧率；当前编码器在此次统计周期内的目标发送帧率，单位为 fps  |
| statsInterval | Integer | 统计间隔；单位为 ms  |
| videoLossRate | Float | 视频丢包率；此次统计周期内的视频上行丢包率，取值范围： [0,1]  |
| rtt | Integer | 往返时延；单位为 ms  |
| encodedBitrate | Integer | 视频编码码率；此次统计周期内的视频编码码率，单位为 kbps   |
| encodedFrameWidth | Integer | 视频编码宽度；单位为 px   |
| encodedFrameHeight | Integer | 视频编码高度；单位为 px   |
| encodedFrameCount | Integer | 此次统计周期内发送的视频帧总数 |
| codecType | Integer | 视频的编码类型 |

参考示例：

```java
public class LocalStreamStats {

    public final LocalVideoStreamStats mLocalVideoStreamStats;
    public final LocalAudioStreamStats mLocalAudioStreamStats;

    public LocalStreamStats(LocalVideoStreamStats localVideoStreamStats, LocalAudioStreamStats localAudioStreamStats) {
        mLocalVideoStreamStats = localVideoStreamStats;
        mLocalAudioStreamStats = localAudioStreamStats;
    }

    public static class LocalVideoStreamStats {
        public float sentKBitrate;
        public int inputFrameRate;
        public int sentFrameRate;
        public int encoderOutputFrameRate;
        public int rendererOutputFrameRate;
        public int targetKBitrate;
        public int targetFrameRate;
        public int statsInterval;
        public float videoLossRate;
        public int rtt;
        public int encodedBitrate;
        public int encodedFrameWidth;
        public int encodedFrameHeight;
        public int encodedFrameCount;
        public int codecType;

        public LocalVideoStreamStats(float sentKBitrate, int inputFrameRate, int sentFrameRate,
                                     int encoderOutputFrameRate, int rendererOutputFrameRate,
                                     int targetKBitrate, int targetFrameRate, int statsInterval,
                                     float videoLossRate, int rtt, int encodedBitrate, int encodedFrameWidth,
                                     int encodedFrameHeight, int encodedFrameCount, int codecType) {
            this.sentKBitrate = sentKBitrate;
            this.inputFrameRate = inputFrameRate;
            this.sentFrameRate = sentFrameRate;
            this.encoderOutputFrameRate = encoderOutputFrameRate;
            this.rendererOutputFrameRate = rendererOutputFrameRate;
            this.targetKBitrate = targetKBitrate;
            this.targetFrameRate = targetFrameRate;
            this.statsInterval = statsInterval;
            this.videoLossRate = videoLossRate;
            this.rtt = rtt;
            this.encodedBitrate = encodedBitrate;
            this.encodedFrameWidth = encodedFrameWidth;
            this.encodedFrameHeight = encodedFrameHeight;
            this.encodedFrameCount = encodedFrameCount;
            this.codecType = codecType;
        }
    }

    public static class LocalAudioStreamStats {

        public float audioLossRate;
        public float sendKBitrate;
        public int recordSampleRate;
        public int statsInterval;
        public int rtt;
        public int numChannels;
        public int sentSampleRate;


        public LocalAudioStreamStats(float audioLossRate, float sendKBitrate, int recordSampleRate,
                                     int statsInterval, int rtt, int numChannels, int sentSampleRate) {
            this.audioLossRate = audioLossRate;
            this.sendKBitrate = sendKBitrate;
            this.recordSampleRate = recordSampleRate;
            this.statsInterval = statsInterval;
            this.rtt = rtt;
            this.numChannels = numChannels;
            this.sentSampleRate = sentSampleRate;
        }
    }
}  
```

#### 错误码

启动游戏可能返回的错误码和相关说明如下：

|  **错误码**  |  **错误信息**  |  **说明**  |
| --- | --- | --- |
| 10000 | ERROR_START_GENERAL | 开始游戏失败。原因：通用错误。建议：请检查开始游戏 start() 接口参数。 |
| 10001 | ERROR_START_AUTHENTICATION_FAILED | 开始游戏失败。原因：火山引擎服务鉴权失败。建议：检查您的 AK、SK、Token 生成，参考 [生成临时 Token 接口](https://www.volcengine.com/docs/6512/75588)。 |
| 10002 | ERROR_START_GAME_ID_NOT_EXIST | 开始游戏失败。原因：当前游戏 ID 或 自定义游戏 ID 不存在。建议：请通过火山引擎云游戏控制台『游戏管理』页面获取正确的游戏 ID。 |
| 10003 | ERROR_START_GAME_ID_NOT_READY | 开始游戏失败。原因：当前游戏尚在适配中。建议：游戏适配需要一定时间，如需加急，可联系火山引擎云游戏服务对接人员。 |
| 10004 | ERROR_START_CONFIGURATION_CODE_NOT_EXIST | 开始游戏失败。原因：资源套餐 ID 不存在。建议：可通过调用服务端 [ListResourceSet](https://www.volcengine.com/docs/6512/102116) 接口获取（configuration_code 字段）获取正确的套餐信息。 |
| 10005 | ERROR_START_CONFIGURATION_CODE_NOT_REDAY | 开始游戏失败。原因：游戏尚未配置资源套餐或套餐 ID 无效。建议：请通过火山引擎云游戏控制台『游戏管理』页面为游戏配置部署策略。 |
| 10006 | ERROR_START_RESOURCE_NOT_READY | 开始游戏失败。原因：当前业务没有订购资源。建议：请通过火山引擎云游戏控制台『资源管理』页面购买资源。 |
| 10007 | ERROR_START_RESOURCE_CAPACITY_NOT_ENOUGH | 开始游戏失败。原因：当前游戏资源用量超限或无可用资源。建议：请通过火山引擎云游戏控制台『资源管理』页面购买资源。 |
| 10009 | ERROR_START_AUTHENTICATION_KEY_FAILED | 开始游戏失败。原因：火山引擎服务鉴权失败。建议：请检查临时鉴权 Token 的有效期，参考 [生成临时 Token 接口](https://www.volcengine.com/docs/6512/75588)。 |
| 10011 | ERROR_START_CONNECTION_ENDED | 开始游戏失败。原因：在调用 start() 接口启动游戏、启动成功回调触发之前，游戏被停止（可能原因包括：服务端游戏崩溃导致开始游戏失败、调用了服务端 [GameStop](https://www.volcengine.com/docs/6512/102175) 接口停止游戏服务）。 |
| 10012 | ERROR_START_RESERVED_ID_NOT_FOUND | 开始游戏失败。原因：资源预锁定 ID（reservedId）不存在。建议：请确保指定的 reservedId 正确；可通过调用服务端 [PreAllocateResource](https://www.volcengine.com/docs/6512/107714) 接口获取。 |
| 10013 | ERROR_START_RESERVED_ID_EXPIRED | 开始游戏失败。原因：资源预锁定 ID（reservedId）已失效。建议：可通过调用服务端 [PreAllocateResource](https://www.volcengine.com/docs/6512/107714) 接口重新获取。 |
| 10014 | ERROR_START_RESERVED_ID_ALREADY_USED_RELEASE | 开始游戏失败。原因：资源预锁定 ID（reservedId）已被使用且已释放。建议：可通过调用服务端 [PreAllocateResource](https://www.volcengine.com/docs/6512/107714) 接口重新获取。 |
| 10015 | ERROR_START_RESERVED_ID_USING | 开始游戏失败。原因：资源预锁定 ID（reservedId）正在被使用中。建议：可通过调用服务端 [PreAllocateResource](https://www.volcengine.com/docs/6512/107714) 接口重新获取。 |
| 10016 | ERROR_START_RESERVED_ID_MISMATCH_PREPARE | 开始游戏失败。原因：资源预锁定 ID（reservedId）与调用 [PreAllocateResource](https://www.volcengine.com/docs/6512/107714) 接口时指定的参数不一致。建议：请检查资源预锁定 ID（reservedId）与调用 PreAllocateResource 接口时指定的参数是否一致。 |
| 10017 | ERROR_START_NO_SUFFICIENT_FUND | 开始游戏失败。原因：启动游戏时，后付费账户欠费，导致服务不可用。建议：请通过火山引擎『费用中心』充值付费，充值到账后服务自动开启。 |
| 10018 | ERROR_START_USER_CONFLICT | 开始游戏失败。原因：用户使用多个设备申请游戏服务时，触发了游戏多开限制。建议：需要客户端提示用户不要进行多开操作。 |
| 10026 | ERROR_START_MISMATCH_ACCOUNTID | 开始游戏失败。原因：指定的火山引擎账号校验失败。建议：通过火山引擎官网页面右上角 **用户 > 账号管理 > 主账号信息** 获取正确的账号。 |
| 10027 | ERROR_START_INVALID_LOCAL_TIME | 开始游戏失败。原因：用户手机时间和服务端时间相差超过7天，导致鉴权 Token 过期。建议：需要客户端提示用户把手机时间修正为标准时间。 |

### 暂停

描述：当 veGameEngine 处于播放状态时，调用 pause() 暂停从云端拉流，此时并不改变云端的运行状态。

返回值：暂停成功会收到 onStreamPaused 的接口回调。

```java
// veGameEngine class

void pause()
```

### 恢复

描述：当 veGameEngine 处于暂停状态时，调用 resume() 恢复从云端拉流播放。

返回值：恢复播放成功会收到 onStreamResumed 的接口回调。

```java
// veGameEngine class

void resume()
```

### 停止

描述：停止从云端拉流，并释放本次游戏相关的资源，同时通知云端释放实例资源（异步）。

```java
// veGameEngine class

void stop()
```

### 重启游戏

描述：重启云端当前游戏进程（10秒内只能调用最多3次 ）。

```java
// veGameEngine class

void restart()
```

### 静音开关

描述：游戏音量静音开关，及检查播放音量是否处在静音状态。

```java
// veGameEngine class

void muteAudio(boolean mute) //true: 静音; false: 开音。
boolean isAudioMuted() //当前是否处在静音状态，返回：true: 静音； false:开音。
```

### 调节本地播放和采集音量

描述：调节客户端本地播放和采集音量。

```java
// AudioService class

int getLocalAudioPlaybackVolume() //获取客户端本地播放音量
int setLocalAudioPlaybackVolume(int volume) //设置客户端本地播放音量，范围 [0,100]
int setLocalAudioCaptureVolume(int volume) //设置采集客户端本地音量，范围 [0,100]
```

### 调节云端游戏播放音量

描述：调节云端游戏播放音量。

```java
// AudioService class

int getRemoteAudioPlaybackVolume() //获取云端游戏播放音量
int setRemoteAudioPlaybackVolume(int volume) //设置云端游戏播放音量，范围 [0,100]
```

### 设置音频输出源

描述：设置或切换播放音频的设备。
- 设置音频播放设备时，需要保证已开启音频数据发送，否则设置无效；
- 当使用蓝牙耳机进行音频采集、播放时，需要获取蓝牙权限。

```java
// AudioService class

void setAudioPlaybackDevice(deviceId) //指定音频播放设备（系统喇叭、外接喇叭和耳机、有线外接播放器，蓝牙播放器等）
```

### 开启/关闭音频数据发送

描述：开启或关闭采集麦克风音频数据后发送给云端实例，进而注入到实例的 Android 系统中。注意: 需要开启麦克风使用权限。

```java
// AudioService class

void onRemoteAudioStartRequest() //收到云端实例请求开始发送音频数据事件
void onRemoteAudioStopRequest() //收到云端实例请求停止发送音频数据事件

int startSendAudioStream() //获取麦克风权限后，采集并发送音频数据
int stopSendAudioStream() //关闭音频数据发送，并且不进行音频采集
```

### 增大/减小游戏音量

描述：增大或减小游戏音量。

```java
// veGameEngine class

void volumeUp()
void volumeDown()
```

### 手柄

描述：发送游戏手柄消息。

```java
// veGameEngine class

public @Nullable GamePadService getGamePadService()

public interface GamePadService {

    //扳机模拟量
    void sendGamePadRockerEvent(float leftX, float leftY, float rightX, float rightY, float leftT, float rightT);

    //KEYCODE_BUTTON_L2  KEYCODE_BUTTON_R2
    void sendRtRbLtLb(int keyCode, float value);

    // 其他的按键操作，官方按键定义的KeyEvent
    void sendKeyEvent(int action, int keyCode);

    // 设置监听远端震动的消息
    void setGamePadListener(GamePadListener gamePadListener);

    interface GamePadListener {

        // index 表示手柄是第几个发的，leftValue 手柄左马达震动量，rightValue 手柄右马达震动量
        void onVibrate(int index, int leftVale, int rightValue);

    }
}
```

### 传感器开关

描述：获取传感器使用权限。

```java
// veGameEngine class

void enableAccelSensor(boolean enable) //是否启用加速度传感器
void enableGyroscopeSensor(boolean enable) //是否启用陀螺仪传感器
void enableGravitySensor(boolean enable) //是否启用重力传感器
void enableOrientationSensor(boolean enable) //是否启用方向传感器
void enableMagneticSensor(boolean enable) //是否启用磁力传感器
```

### 震动开关

描述：本地震动传感器开关。开启后，当云端的应用令手机发生震动时，本地手机会同步震动。注意：需要打开系统的震动开关后才能生效。

```java
// veGameEngine class

void enableVibrator(boolean enable)
```

### 定位开关

描述：当云端请求本地位置信息后，如果设置为启用，则获取本地位置信息传输到云端。

```java
// veGameEngine class

void enableLocationService(boolean enable)
```

### 设置键盘开关

描述：设置是否允许用户使用键盘进行信息输入的能力。

```java
public interface LocalInputManager {

    void setKeyboardEnable(boolean enable); //是否允许用户使用键盘进行输入

    void getKeyboardEnable(); // 获取键盘启用状态
}
```

### 屏幕旋转

描述：旋转游戏画面方向。

orientation 在收到 SDK IStreamListener onRotation 回调后，旋转当前的 Activity 方向。同时在 Activity  的 onConfigurationChanged(Configuration newConfiguration) 回调中，调用 VeGameEngine 的 rotate 方法，传入 newConfiguration.orientation 的值。

参考示例：

```java
// veGameEngine class

void rotate(int orientation)
```

```java
private void setRotation(int rotation) {
    switch (rotation) {
        case 0:
        case 180:
            setRequestedOrientation(SCREEN_ORIENTATION_UNSPECIFIED);
            break;
        case 90:
        case 270:
            setRequestedOrientation(SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
            break;
    }
}
```

### 收发消息

描述：与云端实例之间收发消息（需要在收到 `veGameEngine#addCloudCoreManagerListener` 回调之后使用）。
有关 Message Channel SDK 的使用方法，参考 [Message Channel SDK 接入说明](https://www.volcengine.com/docs/6512/75598)。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| sendMessage(String destChannelUid, String payload, boolean needAck) | 发送消息：  <br>destChannelUid：目标用户 ID（云端游戏初始化 veMessageChannelClient 时填入的用户 ID）  <br>payload：发送消息内容（size：60KB）  <br>needAck：是否需要接收消息反馈 |
| setMessageListener(IMessageReceiver listener) | 设置消息接收回调监听 |
| getMid() | 获取消息编号 |
| getPayload() | 获取携带的消息内容 |
| getTimeMillis() | 获取消息发送时间 |
| getSrcChannelUid() | 获取发送方的 ChannelUid |
| getDestChannelUid() | 获取接收方的 ChannelUid |

#### 消息发送超时设置

通过 MessageChannel 发送消息到云端游戏时，可以设置消息发送超时时长。

> 当消息发送后，如果超过指定的时长还未收到对应的接收消息反馈，则会通过 `IMessageReceiver#onSentResult` 返回消息发送失败信息。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| sendMessage(String destChannelUid, String payload, long timeout) | 发送消息：  <br>destChannelUid：目标用户 ID（云端游戏初始化 veMessageChannelClient 时填入的用户 ID）  <br>payload：发送消息内容（size：60KB）  <br>timeout：消息发送超时时长，单位 ms（需要大于0；当小于等于0时，通过 IMessageReceiver#onError 返回错误信息） |
| setMessageListener(IMessageReceiver listener) | 设置消息接收回调监听 |


#### IMessageReceiver

消息接收回调监听。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onReceiveMessage(IChannelMessage message) | 消息接收回调 |
| onSentResult(boolean success, String mid) | 发送消息结果回调 |
| onError(int errorCode, String message) | 错误上报回调 |
| onRemoteOffline(String channelUid) | 云端实例离线回调，消息发送可能失败（channelUid 为云端游戏侧初始化 veMessageChannelClient 时填入的用户 ID） |
| onRemoteOnline(String channelUid) | 云端实例在线回调，建议在发送消息前监听该回调检查通道是否已连接（channelUid 为云端游戏侧初始化 veMessageChannelClient 时填入的用户 ID） |

参考示例：

```java
// veGameEngine class

public @Nullable IMessageChannel getMessageChannel()
```

```java
public interface IMessageChannel {

    IChannelMessage sendMessage(String destChannelUid, String payload, boolean needAck);

    void setMessageListener(IMessageReceiver listener);

    interface IMessageReceiver {

        void onReceiveMessage(IChannelMessage message);

        void onSentResult(boolean success, String mid);

        void onError(int errorCode, String message);

        void onRemoteOffline(String channelUid);

        void onRemoteOnline(String channelUid);
    }

    interface IChannelMessage {

        String getMid();

        String getPayload();

        long getTimeMillis();

        String getSrcChannelUid();

        String getDestChannelUid();
    }
}
```

消息发送超时设置：

```java
public interface IMessageChannel {

    IChannelMessage sendMessage(String destChannelUid, String payload, long timeout);

    void setMessageListener(IMessageReceiver listener);

    interface IMessageReceiver {

        void onReceiveMessage(IChannelMessage message);

        void onSentResult(boolean success, String mid);

        void onError(int errorCode, String message);

        void onRemoteOffline(String channelUid);

        void onRemoteOnline(String channelUid);
    }

    interface IChannelMessage {

        String getMid();

        String getPayload();

        long getTimeMillis();

        String getSrcChannelUid();

        String getDestChannelUid();
    }
}
```

#### 错误码

收发消息可能返回的错误码和相关说明如下：

|  **错误码**  |  **错误信息**  |  **说明**  |
| --- | --- | --- |
| 50000 | ERROR_MESSAGE_GENERAL | 未知错误 |
| 50001 | ERROR_MESSAGE_NOT_CONNECTED | 发送消息错误。原因：客户端 SDK 与火山引擎云游戏 PaaS 服务未连接。建议：请检查网络后，重新调用发送消息接口，进行消息发送。 |
| 50002 | ERROR_MESSAGE_FAILED_TO_PARSE_MSG | 消息通道数据解析失败。原因：无法解析接收到的消息。 |
| 50003 | ERROR_MESSAGE_CHANNEL_UID_ILLEGAL | 指定的消息通道 ID 无效。原因：指定的消息通道 ID 与云端初始化 veMessageChannelClient 时填入的用户 ID 不符。 |
| 50004 | ERROR_MESSAGE_FAILED | 发送消息错误。建议：请检查网络。 |
| 50005 | ERROR_MESSAGE_NOT_CLOUD_RUNTIME  | 消息通道错误。原因：云端 SDK 未运行。建议：请检查网络。 |
| 50006 | ERROR_MESSAGE_CONNECT_VERIFY_FAILED  | 消息通道错误。原因：鉴权失败。建议：请检查参数配置。 |
| 50007 | ERROR_MESSAGE_OVER_SIZED | 通过客户端 SDK 发送的消息错误。原因：消息包体过大（不应超过60kb）。建议：减小发送包体大小，重新发送。 |
| 50009 | ERROR_MESSAGE_TIMEOUT_ILLEGAL | 发送消息错误。原因：指定的消息发送超时时长不符合要求，必须为大于0的值。 |

### 收发数据

描述：与云端游戏之间收发数据（需要通过 `start()` 接口的配置参数 `enableFileChannel`，开启文件传输通道，并在收到 `onPlaySuccess` 播放成功回调之后使用）。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| sendData(byte[] payload) | 发送数据包到云端游戏，无数据发送到达回执：  <br>payload：发送的数据包（支持发送的数据包不大于5MB，如果发送的数据大于5MB，将抛出 IllegalArgumentException 异常） |
| sendData(byte[] payload, long timeout, ISendDataListener listener) | 发送数据包到云端游戏，有数据发送到达回执：  <br>payload：发送的数据包  <br>timeout：接收回执超时时长（单位：毫秒）  <br>listener：用于接收数据发送到达回执 |
| setReceiveDataListener(IReceiveDataListener listener) | 设置接收数据回调监听 |
| setStatusListener(IChannelStatusListener listener) | 设置文件传输通道内部状态回调监听 |
| getStatus() | 获取当前 FileChannelEngine 的内部状态 |


#### ISendDataListener

用于向云端游戏发送数据时，消息到达回执的回调。
|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onDataSend() | 当客户端发送数据到云端游戏成功时回调 |
| onError(int err) | 当客户端发送数据到云端游戏失败时回调 |

#### IReceiveDataListener

用于接收云端游戏发送给客户端数据的回调（接收的数据包不大于5MB）。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onDataReceived(byte[] payload) | 当客户端接收到云端游戏发送的数据时回调 |


#### IChannelStatusListener

用于接收当前 FileChannelEngine 通道内部状态变更的回调。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onStateUpdated(int status) | 当文件传输通道内部状态变更时回调 |

状态说明如下：

|  **状态**  |  **说明**  |
| --- | --- |
| IDLE (0x0001) | 未初始化状态 |
| INITIALIZED (0x0002) | 初始化成功 |
| CONNECTING (0x0004) | 正在连接远程服务端 |
| CONNECTED (0x0008) | 与远程服务端连接中 |
| DISCONNECTED (0x0010) | 与远程服务端断开连接中 |

参考示例：

```java
// veGameEngine class

public @Nullable IFileChannel getFileChannel()
```

```java
public interface IFileChannel {

    /**
     * 发送数据包到云端游戏，无到达回执＜br＞
     *
     * @param payload 发送的数据包
     * @exception IllegalArgumentException 发送的数据包太大，无法发送
     * @exception IllegalStateException 内部状态出错，无法完成发送数据包
     */
    void sendData(byte[] payload);

    /**
     * 发送数据包到云端游戏，有到达回执＜br＞
     *
     * @param payload 发送的数据包
     * @exception IllegalArgumentException 发送的数据包太大，无法发送
     * @exception IllegalStateException 内部状态出错，无法完成发送数据包
     */
    void sendData(byte[] payload, long timeout, ISendDataListener listener);

    /**
     * 数据发送到达回执监听
     */
    interface ISendDataListener {
        /**
         * 发送数据包到云端游戏成功后回调＜br＞
         *
         */
        void onDataSend();

        /**
         * 发送数据包到云端游戏失败后回调＜br＞
         *
         * @param err 发送失败原因错误码
         */
        void onError(int err);
    }

    /**
     * 设置接收数据回调监听＜br＞
     *
     * @param listener 接收数据包的回调监听器
     */
    void setReceiveDataListener(IReceiveDataListener listener);

    /**
     * 数据接收监听
     */
    interface IReceiveDataListener {
        /**
         * 接收云端游戏发送给客户端的数据包＜br＞
         *
         * @param payload 云端游戏发送给客户端的数据包
         */
        void onDataReceived(byte[] payload);
    }

    /**
     * 设置通道内部状态回调监听＜br＞
     *
     * @param listener 接收通道内部状态变更的回调
     */
    void setStatusListener(IChannelStatusListener listener);

    /**
     * 获取当前FileChannelEngine的内部状态＜br＞
     *
     * @return 内部状态
     */
    int getStatus();

    /**
     * 文件传输通道内部状态监听
     */
    interface IChannelStatusListener {
        /**
         * 当状态变更时回调＜br＞
         *
         * IDLE  (0x0001) - 未初始化状态
         * INITIALIZED (0x0002) - 初始化成功
         * CONNECTING (0x0004) - 正在连接远程服务端
         * CONNECTED (0x0008) - 与远程服务端连接中
         * DISCONNECTED (0x0010) - 与远程服务端断开连接中
         *
         * @param status 新状态
         */
        void onStateUpdated(int status);
    }
}
```

### 收发文件

描述：云游戏客户端 SDK 支持通过 `IFileChannelExt` 类实现与云端游戏进行文件传输。需要注意的是 `IFileChannelExt` 是一个接口类，通过 VeGameEngine 的 `getFileChannelExt()` 方法获取对象引用。

> 说明：
> - 收发文件功能需要与 Message Channel SDK V1.0.9 及以上版本配合使用。有关 Message Channel SDK 的使用方法，参考 [Message Channel SDK 接入说明]()。
> - 收发文件功能依赖第三方库 okttp3 实现数据的发送和接收功能，请确保工程中已引入该库：
`implementation 'com.squareup.okhttp3:okhttp:4.9.0'`

参考示例：

```java
// veGameEngine class

public @Nullable IFileChannelExt getFileChannelExt()
```

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| startSendFile(File file, ISendFileListener listener) | 启动客户端向云端游戏发送文件（有关 File 类的详细说明，参考 [Android 官方文档说明](https://developer.android.com/reference/java/io/File)） |
| stopSendFile(File file) | 取消客户端向云端游戏发送文件 |
| stopReceiveFile(File file)  | 取消接收云端游戏向客户端发送的文件 |
| setReceiveFileListener(IReceiveFileListener listener) | 设置接收云端游戏发送文件的回调监听 |
| ISendFileListener | 向云端游戏发送文件的状态回调 |
| IReceiveFileListener | 接收云端游戏发送文件的状态回调 |

#### 发送文件数据

描述：用于客户端发送文件到云端游戏，该接口支持发送本地文件，并且可通过 [ISendFileListener](#isendfilelistener) 监听发送进度以及到达情况。注意：当有多个发送任务时，SDK 内部会串行执行，依次发送。

```java
// IFileChannelExt class

/**
 * 发送本地文件到云端游戏
 *
 * @param file 发送的本地文件
 * @param options 自定义参数，将传输到云端游戏
 * @param listener 用于接收发送进度和到达回执
 * */
void startSendFile(File file, Map<String, String> options, ISendFileListener listener);
```

#### ISendFileListener

描述：用于向云端游戏发送文件时监听发送进度以及到达情况。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onStart(File file, Map<String, String> options) | 发送启动时回调 |
| onProgress(File file, Map<String, String> options, int progress) | 发送进度变化时回调：  <br>progress（0-100之间的整数） |
| onComplete(File file, Map<String, String> options) | 发送完成时回调 |
| onError(File file, Map<String, String> options, int err) | 当发送失败时回调：<br>err（指定错误类型，可参考 [错误码](#错误码)） |
| onCancel(File file, Map<String, String> options) | 发送被取消时回调 |

```java
public interface ISendFileListener {
    /**
     * 发送启动时回调
     *
     * @param file 发送的本地文件
     * @param options 自定义参数，将传输到云端游戏
     */
    void onStart(File file, Map<String, String> options);

    /**
     * 发送进度变更时回调
     *
     * @param file 发送的本地文件
     * @param options 自定义参数
     * @param progress 发送进度，取值范围(0,100)
     */
    void onProgress(File file, Map<String, String> options, int progress);

    /**
     * 发送完成时回调
     *
     * @param file 发送的本地文件
     * @param options 自定义参数，将传输到云端游戏
     */
    void onComplete(File file, Map<String, String> options);

    /**
     * 发送取消时回调
     *
     * @param file 发送的本地文件
     * @param options 自定义参数
     */
    void onCancel(File file, Map<String, String> options);

    /**
     * 当发送失败时回调
     *
     * @param file 发送的本地文件
     * @param options 自定义参数
     * @param err 发送失败错误码
     */
    void onError(File file, Map<String, String> options, int err);
}
```

#### 停止发送文件数据

描述：停止客户端向云端游戏发送文件。

```java
// IFileChannelExt class

/**
 * 停止发送本地文件到云端游戏
 *
 * @param file 要停止发送的本地文件
 *
 */
void stopSendFile(File file);
```

#### 接收文件数据

描述：云端游戏发送文件到客户端，通过 IReceiveFileListener 来接收文件发送状态信息。

```java
// IFileChannelExt class

/**
 * 设置接收文件回调监听
 *
 * @param listener 接收文件的回调监听
 */
void setReceiveFileListener(IReceiveFileListener listener);
```

#### IReceiveFileListener

描述：用于接收云端游戏发送文件到客户端的状态信息回调。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onStart(File file, Map<String, String> options) | 接收文件开始时回调 |
| onProgress(File file, Map<String, String> options, int progress) | 接收文件进度变化时回调  <br>progress（0-100之间的整数） |
| onComplete(File file, Map<String, String> options) | 接收文件完成时回调 |
| onError(File file, Map<String, String> options, int err) | 接收文件失败时回调：  <br>err（指定错误类型，可参考 [错误码](#错误码)） |
| onCancel(File file, Map<String, String> options) | 接收文件被取消时回调 |

```Java
public interface IReceiveFileListener {
    /**
     * 接收启动时回调
     *
     * @param file 云端游戏发送的文件
     * @param options 自定义参数，由云端游戏发送到客户端
     */
    void onStart(File file, Map<String, String> options);

    /**
     * 接收进度回调
     *
     * @param file 云端游戏发送的文件
     * @param options 自定义参数，由云端游戏发送到客户端
     * @param progress 传输进度
     */
    void onProgress(File file, Map<String, String> options, int progress);

    /**
     * 接收完成时回调
     *
     * @param file 云端游戏发送的文件
     * @param options 自定义参数，由云端游戏发送到客户端
     */
    void onComplete(File file, Map<String, String> options);

    /**
     * 接收取消时回调
     *
     * @param file 云端游戏发送的文件
     * @param options 自定义参数，由云端游戏发送到客户端
     */
    void onCancel(File file, Map<String, String> options);

    /**
     * 当接收失败时回调
     *
     * @param file 云端游戏发送的文件
     * @param options 自定义参数，由云端游戏发送到客户端
     * @param err 接收失败错误码
     */
    void onError(File file, Map<String, String> options, int err);
}
```

### 停止接收文件数据

描述：停止接收云端游戏向客户端发送的文件。

```java
// IFileChannelExt class

/**
 * 停止接收云端游戏发送的文件
 *
 * @param file 要停止接收的文件
 *
 */
void stopReceiveFile(File file)
```

### 本地键盘

描述： 获取 LocalInputManager，管理本地键盘与云端游戏的交互。

#### LocalInputManager

```java
// veGameEngine class

public @Nullable LocalInputManager getLocalInputManager()
```

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| setRemoteInputCallBack(RemoteInputCallBack remoteInputCallBack) | 设置监听远端键盘弹出的回调，参考以下 [RemoteInputCallBack](#remoteinputcallback) |
| coverCurrentEditTextMessage(String text) | 设置当前输入框的内容 |
| enableShowCurrentInputText(boolean enable) | 显示自带的 textView，用于显示当前输入框内容，默认不打开 |
| closeAutoKeyBoard(boolean isIntercept) | 拦截是否打开本地键盘，默认不拦截 SDK 调起本地键盘；当传入 true 时，由用户自行处理键盘的弹出和内容的发送 |
| getEditText() | 获取隐藏的输入框对象 |
| getKeyboardEnable() | 远端输入法是否开启 |
| setKeyBoardEnable(boolean enable) | 设置远端输入法是否开启 |

参考示例：

```java
public interface LocalInputManager {

    /**
     * 设置监听远端键盘弹出的回调
     * @param remoteInputCallBack
     */
    void setRemoteInputCallBack(RemoteInputCallBack remoteInputCallBack);

    /**
     * 设置当前输入框的内容
     * @param text 内容字符串
     */
    void coverCurrentEditTextMessage(String text);

    /**
     * 显示自带的输入view ，默认不打开
     * @param enable 是否打开
     */
    void enableShowCurrentInputText(boolean enable);

    /**
     * 拦截是否打开本地键盘；默认false 不拦截sdk调起本地键盘；传入true 用户自行处理键盘的弹出和内容的发送
     * @param isIntercept  是否拦截本地键盘弹出。
     */
    void closeAutoKeyBoard(boolean isIntercept);

    /**
     * 获取本地隐藏的输入框
     * @return 本地的edittext
     */
    EditText getEditText();

    /**
     * 获取远端输入法是否开启
     */
    boolean getKeyboardEnable();

    /**
     * 设置远端输入法是否开启
     */
    int setKeyBoardEnable(boolean enable);
}
```

#### RemoteInputCallBack

远端键盘弹出的回调。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onPrepare(String hintText, int inputType) | 远端当前输入框的一些状态回调 |
| onCommandShow() | 请求弹出软键盘的回调，会多次回调 |
| onCommandHide() | 请求收起软键盘的回调 |
| onTextChange(String text) | 远端输入框内容的更改回调 |
| onRemoteKeyBoardEnabled(boolean enable) | 远端输入法状态更新的回调 |

参考示例：

```java
interface RemoteInputCallBack {

        /**
         * @param hintText 提示文本
         * @param inputType 输入格式
         */
        void onPrepare(String hintText , int inputType);

        /**
         * 远端键盘请求弹出
         */
        void onCommandShow();

        /**
         * 远端键盘请求收起
         */
        void onCommandHide();

        /**
         * 监听本地输入内容的改变 ，可以自定义显示给用户
         * @param text 改变的内容
         */
        void onTextChange(String text);

        /**
         *  远端输入法状态更新
         */
        void onRemoteKeyBoardEnabled(boolean enable);
    }
```

### 清晰度切换

描述：在游戏运行过程中，可通过指定清晰度 ID，切换游戏的清晰度。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| switchVideoStreamProfile(int streamProfileId) | 切换游戏的清晰度（streamProfileId 的可选值和清晰度配置说明，参考以下 [清晰度档位说明](#清晰度档位说明)） |
| setStreamProfileChangeListener(StreamProfileChangeCallBack streamProfileChangeCallBack) | 设置清晰度切换成功的回调 |

#### StreamProfileChangeCallBack

清晰度切换成功的回调

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onVideoStreamProfileChange(boolean isSuccess, int from, int current) | 清晰切换结果：  <br>isSuccess（清晰度是否成功切换）  <br>from（切换前的清晰度）  <br>current（当前的清晰度） |

#### 清晰度档位说明

|  **档位 ID**  |  **宽**  |  **高**  |  **码率（kbps）**  |  **帧率**  |
| --- | --- | --- | --- | --- |
| 1 | 720 | 1280 | 4000 | 30 |
| 2（流畅） | 720 | 1280 | 4000 | 60 |
| 3 | 720 | 1280 | 5000 | 30 |
| 4 | 720 | 1280 | 5000 | 60 |
| 5 | 720 | 1280 | 6000 | 30 |
| 6（标清，默认档位） | 720 | 1280 | 6000 | 60 |
| 7 | 720 | 1280 | 8000 | 30 |
| 8 | 720 | 1280 | 8000 | 60 |
| 9 | 1080 | 1920 | 4000 | 30 |
| 10 | 1080 | 1920 | 4000 | 60 |
| 11 | 1080 | 1920 | 5000 | 30 |
| 12 | 1080 | 1920 | 5000 | 60 |
| 13 | 1080 | 1920 | 6000 | 30 |
| 14（高清） | 1080 | 1920 | 6000 | 60 |
| 15 | 1080 | 1920 | 8000 | 30 |
| 16（超清） | 1080 | 1920 | 8000 | 60 |
| 17 | 540 | 960 | 4000 | 30 |
| 18 | 450 | 800 | 4000 | 30 |
| 19 | 360 | 640 | 3000 | 30 |
| 20 | 1080 | 1920 | 10000 | 30 |
| 21 | 1080 | 1920 | 10000 | 60 |
| 22 | 1440 | 2560 | 12000 | 30 |
| 23 | 1440 | 2560 | 12000 | 60 |

参考示例：

```java
public interface StreamProfileManager {

    void switchVideoStreamProfile(int streamProfileId);

    void setStreamProfileChangeListener(StreamProfileChangeCallBack streamProfileChangeCallBack);
}
```

```java
public interface StreamProfileChangeCallBack {

    void onVideoStreamProfileChange(boolean isSuccess, int from, int current);
}
```

### 客户端前后台切换

描述：设置客户端应用或游戏切换前后台的状态。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| switchBackground(boolean on) | 客户端切换前后台：  <br>true：切后台  <br>false：切前台 |
| setBackgroundSwitchListener(BackgroundSwitchListener listener) | 设置监听客户端应用切换前后台的回调方法：  <br>BackgroundSwitchListener：监听客户端应用切换前后台 |

参考示例：

```java
// veGameEngine class

public @Nullable PodControlService getPodControlService()

public interface PodControlService {

    void switchBackground(boolean on);

    void setBackgroundSwitchListener(BackgroundSwitchListener listener);
}
```

#### BackgroundSwitchListener

监听客户端应用切换前台或后台。

> 如果重复调用切换前后台接口，IGamePlayerListener 的 onWarning 会给出告警提示。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onBackgroundSwitched() | 客户端应用切换前台或后台的回调（用户手动调用 `switchBackground` 接口或者触摸屏幕发生切换前后台都会触发这个回调）：  <br>true（切换到后台）  <br>from（切换到前台） |

参考示例：

```java
public interface BackgroundSwitchListener {

    void onBackgroundSwitched(boolean on);
}
```

### 云端游戏切换前台

描述：将云端游戏切换到前台。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| setRemoteGameForeground() | 将云端游戏切换到前台 |

参考示例：

```java
interface VeGameManager {

    void setRemoteGameForeground();

    void setGroundChangeListener(GameGroundSwitchedListener gameGroundSwitchedListener);
}
```

#### GameGroundSwitchedListener

云端游戏切换前台或后台状态。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onRemoteGameSwitchedBackground(int switchedType) | 云端游戏切换到后台，switchedType：  <br>0（手动）  <br>1（自动） |
| onRemoteGameSwitchedForeground(int switchedType) | 云端游戏切换到前台，switchedType：  <br>0（手动）  <br>1（自动） |
| onRemoteGameSwitchedFailed(int errorCode, String errorMsg) | 云端游戏切换前后台失败 |

参考示例：

```java
public interface GameGroundSwitchedListener {

    void onRemoteGameSwitchedBackground(int switchedType);

    void onRemoteGameSwitchedForeground(int switchedType);

    void onRemoteGameSwitchedFailed(int errorCode, String errorMsg);
}
```

### 设置保活时间

描述：在游戏中动态设置客户端切后台之后，云端游戏服务的保活时间（如不设置，则使用默认保活时长300秒；支持设置的上限值为7200，即2小时；如需设置更长时间，可联系火山引擎云游戏技术支持）。

> 当客户端切后台时开始计时。如用户在设置的保活时间到达时未切回前台，服务端将断开游戏连接。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| setIdleTime() | 指定保活时长，单位秒 |

参考示例：

```java
// veGameEngine class

public @Nullable PodControlService getPodControlService()

public interface PodControlService {
    /**
     * 设置保活时间
     *
     * @param time 单位秒
     * @return -1 表示未在播放状态下使用
     */
    int setIdleTime(long time);
}
```

### 设置/获取无操作回收服务时间

描述：在游戏中动态设置无操作回收服务时长。

> 如果已在 `start()` 接口中通过 `autoRecycleTime` 参数设置，动态设置的时长优先级高。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| setAutoRecycleTime(int time, SetAutoRecycleTimeCallback callback) | 设置无操作回收服务时间：  <br>time（无操作回收服务时长，单位秒；支持设置的范围为 [0, 7200]，即0到2小时；如不设置或设置为0，则使用默认时长300秒）  <br>callback（设置无操作回收服务时长的回调） |
| getAutoRecycleTime(GetAutoRecycleTimeCallback callback) | 获取已设置的无操作回收服务时间：  <br>callback（获取无操作回收服务时长的回调） |

参考示例：

```java
// veGameEngine class

public @Nullable PodControlService getPodControlService()

public interface PodControlService {

    /**
     * @param time 无操作回收时长
     * @param callback 设置无操作回收服务时长的回调
     * @return 0：正常返回；-1：ICoreEngine为空；-2：time参数小于0
     */
    int setAutoRecycleTime(int time, SetAutoRecycleTimeCallback callback);

    /**
      * @param callback 获取已设置无操作回收服务时长的回调
      * @return 0：正常返回；-1：ICoreEngine为空
      */
     int getAutoRecycleTime(GetAutoRecycleTimeCallback callback);
}
```

### 重置签名密钥

描述：在用户鉴权密钥过期之前（生成 token 时指定的密钥过期时间），刷新和重置签名密钥（通过调用服务端  STS Token 接口获取）。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| resetToken() | 配置参数如下：  <br>ak：用于用户鉴权的临时 Access Key  <br>sk：用于用户鉴权的临时 Secret Key  <br>token：用于用户鉴权的临时 Token  <br>以上参数可通过调用服务端 [STSToken 接口](https://www.volcengine.com/docs/6512/75588) 获取 |

参考示例：

```java
// veGameEngine class

/**
 * 重新设置 ak，sk，token
 */
public void resetToken(@NonNull String ak, @NonNull String sk, @NonNull String token)
```

### 发送剪贴板消息

描述：获取剪贴板管理类、注册云端实例剪贴板回调、发送本地剪贴板消息到云端（需要在收到 onPlaySuccess 播放成功回调之后使用）。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| sendClipBoardMessage() | 发送本地剪贴板数据到云端，支持发送文本数据 |
| onClipBoardMessageReceived() | 收到云端返回的剪贴板数据回调 |

参考示例：

```java
// veGameEngine class

public @Nullable IClipBoardServiceManager getClipBoardServiceManager()
```

```java
/**
 * @desc 剪贴板 操作 发送剪贴板数据
 */
public interface IClipBoardServiceManager {

    /**
     * 云端实例同步过来的剪贴板数据
     * @param iClipBoardListener
     */
    void setBoardSyncClipListener(IClipBoardListener iClipBoardListener);

    /**
     * 手动发送剪贴板数据
     *
     * @param data 剪贴板数据
     */
    void sendClipBoardMessage(ClipData data);
}
```

> 注意：clipData mimeType 只支持：ClipDescription.MIMETYPE_TEXT_PLAIN 和 ClipDescription.MIMETYPE_TEXT_HTML

```java
/**
 * @desc 回调用户云端剪贴板数据的 listener
 */
public interface IClipBoardListener {
    /**
     * @param clipData 云端返回的剪贴板数据
     */
    void onClipBoardMessageReceived(ClipData clipData);
}
```

### Camera 注入

描述：获取 CameraManager 对象，该对象支持发送本地视频流和监听云端请求本地摄像头的打开和关闭。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| getCameraManager() | 获取 CameraManager 对象 |

```java
// veGameEngine class

public @Nullable CameraManager getCameraManager()
```

```java
public interface CameraManager {

    int switchCamera(CameraId cameraId);

    int startVideoStream(CameraId cameraId);

    void stopVideoStream();

    void setRemoteRequestListener(RemoteCameraRequestListener remoteRequestListener);

    void setCameraManagerListener(CameraManagerListener cameraManagerListener);

    void setLocalVideoCanvas(SurfaceView surfaceView, RenderMode mode);

    void setLocalVideoMirrorMode(MirrorMode mode);

    void setVideoEncoderConfig(List<VideoStreamDescription> videoStreamDescriptions);
}
```

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| switchCamera(CameraId cameraId) | 切换前后摄像头，用于中途切换（预留，目前仅支持前置摄像头）  <br>**参数**  <br>CameraId：FRONT（前置摄像头）；  <br>BACK（后置摄像头）  <br>**返回值**  <br>0：调用成功  <br>-1：调用失败（可能原因为尚未收到 `onPlaySuccess` 播放成功回调） |
| startVideoStream(CameraId cameraId) | 开始指定摄像头采集并推流  <br>**参数**  <br>CameraId：FRONT（前置摄像头）；  <br>BACK（后置摄像头）  <br>**返回值**  <br>0：调用成功  <br>-1：调用失败（可能原因为尚未收到 `onPlaySuccess` 播放成功回调）  <br>**调用时机**  <br>在收到 `RemoteCameraRequestListener` 的方法 `onVideoStreamStartRequested` 中去调用  <br>**调用结果**  <br>CameraManagerListener 回调接口的 `onLocalVideoStateChanged` 方法中会回调调用结果 |
| stopVideoStream() | 停止推流  <br>**调用时机**  <br>在收到 `RemoteCameraRequestListener` 的方法 `onVideoStreamStopRequested` 中去调用 |
| setRemoteRequestListener() | 设置监听云端请求打开本地摄像头的回调方法  <br>**参数**  <br>RemoteCameraRequestListener：云端请求打开（关闭）摄像头 |
| setCameraManagerListener() | 设置监听当前推流状态和首帧采集  <br>**参数**  <br>CameraManagerListener：监听调用 startVideoStream 的推流结果 |
| setLocalVideoCanvas(SurfaceView surfaceView, RenderMode mode) | 设置本地视频画面布局  <br>**参数**  <br>1：RENDER_MODE_HIDDEN（默认，视窗填满优先；缩放完成后，视频帧的一边长和视窗的对应边长一致，另一边长大于等于视窗对应边长）  <br>2：RENDER_MODE_FIT（视频帧内容全部显示优先；缩放完成后，视频帧的一边长和视窗的对应边长一致，另一边长小于等于视窗对应边长）  <br>3：RENDER_MODE_Fill（视频帧自适应画布；视频帧非等比缩放，直至画布被填满。在此过程中，视频帧的长宽比例可能会发生变化） |
| setLocalVideoMirrorMode(MirrorMode mode) | 使用“前置摄像头”采集时，是否开启镜像翻转本地摄像头画面  <br>**参数**  <br>0：MIRROR_MODE_OFF（默认，不开启镜像翻转）  <br>1：MIRROR_MODE_ON（开启镜像翻转） |
| setVideoEncoderConfig(List<VideoStreamDescription> videoStreamDescriptions) | 根据客户端的网络情况，配置本地最大视频编码质量参数  <br>**参数**  <br>width（宽度）  <br>height（高度）  <br>frameRate（帧率）  <br>maxBitrate（最大码率） |

#### RemoteCameraRequestListener

监听云端请求本地摄像头的打开和关闭。

```java
public interface RemoteCameraRequestListener {

    /**
     * 云端请求本地摄像头
     * @param CameraId：FRONT（前置摄像头）；BACK（后置摄像头）
     *
     */
    void onVideoStreamStartRequested(CameraId cameraId);

    /**
     * 云端关闭本地摄像头回调
     */
    void onVideoStreamStopRequested();
}
```

#### CameraManagerListener

监听本地推流状态。

```java
public interface CameraManagerListener {

    /**
     *
     * @param localVideoStreamState 枚举值：当前推流状态
     * @param errorCode 错误码
     */
    void onLocalVideoStateChanged(LocalVideoStreamState localVideoStreamState, LocalVideoStreamError errorCode);

    /**
     * 第一帧被采集
     */
    void onFirstCapture();
}
```

#### LocalVideoStreamState

```java
public enum LocalVideoStreamState {
    LOCAL_VIDEO_STREAM_STATE_STOPPED(0),
    LOCAL_VIDEO_STREAM_STATE_RECORDING(1),
    LOCAL_VIDEO_STREAM_STATE_ENCODING(2),
    LOCAL_VIDEO_STREAM_STATE_FAILED(3);
 }
 ```   

推流状态说明：

|  **状态码**  |  **说明**  |
| --- | --- |
| LOCAL_VIDEO_STREAM_STATE_STOPPED | 本地视频采集停止状态（默认初始状态） |
| LOCAL_VIDEO_STREAM_STATE_RECORDING | 本地视频采集设备启动成功 |
| LOCAL_VIDEO_STREAM_STATE_ENCODING | 本地视频采集后，首帧编码成功 |
| LOCAL_VIDEO_STREAM_STATE_FAILED | 本地视频启动失败 |

#### LocalVideoStreamError

```java
public enum LocalVideoStreamError {
    LOCAL_VIDEO_STREAM_ERROR_OK(0),
    LOCAL_VIDEO_STREAM_ERROR_FAILURE(1),
    LOCAL_VIDEO_STREAM_ERROR_DEVICE_NO_PERMISSION(2),
    LOCAL_VIDEO_STREAM_ERROR_DEVICE_BUSY(3),
    LOCAL_VIDEO_STREAM_ERROR_DEVICE_NOT_FOUND(4),
    LOCAL_VIDEO_STREAM_ERROR_CAPTURE_FAILURE(5),
    LOCAL_VIDEO_STREAM_ERROR_ENCODE_FAILURE(6);
}
```    

错误码说明：

|  **错误码**  |  **说明**  |
| --- | --- |
| LOCAL_VIDEO_STREAM_ERROR_OK | 状态正常 |
| LOCAL_VIDEO_STREAM_ERROR_FAILURE | 本地视频流发布失败 |
| LOCAL_VIDEO_STREAM_ERROR_DEVICE_NO_PERMISSION | 没有权限启动本地视频采集设备 |
| LOCAL_VIDEO_STREAM_ERROR_DEVICE_BUSY | 本地视频采集设备被占用 |
| LOCAL_VIDEO_STREAM_ERROR_DEVICE_NOT_FOUND | 本地视频采集设备不存在或已移除 |
| LOCAL_VIDEO_STREAM_ERROR_CAPTURE_FAILURE | 本地视频采集失败，建议检查采集设备是否正常工作 |
| LOCAL_VIDEO_STREAM_ERROR_ENCODE_FAILURE | 本地视频编码失败 |

参考代码

```java
// 在收到 IGamePlayerListener 的 onPlaySuccess 回调后，获取 CameraManager，监听远端请求，同时调用对应的方法

@Override
public void onPlaySuccess(String roundId, int videoStreamProfile, Map<String, String> extraMap, String gameId, String reservedId) {

    VeGameEngine.getInstance().getCameraManager().setRemoteRequestListener(new RemoteCameraRequestListener() {
        @Override
        public void onVideoStreamStartRequested(CameraId cameraId) {
            AcLog.d(TAG, "onVideoStreamStartRequested, cameraId :" + cameraId);
            // 用户弹窗 提示用户将打开摄像头或者照相机权限请求，有权限可以直接调用下面这行
            VeGameEngine.getInstance().getCameraManager().startVideoStream(cameraId);
        }

        @Override
        public void onVideoStreamStopRequested() {
            AcLog.d(TAG, "onVideoStreamStopRequested ");
            // 关闭推流
            VeGameEngine.getInstance().getCameraManager().stopVideoStream();
        }
    });
    VeGameEngine.getInstance().getCameraManager().setCameraManagerListener(new CameraManagerListener() {
        @Override
        public void onLocalVideoStateChanged(LocalVideoStreamState localVideoStreamState, LocalVideoStreamError errorCode) {
            // 打印当前推流状态，用于排查问题
            AcLog.d(TAG, "LocalVideoStreamState" + localVideoStreamState.toString() + ",LocalVideoStreamError" + errorCode);
        }

        @Override
        public void onFirstCapture() {
            AcLog.d(TAG, "onFirstCapture");
        }
    });
}
```

### 保存/还原用户配置信息

描述：设置保存游戏云端配置文件的路径，当用户正常退出或异常退出（切前后台保活到期、踢下线、游戏崩溃等），按照配置的路径保存用户的配置文件。当用户再次进入游戏时，拉取和还原已保存的用户配置信息。

> 需要已通过 `start()` 接口的配置参数 `userProfilePath` 设置保存配置文件的路径后，并收到 `onPlaySuccess` 播放成功回调之后使用。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| setUserProfilePath(String[] userProfilePath) | 设置游戏云端配置文件绝对路径：  <br>userProfilePath：保存配置文件的路径列表 |
| getUserProfilePath(GetUserProfilePathListener userProfilePathListener) | 获取保存配置文件的路径 |

参考示例：

```java
// veGameEngine class

public @Nullable PodControlService getPodControlService()

public interface PodControlService {

   int setUserProfilePath(List<String> pathList);

   int getUserProfilePath(GetUserProfilePathListener userProfilePathListener);
}
```

### 游戏控制权转移

描述：获取多用户管理类 MultiUserService，支持开启多人游戏及游戏控制权转移等功能。

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| getMultiUserService() | 获取多用户管理服务，注意需要在接口 `ICloudCoreManagerStatusListener` 的 `onInitalized` 函数回调后获取，可能为空（插件包） |

#### MultiUserService

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| changeRole(String userId, Role role, ChangeRoleCallBack callBack) | 修改某个用户的角色：  <br>userid：游戏玩家用户 ID  <br>role：角色（Role.PLAYER：操作者；Role.VIEWER：观看者）  <br>callBack：游戏玩家角色修改结果回调 |
| setRoomListener(RoomListener roomListener) | 设置当前房间的监听器（需要在首帧前设置） |
| getCurrentRole() | 获取当前游戏玩家的角色 |

#### RoomListener 回调

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onPlayerChanged(String userId) | 当用户角色发生变化后，收到远端服务的通知 |
| onJoinRoomRoleResult(Role role, int reason, String PlayerUid) | 用户加入房间成功后，收到的当前用户角色的回调：  <br>role：用户角色  <br>reason（0：成功，请求的一致；其他值为失败的原因)  <br>playerUid：当前的操作者 ID |

#### ChangeRoleCallBack 回调

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onResult(String userId, Role role, int result) | 游戏玩家角色修改结果回调：  <br>userId：游戏玩家用户 ID  <br>role：游戏玩家角色  <br>result（0：角色修改成功；1：房间类型不正确，不支持切换控制权或不支持多玩家；2：希望改变的用户不存在） |

参考示例：

```java
// veGameEngine class

public interface MultiUserService {

    // 改变某个userId的角色
    void changeRole(String userId, Role role, ChangeRoleCallBack callBack);

    // 设置当前的RoomListener
    void setRoomListener(RoomListener roomListener);

    // 获取当前用户的角色
    Role getCurrentRole();

    interface RoomListener {

        // 回调当前的广播Player
        void onPlayerChanged(String userId);

        // join Room首帧到达后获取当前的用户角色, reason (0 成功和请求的一致， 其他值标识失败的原因)
        void onJoinRoomRoleResult(Role role, int reason, String PlayerUid);

    }

    interface ChangeRoleCallBack {
        // 游戏玩家角色修改结果
        void onResult(String userId, Role role, int result);

    }
}
```

### 设置 Debug 模式

描述：设置调试模式，在该模式下会打印日志信息。

```java
// veGameEngine class

static void setDebug(boolean debug)
```

### 获取设备 ID

描述：获取 SDK 生成的设备 ID，用于后台查询 SDK 日志，进行调试和问题排查（建议接入方在需要问题排查时，提供给火山引擎云游戏服务技术支持）。

参考以下示例：

```java
// veGameEngine class

public String getDeviceId()
```

### 设置是否生成本地日志

描述: 打印的日志是否生成本地文件，调试阶段使用。设置为 true 就会打印日志到本地，默认路径为外部存储路径下 `file/veSdkLog/`。

```java
// veGameEngine class

public static void setCreateLocalLog(boolean isCreate)
```

### 设置 Logger

描述：设置自定义 Logger。

```java
// veGameEngine class

static void setLogger(ILogger logger)
```

#### ILogger

```java
public interface ILogger {
    /**
     * verbose日志
     *
     * @param tag
     * @param msg
     */
    void onVerbose(String tag, String msg);

    /**
     * debug日志
     *
     * @param tag
     * @param msg
     */
    void onDebug(String tag, String msg);

    /**
     * info日志
     *
     * @param tag
     * @param msg
     */
    void onInfo(String tag, String msg);

    /**
     * warn日志
     *
     * @param tag
     * @param msg
     */
    void onWarn(String tag, String msg);

    /**
     * error日志
     *
     * @param tag
     * @param msg
     */
    void onError(String tag, String msg);

    /**
     * error日志
     *
     * @param tag
     * @param msg
     * @param e
     */
    void onError(String tag, String msg, Throwable e);
}    
```    

### 获取 SDK 版本信息

```java
// veGameEngine class
static String getSDKVersion()
```

## 警告码

警告码回调接口为 `IGamePlayerListener#onWarning`，具体错误原因见回调接口中的 `message`。

|  **警告码**  |  **警告信息**  | **说明**  |
| --- | --- | --- |
| 10010 | WARNING_START_NO_STOP_BEFORE | 开始游戏失败。原因：同一个客户端连续调用了两次 `start()` 接口启动游戏，之前未调用 `stop()` 接口停止游戏。建议：请调用服务端 [GameStop](https://www.volcengine.com/docs/6512/102175) 接口结束上一次 “游戏”，再调用 `start()` 接口开始游戏。 |
| 10019 | WARNING_START_INVALID_AUTO_RECYCLE_TIME | 设置无操作回收服务时长出错。建议：请参考 [autoRecycleTime]() 参数描述进行设置。 |
| 10023 | WARNING_START_WITH_FRAMEWORK_NOT_FOUND | 指定的伴随程序不存在。原因：传入的伴随程序包名错误。建议：检查是否已上传指定的伴随程序、且伴随程序包名称正确。 |
| 10024 | WARNING_START_WITH_FRAMEWORK_PART_MATCH | 指定的部分伴随程序不存在。原因：传入的伴随程序包名部分错误。建议：检查是否已上传指定的伴随程序、且伴随程序包名称是否正确。 |
| 10025 | WARNING_START_WITH_FRAMEWORK_WRONG_INPUT_FORMAT | 指定的伴随程序包名格式错误。建议：检查启动游戏接口的 `extra` 参数格式是否正确。 |
| 30007 | WARNING_SDK_LACK_OF_LOCATION_PERMISSION | SDK 无定位服务权限。原因：定位服务权限未开启。建议：检查设备定位服务权限是否开启。 |
| 40036 | WARNING_USER_PROFILE_RESTORE_FAILURE | 用户存档信息还原失败。建议：请联系火山引擎云游戏服务技术支持。 |
| 40037 | WARNING_LOCAL_ALREADY_SET_BACKGROUND | 本地客户端应用已切换到后台，请勿重复调用切换后台接口。 |
| 40038 | WARNING_LOCAL_ALREADY_SET_FOREGROUND | 本地客户端应用已切换到前台，请勿重复调用切换前台接口。 |
| 40040 | WARNING_REMOTE_<package name>_FOREGROUND_FAILURE_NO_SUCH_PACKAGE_NAME | 云端游戏切换前台失败。原因：游戏包名不存在。建议：请检查指定的游戏包名是否正确。 |
| 40041 | WARNING_REMOTE_<package name>_ALREADY_FOREGROUND | 云端游戏切换前台失败。原因：云端游戏已处于前台。 |
| 40043 | WARNING_REMOTE_<package name>_FOREGROUND_FAILURE_NO_PACKAGE_NAME_BACKGROUND | 云端游戏切换前台失败。原因：指定的游戏不在后台或已被关闭。 |
| 40044 | WARNING_GAME_STOPPED_INGAME_EXIT | 游戏停止。原因：一般是因为云端运行的游戏主动退出了。建议：客户端需要提示用户目前游戏主动被结束了，如果需要，可以重新开始游戏。 |

## 错误码

错误码回调接口为 `IGamePlayerListener#onError`，具体错误原因见回调接口中的 `message`。

|  **错误码**  |  **错误信息**  | **说明**  |
| --- | --- | --- |
| 20000 | ERROR_STREAM_GENERAL | 游戏串流连接错误。原因：网络原因。建议：请检查网络。 |
| 20002 | ERROR_STREAM_CHANGE_CLARITY_ID_NOT_IN_START_STATE | 切换清晰度失败。原因：当前处于非播放状态 。建议：请调用 `start()` 接口，在收到播放成功回调后，再调用切换清晰度档位接口。 |
| 20003 | ERROR_STREAM_CLARITY_ID_NOT_IN_RANGE | 切换清晰度失败。原因：切换游戏视频清晰度档位不在预设范围内。 |
| 30000 | ERROR_SDK_GENERAL | 客户端 SDK 通用错误。 |
| 30001 | ERROR_SDK_INIT_FAILED | 客户端 SDK 初始化失败。 |
| 30002 | ERROR_SDK_<Parameter>_PARAMETER_EMPTY | 客户端调用 SDK 接口错误。原因：接口参数为空。建议：请参考接口说明文档，检查接口参数。 |
| 30003 | ERROR_SDK_<Parameter>_PARAMETER_ERROR | 客户端调用 SDK 接口错误。原因：接口参数错误。建议：请参考接口说明文档，检查接口参数。 |
| 30009 | ERROR_INIT_ACCOUNT_ID_ILLEGAL | 未指定火山引擎账号。建议：通过火山引擎官网页面右上角 **用户 > 账号管理 > 主账号信息** 获取正确的账号。 |
| 40011 | MESSAGE_START_RESERVED_ID_EXPIRED | 资源预锁定 ID 已过期。建议：调用服务端 [PreAllocateResource](https://www.volcengine.com/docs/6512/107714) 接口重新获取。 |
| 40012 | MESSAGE_RESERVED_ID_NOT_APPLIED | 未通过指定的资源预锁定 ID 启动游戏。建议：确保启动游戏时指定的资源预锁定 ID 正确。 |
| 40013 | MESSAGE_START_RESERVED_ID_RELEASED | 与指定的资源预锁定 ID 相对应的实例资源已被退订。建议：确保申请的实例资源可用。 |
| 40014 | MESSAGE_RESOURCE_OFFLINE | 实例已离线。建议：检查实例资源的运行状态。 |
| 40015 | MESSAGE_RESOURCE_UPDATING | 实例升级中。建议：检查实例资源的运行状态。 |
| 40016 | MESSAGE_RESOURCE_REBOOTING | 实例重启中。建议：检查实例资源的运行状态。 |
| 40017 | MESSAGE_RESOURCE_MAINTANENCE | 实例运维中。建议：检查实例资源的运行状态。 |
| 40018 | MESSAGE_RESOURCE_SERVICE_UPDATING | 实例的云服务升级中。建议：检查实例资源的运行状态。 |
| 40019 | MESSAGE_RESOURCE_OCCUPIED | 实例被占用。建议：检查实例资源的运行状态。 |
| 40020 | MESSAGE_RESOURCE_LAUNCHING_GAME | 实例正在启动游戏。 |
| 40021 | MESSAGE_RESOURCE_IS_RUNNING | 实例正在运行。 |
| 40022 | MESSAGE_RESOURCE_IS_PUBLISH | 实例正在推流。 |
| 40023 | MESSAGE_RESOURCE_RELEASED_NORMAL | 实例正常释放。 |
| 40024 | MESSAGE_RESOURCE_RELEASED_NO_USER | 实例异常释放：客户端超时未加入。 |
| 40025 | MESSAGE_RESOURCE_RELEASED_IDLE | 实例异常释放：客户端无操作释放。 |
| 40026 | MESSAGE_RESOURCE_RELEASED_OS_MISSED | 实例异常释放：游戏镜像缺失。 |
| 40027 | MESSAGE_RESOURCE_RELEASED_GAME_START_FAILURE | 实例异常释放：游戏启动失败。 |
| 40028 | MESSAGE_RESOURCE_RELEASED_STREAMING_ERROR | 实例异常释放：RTC 推流成功，但是推流过程中出现异常。建议：请联系火山引擎云游戏服务技术支持。 |
| 40029 | MESSAGE_RESOURCE_RELEASED_3RD_APP_MISSED | 实例异常释放：伴随包镜像缺失。建议：请联系火山引擎云游戏服务技术支持。 |
| 40030 | MESSAGE_RESOURCE_ERROR | 实例故障。建议：检查实例资源的运行状态或联系火山引擎云游戏服务技术支持。 |
| 40031 | MESSAGE_3RD_APP_START_FAILURE | 伴随包启动失败。建议：请联系火山引擎云游戏服务技术支持。 |
| 40032 | MESSAGE_CLOUD_GAME_CRASH_OFTEN | 游戏频繁崩溃。建议：请尝试复现游戏在本地设备是否运行异常，如本地未复现，可以联系火山引擎云游戏服务技术支持。 |
| 40033 | MESSAGE_GAME_STEAMING_FAILURE | RTC 推流不成功。建议：请联系火山引擎云游戏服务技术支持。 |
| 40035 | ERROR_GAME_STOPPED_USER_PROFILE_PATH_UPLOAD_FAILURE | 游戏停止。原因：用户存档信息上传失败。建议：请联系火山引擎云游戏服务技术支持。 |
| 40039 | ERROR_SET_INVALID_AUTO_RECYCLE_TIME | 设置无操作回收服务时长错误，请查看 [设置无操作回收服务时间](#设置获取无操作回收服务时间) 接口文档进行设置。 |
| 40047 | MESSAGE_RESOURCE_RELEASED_INVALID_PARAMETER | 云端实例接收到的参数非法。 |
| 40048 | MESSAGE_RESOURCE_RELEASED_HEART_BEAT_TIMEOUT | 云端实例离线60秒，中间没有任何心跳。 |
| 40049 | MESSAGE_RESOURCE_RELEASED_INGAME_EXIT | 游戏停止。原因：一般是因为云端运行的游戏主动退出了。建议：客户端需要提示用户目前游戏主动被结束了，如果需要，可以重新开始游戏。 |
| 60001 | ERROR_NET_REQUEST_ERROR | 网络请求失败 |
| 60002 | ERROR_HTTP_REQUEST_ERROR | 网络请求失败 |
| 71001 | ERROR_FILE_CHANNEL_FILE_NOT_FOUND | 文件传输失败。原因：上传或下载的文件不存在。建议：请检查文件名是否正确且确保文件存在后重试。 |
| 71002 | ERROR_FILE_CHANNEL_FILE_NO_ACCESS | 文件传输失败。原因：上传或下载的文件无权限访问。建议：请检查文件的访问权限。 |
| 71003 | ERROR_FILE_CHANNEL_NETWORK_ABORT | 文件传输失败。原因：上传或下载文件过程中网络异常中断。建议：请检查网络连接后重试。 |
| 71004 | ERROR_FILE_CHANNEL_TIMEOUT | 文件传输失败。原因：上传或下载文件过程中超时。建议：请检查网络连接后重试。 |
| 71005 | ERROR_FILE_CHANNEL_STORAGE_NOT_ENOUGH | 文件传输失败。原因：上传或下载文件磁盘空间不足。建议：请确保有足够的存储空间保存文件。 |
| 71006 | ERROR_FILE_CHANNEL_INTERNAL_ERROR | 文件传输失败。原因：上传或下载文件发生内部错误 |
