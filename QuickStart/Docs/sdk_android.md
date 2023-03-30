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

```
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

```
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

```
implementation 'com.volcengine.vegame:vegame:1.22.0'
```

- （方式三，非必须）在应用模块的 build.gradle 文件中的 dependencies 中添加 veGameSDK 以及插件包的依赖，参考以下示例：

```
implementation 'com.volcengine.vegame:vegame:1.22.0'
implementation 'com.volcengine.vegame:core-full:1.22.0'
```

3. 设置 Java 版本到 1.8，参考以下示例：

```
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

```
<meta-data
    android:name="VOLC_ACCOUNT_ID"
    android:value="21000xxxxx" />
```    

**权限声明**

根据实际场景在 `AndroidManifest.xml` 文件中声明 SDK 需要的权限，参考以下示例：

```
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

```
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

```
// veGameEngine class
int getStatus()
```

```
// veGameEngine class
void addCloudCoreManagerListener(@NonNull ICloudCoreManagerStatusListener listener)

void removeCloudCoreManagerListener(@NonNull ICloudCoreManagerStatusListener listener)

public interface ICloudCoreManagerStatusListener {
    void onInitialed();
}
```

**addCloudCoreManagerListener(listener)**

描述：设置 veGameEngine 生命周期回调监听。

**removeCloudCoreManagerListener(listener)**

描述：取消 veGameEngine 生命周期回调监听。

**ICloudCoreManagerStatusListener**

描述：veGameEngine 生命周期回调。

|  **状态**  |  **说明**  |
| --- | --- |
| onInitialed() | SDK 初始化成功回调 |


### 初始化

描述：初始化 veGameEngine，建议在 Application 初始的时候调用。

> 说明：
> - 第一次初始化时，SDK 会进行一次资源异步加载。如果在初始化时资源加载失败，不需要重新调用 init。SDK 会在调用 start 接口启动游戏时检查资源是否已成功加载，如果没有，则会重新加载。资源加载完成后会由 ICloudCoreManagerStatusListener 的 onInitialed() 回调。
> - 初始化只需要执行一次。特别注意：多进程会多次执行 Application 的 onCreate，需要在 onCreate 中添加当前进程判断，详情可参考 [Github Demo](https://github.com/volcengine/veGame/tree/master/QuickStart/Android)。

```
/**
 * veGameEngine class
 * @param application Android上下文
 */
public void init(Application application)
```

#### 火山引擎账号

在申请云游戏服务之前，需要在 `\app\src\main\AndroidManifest.xml` 文件的 `meta-data` 中填入注册的火山引擎用户账号（`VOLC_ACCOUNT_ID`）。参考以下示例：

```
<meta-data
    android:name="VOLC_ACCOUNT_ID"
    android:value="21000xxxxx" />
```    

> 可通过火山引擎官网页面右上角 **用户 > 账号管理 > 主账号信息** 获取用户账号。

### 获取设备唯一标识

描述：获取用户终端设备的唯一标识。

> 最佳实践：保存获取到的设备唯一标识到业务的服务端。在通过调用服务端 PreAllocateResource 接口锁定资源、请求游戏服务时，透传保存的设备唯一标识，用于确定是否支持游戏多开。

参考以下示例：

```
// veGameEngine class
public String getServiceDeviceId()
```

### 网络探测

描述：在开始游戏之前，可使用云游戏 SDK 提供的网络测速服务对客户端网络质量进行探测，提示用户当前的网络状态。

#### 启动网速探测

使用 `veGameEngine` 类中的 `probeStart` 接口启动网速探测，探测过程以及探测结果通过 `IProbeNetworkListener` 回调接口进行监听。

参考示例：

```
// veGameEngine class
public void probeStart(@NonNull GamePlayConfig config, @NonNull IProbeNetworkListener listener)
```

**IProbeNetworkListener**

接收网络探测流程和状态监听的回调。

|  **接口名称**  |  **接口描述**  |

| onProbeStarted() | 启动网络测速回调 |
| onProbeProgress(ProbeStats stats) | 网络测速过程中检测状态的回调，此时 ProbeStats 是中间测试状态，仅供参考 |
| onProbeCompleted(ProbeStats stats, int quality) | 网络测速成功结束回调，此时 ProbeStats 标识最终的网络测试结果，quality 标识当前网络推荐值，有以下三个档位：
1（网络极好，可以很流畅地玩游戏）2（网络较好，可以玩游戏）3（网络较差，不推荐玩游戏） |
| onProbeError(int err, String message) | 网络测速异常结束回调，err 标识错误码，message 标识错误信息；错误码说明如下：
1（探测过程网络环境出错，无法完成探测）2（探测过程被终止取消）3（探测过程结束，但没有任何探测结果，通常情况下不会发生） |


#### init()

在申请到 Pod 实例后，进行初始化操作，参考以下示例：

```
CloudPhoneConfig.Builder builder = new CloudPhoneConfig.Builder();
CloudPhoneConfig config = builder.userId("userid") // 用户userid，用于标识在流媒体 room 中的身份
        .deviceId("deviceid") //设备的唯一编号，用于统计区分使用
        .podInfo(podInfo)//podInfo，需要申请，参看步骤(1)
        .container(layout)//用来承载画面的Container, 参数说明: layout 需要是FrameLayout或者FrameLayout的子类
        .width(1080) //container的宽，单位px
        .height(2160) //container的高，单位px
        .enableLocalKeyboard(false) // 是否开启本地键盘。开启后，当pod端需要输入时，会唤起本地键盘。
        .magneticSensor(true) //是否开启磁力传感器
        .acceleratorSensor(true) //是否开启加速度传感器
        .gravitySensor(true) //是否开启重力传感器
        .gyroscopeSensor(true) //是否开启陀螺仪传感器
        .locationService(true) //是否开启自动定位
        .channel("channel")//设置渠道
        .jsonConverter(converter)//设置IJsonConverter实现类，用来做转换json
        .vibrator(true) //是否开启震动同步
        .rotation(90)//旋转角度，竖屏 0 180 / 横屏 90 270
        .build(); //超时设置，默认不限制时长

