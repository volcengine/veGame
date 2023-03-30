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
> - 第一次初始化时，SDK 会进行一次资源异步加载。如果在初始化时资源加载失败，不需要重新调用 init。SDK 会在调用 start 接口启动游戏时检查资源是否已成功加载，如果没有，则会重新加载。资源加载完成后会由 ICloudCoreManagerStatusListener 的 onInitialed() 回调。
> - 初始化只需要执行一次。特别注意：多进程会多次执行 Application 的 onCreate，需要在 onCreate 中添加当前进程判断，详情可参考 [Github Demo](https://github.com/volcengine/veGame/tree/master/QuickStart/Android)。

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

> 最佳实践：保存获取到的设备唯一标识到业务的服务端。在通过调用服务端 PreAllocateResource 接口锁定资源、请求游戏服务时，透传保存的设备唯一标识，用于确定是否支持游戏多开。

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

描述：在网速探索过程中，可使用 `veGameEngine` 类中的 `probeInterrupt` 接口来取消网速探测。网速探测终止完成后，会通过 [IProbeNetworkListener](#IProbeNetworkListener) 接口收到 `onProbeError` 回调，错误码是 2，标识探测过程被取消。

参考示例：

```java
// veGameEngine class
public void probeInterrupt()
```

### 开始播放

描述：当成功初始化完成后，可以调用 `start()`，拉流开始播放。

入参：config

返回值：开始播放触发的回调为 `IStreamListener` 的 `onFirstRemoteVideoFrame`

参考以下示例：

```java
public void start(@NonNull GamePlayConfig config, @NonNull IPlayerListener playerListener)
```

`config` 字段描述如下:

|  **名称**  |  **类型**  |  **是否必填**  |  **说明**  |
| --- | --- | --- | --- |
| userId | String | 是 | 自定义客户端用户 ID，用于标识用户在游戏房间中的身份，命名规则如下：  <br>-最大长度为64位的非空字符串，支持的字符集范围为:  <br>-大写字母 A ~ Z  <br>-小写字母 a ~ z  <br>-数字 0 ~ 9  <br>-下划线、减号 |
| ak | String | 是 | 用于用户鉴权的临时 Access Key，通过调用服务端 STSToken 接口获取，参考 [生成临时 Token 接口]() |
| sk | String | 是 | 用于用户鉴权的临时 Secret Key，通过调用服务端 STSToken 接口获取，参考 [生成临时 Token 接口]() |
| token | String | 是 | 用于用户鉴权的临时 Token，通过调用服务端 STSToken 接口获取，参考 [生成临时 Token 接口]() |
| gameId | String | 是 | 游戏 ID，可通过火山引擎云游戏控制台『游戏管理』页面获取，例如：1428112352161312345；当传入 customGameId 时，可不传入（以 game_id 优先） |
| customGameId | String | 是 | 注册游戏时指定的用户自定义游戏 ID；当传入 gameId 时，可不传入 |
| roundId | String | 是 | 当次游戏生命周期的标识符，命名规则如下：  <br>-最大长度为128位的非空字符串，支持的字符集范围为:  <br>-大写字母 A ~ Z  <br>-小写字母 a ~ z  <br>-数字 0 ~ 9  <br>-下划线、减号 |
| roomType | Int | 否 | 启动游戏的场景，用于控制是否开启多人游戏及游戏控制权转移：  <br>0（单用户，默认）  <br>1（单房间多用户，不可转移游戏控制权）  <br>2（单房间多用户, 可转移游戏控制权） |
| role | Role | 否 | 启动游戏时，游戏玩家的角色：  <br>Role.PLAYER（操作者）  <br>Role.VIEWER（观看者）；注：当游戏场景类型为1和2时，如不指定则游戏玩家的角色默认为观看者（游戏场景类型为1或2时才有多用户之分） |
| container | FrameLayout 或其子类 | 是 | 用来承载画面的 Container |
| planId | String | 否 | 火山侧套餐 ID，可通过调用服务端 ListResourceSet 接口获取（configuration_code 字段） |
| enableFileChannel | Boolean | 否 | 是否使能文件传输通道（默认为 false） |
| enableLocalKeyboard | Boolean | 否 | 是否使能手机本地输入法，（默认为 false）；注：焦点拦截、以及 activity 设置能弹出软键盘等，都会导致本地键盘无法弹出 |
| keyBoardEnable | Boolean | 否 | 默认输入法开启，true ：开启， false: 关闭（默认为 true） |
| enableMagneticSensor | Boolean | 否 | 是否使能磁力传感器（默认为 false） |
| enableAcceleratorSensor | Boolean | 否 | 是否使能加速度传感器（默认为 false） |
| enableGravitySensor | Boolean | 否 | 是否使能重力传感器（默认为 false） |
| enableGyroscopeSensor | Boolean | 否 | 是否使能陀螺仪传感器（默认为 false） |
| enableLocationService | Boolean | 否 | 是否使能定位服务（默认为 false） |
| enableVibrator | Boolean | 否 | 是否开启震动（默认为 true） |
| videoStreamProfileId | Int | 否 | 游戏视频流清晰度 ID；如不传入，则使用默认清晰度（清晰度档位 ID 列表和详细说明，参考 [云游戏清晰度档位说明]()） |
| userTag | String | 否 | 用户标签，由业务方指定，可通过调用服务端 CreateUserTag 接口创建，参考 [创建用户属性]() |
| reservedId | String | 否 | 资源预锁定 ID，通过调用服务端 PreAllocateResource 接口获取（如不传入，将在开始播放成功 onPlaySuccess 回调中返回）；资源预锁定 ID 用于申请火山引擎云游戏的每次服务，建议业务记录此 ID，用于每次服务会话的唯一标识，可用于调试和问题排查 |
| autoRecycleTime | Int | 否 | 设置无操作自动回收服务时长，单位秒（如不设置或设置为0，则使用默认时长300秒，支持设置的上限值为7200，即2小时） |
| userProfilePath | String[] | 否 | 保存用户游戏配置文件的路径列表（如需在游戏进行中通过 setUserProfilePath 接口设置保存配置文件的路径，需要配置该参数） |
| extra | Map<String,String> | 否 | 根据业务需要，自定义的扩展参数；详细信息，参考 [自定义扩展参数列表]() |

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

#### IGamePlayerListener

拉流播放状态回调监听：

|  **接口名称**  |  **接口描述**  |
| --- | --- |
| onServiceInit() | 加入房间前回调，用于获取并初始化各个功能服务，例如设置各种事件监听回调 |
| onPlaySuccess(String roundId, int videoStreamProfileId, Map<String, String> extra, String gameId, String reservedId)
 | 播放成功回调：  <br>roundId：当次游戏生命周期标识符  <br>videoStreamProfileId：当前游戏画面的清晰度，首帧渲染到画面时触发该回调  <br>extra：自定义的扩展参数  <br>gameId：游戏 ID  <br>reservedId：资源预锁定 ID  |
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
| onStreamStats(StreamStats streamStats) | 视频流的当前性能状态回调（2秒周期内音视频网路状态的回调，可用于内部数据分析或监控）：  <br>streamStats：远端数据间视频流的性能状态，参考以下 [StreamStats 类型说明]() |
| onStreamConnectionStateChanged(int state) | 视频流连接状态变更回调：  <br>1：连接断开  <br>2：首次连接，正在连接中  <br>3：首次连接成功  <br>4：连接断开后重新连接中  <br>5：连接断开后重连成功  <br>6：网络连接断开超过 10 秒，仍然会继续重连 |
| onNetworkQuality(int quality) | 游戏中网络质量回调，每隔 2 秒上报一次网络质量评级：  <br>quality：网络质量评级（可根据当前返回的网络质量评级进行推流参数降级或者终止拉流；详细信息，参考以下 [onNetworkQuality 相关信息]()） |
| onDetectDelay(long  elapse) | 当前操作延时回调，单位毫秒（操作延时获取是指在操作时发送到远端的消息，本地记录的时间戳，收到远端视频流会带上操作延时的标记，从而计算出来的一个值，该值可以理解为操作和对应画面渲染更新的一个差值） |
| onRotation(int rotation) | 客户端旋转回调 |
| onPodExit(int reason, String msg) | 实例端退出回调，参考以下 [onPodExit 相关信息]() |

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
| 40011 | MESSAGE_START_RESERVED_ID_EXPIRED | 资源预锁定 ID 已过期。建议：调用服务端 [PreAllocateResource]() 接口重新获取。 |
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
| 40036 | WARNING_USER_PROFILE_RESTORE_FAILURE | 用户存档信息还原失败。建议：请联系火山引擎云游戏服务技术支持。 |
| 40047 | MESSAGE_RESOURCE_RELEASED_INVALID_PARAMETER | 云端实例接收到的参数非法。 |
| 40048 | MESSAGE_RESOURCE_RELEASED_HEART_BEAT_TIMEOUT | 云端实例离线60秒，中间没有任何心跳。 |
| 40049 | MESSAGE_RESOURCE_RELEASED_INGAME_EXIT | 游戏停止。原因：一般是因为云端运行的游戏主动退出了。建议：客户端需要提示用户目前游戏主动被结束了，如果需要，可以重新开始游戏。 |

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