//CloudPhoneManager class
void init(@NonNull Context context, @NonNull Cloud
Config config, @NonNull IPlayerListener playerListener)
```
 **详细配置说明**

|  **名称**  |  **参数范围**  |  **是否必填**  |  **说明**  |
| --- | --- | --- | --- |
| userId | 字符串，小于64位 | 是 | 客户端用户 ID，用于标识用户在流媒体房间中的身份 |
| podInfo | PodInfo 实例 | 是 | Pod实例信息 |
| conatiner | FrameLayout 或其子类 | 是 | 用来承载画面的 Container |
| width | 单位 px | 是 | Container 的宽 |
| height | 单位 px | 是 | Container 的高 |
| jsonConverter | IJsonConverter 实例 | 是 | Json 转换器 |
| enableLocalKeyboard | true / false | 否 | 是否使能本地输入法 |
| magneticSensor | true / false | 否 | 是否使能磁力传感器 |
| acceleratorSensor | true / false | 否 | 是否使能加速度传感器 |
| gravitySensor | true / false | 否 | 是否使能重力传感器 |
| gyroscopeSensor | true / false | 否 | 是否使能陀螺仪传感器 |
| locationService | true / false | 否 | 是否使能定位服务 |
| channel | 字符串，小于64位 | 否 | 渠道，埋点信息 |
| vibrator | true / false | 否 | 是否开启震动 |
| rotation | \[0, 90, 180, 270\] | 否 | 屏幕旋转角度 |
| deviceId | 字符串小于64位 | 否 | 设备唯一标识，性能统计 |

 **IPlayerListener**

```
public interface IPlayerListener {

    /**
     * 初始化成功
     */
    void onInitSuccess(); //初始化成功

    /**
     * 初始化失败
     * @param code 错误码
     * @param error 错误描述
     */
    void onInitFail(int code, String error);

    /**
     * 播流成功，Play/切分辨率/重连成功均会回调
     */
    void onPlaySuccess();

    /**
     * 播放失败
     *
     * @param code 错误码
     * @param err  错误描述
     */
    void onPlayFail(int code, String err);

    /**
     * 分辨率切换的回调
     *
     * @param result   是否切换成功
     * @param currentId
     */
    void onSwitchResolution(int result, String currentId);


    /**
     * SDK内部产生的事件回调
     * @param event 事件
     */
    void onPlayEvent(CommonEvent event);


    /**
     * SDK内部产生错误回调
     * 详情见【错误码表】
     */
    void onError(int code, String message);

     /**
     * 网络类型状态的切换
     * -1：网络连接类型未知。
     * 0： 网络连接已断开。
     * 1： 网络类型为 LAN 。
     * 2： 网络类型为 Wi-Fi（包含热点）。
     * 3： 网络类型为 2G 移动网络。
     * 4： 网络类型为 3G 移动网络。
     * 5： 网络类型为 4G 移动网络。
     */
    void onNetworkChanged(int type);
}
```

 **CommonEvent 说明**

```
public class CommonEvent {
    public static final int RECONNECTION = 5;
    public static final int FIRST_FRAME = 8;
    public static final int FIRST_FRAME_TIMEOUT = 9;
    public static final int DETECT_DELAY = 12;
    public static final int PERFORMANCE_STATS = 13;
    public static final int CONNECTION_INTERRUPTED = 16;
    public static final int SCREEN_ROTATION = 17;
    public static final int POD_EXIT = 19;
    public static final int PRESSURE_TEST = 20;
    public static final int REMOTE_OFFLINE = 22;
    public final int code;
    public JSONObject data;
}
```

|  **code / 名称**  |  **data (Json)**  |  **说明**  |
| --- | --- | --- |
| 5 / `CommonEvent.RECONNECTION`  | NG |  重连成功  |
| 8 / `CommonEvent.FIRST_FRAME`  | NG | 首帧到达 |
| 9 / `CommonEvent.FIRST_FRAME_TIMEOUT`  | NG | 首帧超时 |
| 12 / `CommonEvent.DETECT_DELAY`  | {  <br>elapse: 10 // long 型  <br>} | 操作延迟(参考)探测  <br>表示耗时 |
| 16 / `CommonEvent.CONNECTION_INTERRUPTED`  | {} | 连接中断 |
| 17 / `CommonEvent.SCREEN_ROTATION`  | {  <br>"rotation":0 // int 型  <br>} | Pod 屏幕旋转  <br>rotation: 0 默认竖屏，90 旋转90度，180 旋转180度，270 旋转270度 |
| 19 / `CommonEvent.POD_EXIT`  | {  <br>"reason":255,// Int 型  <br>"message":"String"  <br>} | 远端 Pod 由于某些原因退出  <br>reason: 255代表正常退出；其他代表异常退出；  <br>message:退出的具体原因说明 |
| 21 / `CommonEvent.DATA_CHANNEL_STATE`  | {  <br>"state":1 // int  <br>} | DataChannel 状态变化  <br>state 取值范围:  <br>1：连接中断  <br>2：连接建立中  <br>3：连接已建立  <br>4：连接重连中  <br>5：连接已重连  <br>6：连接失败 |
| 22 / `CommonEvent.REMOTE_OFFLINE`  | {  <br>reason : 0 // int  <br>message : "string"  <br>} | 远端离线  <br>0: 用户主动离开。  <br>1: 用户掉线。  <br> |

**IStreamListener**

获取媒体流信息。

```
// 在 IPlayerListener onInitSuccess 后注册该接口
CloudPhoneManager.getInstance().setStreamListener(new IStreamListener);
```

```
public interface IStreamListener {
    /**
     *
     * @param uid 远端pod视频流id
     * @param elapsed 订阅视频流到收到音频首帧的时间
     */
    void onFirstAudioFrame(String uid, long elapsed);

    /**
     *
     * @param uid 远端pod视频流id
     * @param elapsed 订阅视频流到收到视频首帧的时间
     */
    void onFirstRemoteVideoFrame(String uid, long elapsed);

    void onStreamStarted();

    void onStreamPaused();

    void onStreamResumed();

    /**
     * 视频流的当前性能状态
     *
     * @param streamStats 远端数据间 stremstatus
     */
    void onStreamStats(StreamStats streamStats);

    /**
     * 视频流网络异常
     *
     * @param state
     */
    void onStreamConnectionStateChanged(int state);
}
```

 **StreamStats**

```
public class StreamStats implements ModelPool.PoolModel {

    /**
     * 视频接收码率（kbps），瞬时值
     */
    private int receivedVideoBitRate;

    /**
     * 音频接收码率（kbps），瞬时值
     */
    private int receivedAudioBitRate;

    /**
     * 解码器输出帧率，单位 fps
     */
    private int decoderOutputFrameRate;

    /**
     * 渲染帧率，单位 fps
     */
    private int rendererOutputFrameRate;

    /**
     * 远端视频流高度
     */
    private int receivedResolutionHeight;

    /**
     * 远端视频流宽度
     */
    private int receivedResolutionWidth;


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
}
```

#### play()

当 CloudPhoneManager 触发 `IPlayerListener` 的 `onInitSuccess` 回调后，可以调用 `play()`，然后拉流开始播放。播放触发的回调为 `IStreamListener` 的 `onFirstRemoteVideoFrame`。

```
//CloudPhoneManager class
void play()
```

#### pause()

当 CloudPhoneManager 处于播放状态时，调用 `pause()` 暂停从云端拉流，此时并不改变云端的运行状态。

```
//CloudPhoneManager class
void pause()
```

#### resumePlay()

当 CloudPhoneManager 处于暂停状态时，调用 `resumePlay()` 重新从云端拉流播放。

```
//CloudPhoneManager class
void resumePlay()
```

#### stop()

停止从云端拉流，并且退出房间，但不会回收资源。

```
//CloudPhoneManager class
void stop()
```

#### release()

销毁实例，释放资源。如果没有调用 `stop()` 接口，则会直接停止从云端拉流并注销资源。

```
//CloudPhoneManager class
void release()
```

#### restart()

重启远端 pod 当前的游戏或应用。

```
//CloudPhoneManager class
void restart()
```

### 其它功能接口说明

#### 静音开关

媒体流静音开关，及检查静音状态。

```
//CloudPhoneManager class
void muteAudio(boolean mute) //true: 静音; false: 开音。
boolean isAudioMuted() //当前是否处在静音状态，返回：true: 静音； false: 开音。
```

#### 麦克风开关

打开后会将本地麦克风数据发送给云端pod，进而注入到pod的Android系统中。注意: 需要开启麦克风使用权限。

```
//CloudPhoneManager class
void enableMic(boolean enable) //激活本地麦克风。
boolean isEnableMic() //获取当前麦克风激活状态。
```

#### 增大/减小pod音量

增大或减小媒体流音量。

```
// CloudPhoneManager class
void volumeUp()
void volumeDown()
```

#### 切换清晰度

```
//CloudPhoneManager class
List<IVideoDescription> getSupportVideoDescList() //获取当前支持的分辨率列表，详情参考IVideoDescription
void switchResolution(IVideoDescription info) //选择要切换的分辨率
```

**IVideoDescription**

```
public interface IVideoDescription {
    String getId();

    /**
     * 分辨率名称
     */
    String getName();

    /**
     * 分辨率，形如<width>x<height>
     *
     * @return
     */
    String getResolution();

    /**
     * 峰值码率， 单位 KB/s
     *
     * @return
     */
    String getPeakBitRate();

    /**
     * 码率，单位为 bit/s, Long
     *
     * @return
     */
    String getBitRate();

    /**
     * 帧率, Int
     *
     * @return
     */
    int getFrameRate();

    /**
     * 是否是默认选择
     *
     * @return
     */
    boolean isDefaultChoice();

    /**
     * 是否开启
     *
     * @return
     */
    boolean isEnable();
}
```

#### 传感器开关

获取传感器使用权限。

```
//CloudPhoneManager class
void enableAccelSensor(boolean enable) //是否使能加速度传感器
void enableGyroscopeSensor(boolean enable) //是否使能陀螺仪传感器
void enableGravitySensor(boolean enable) //是否使能重力传感器
void enableOrientationSensor(boolean enable) //是否使能方向传感器
void enableMagneticSensor(boolean enable) //是否使能磁力传感器
```

#### 震动开关

本地震动传感器开关。开启后，当云端的应用令手机发生震动时，本地手机会同步震动。注意：需要打开系统的震动开关后才能生效。

```
//CloudPhoneManager class
void enableVibrator(boolean enable)
```

#### 定位开关

```
// CloudPhoneManager class
void enableLocationService(boolean enable) //是否打开定位开关
```


#### 屏幕旋转

旋转游戏画面方向。

```
void rotate(int orientation) //orientation:0，90，180，270。0为默认竖屏方向，其他角度为顺时针旋转的角度
```

#### 发送KeyEvent

发送 Android KeyEvent，参考：[开发者文档](https://developer.android.com/reference/android/view/KeyEvent)

```
void sendKeyEvent(int keyCode) //发送 Android KeyEvent
```

#### 启动/关闭云端App

```
// CloudPhoneManager class
void launchApp(String pkgName) //启动云端App及将云端App置于前台，参数：pkgName 应用包名
void closeApp(String pkgName) //关闭云端App，参数：pkgName 应用包名
```

#### 设置Debug模式

设置调试模式，在该模式下会打印日志信息。

```
static void setDebug(boolean debug) //当设置为debug时，会打印log信息
```

#### 设置Logger

```
// CloudPhoneManager class
static void setLogger(ILogger logger) //设置自定义Logger
```
**ILogger**

```
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
```


#### 获取 SDK 版本信息

```
// CloudPhoneManager class
static String getSDKVersion()
```

## 错误码

错误码回调接口为 `IPlayerListener#onError`，具体错误原因见回调接口中的 `errorMsg` 。

|  **错误码**  |  **描述**  |
| --- | --- |
| 1000 | 初始化错误 |
| 1001 | 状态错误，即当前状态不支持用户的操作 |
| 1002 | 连接云端超时 |  |
| 1003 | 订阅云端超时 |  |
| 1900 | 未知错误 |  |
| 1901 | 没有权限 |  |
| 1902 | 解析 json 失败 |  |
| 1903 | 流媒体内部错误 |  |
| 1904 | 设置流媒体渲染错误 |  |
| 2100 | 多媒体流发生错误 |  |
| 2200 | 发送消息的大小超出 64KB 限制 |  |
| 2201 | 与云端未建立连接的情况下发送信息 |  |
