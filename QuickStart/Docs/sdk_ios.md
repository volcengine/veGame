# 火山引擎云游戏 iOS 客户端 SDK 接入说明  

本文档描述了火山引擎云游戏 iOS 客户端 SDK 的使用说明，方便开发者快速接入。

## 集成准备

### SDK 下载

|  **SDK 包文件**  |  **快速接入 Demo（GitHub 仓库）**  |
| --- | --- |
| [前往云游戏控制台](https://console.volcengine.com/veGame/guidepage?activeStep=sdk) | [Quick Start Demo](https://github.com/volcengine/veGame/tree/master/QuickStart/iOS) |

### 环境要求

* iOS 9.0 及以上系统版本的设备
* 使用 Objective-C 开发语言

说明：本文档中涉及编译器的示例图参考 Xcode 12.4 版本。

### 工程配置

1. 下载 veGameSDK 压缩包后，解压得到 `VeGame.framework` 和 `VolcEngineRTC.framework` 文件夹。手动将 `VeGame.framework` 和 `VolcEngineRTC.framework` 拖入目标工程，选择如下：

    ![image.png](ios_1)

2. 添加 `VeGame.framework` 和 `VolcEngineRTC.framework`：

    ![image.png](ios_2)


3. 修改 Build Settings  设置：

第一步：

    ![image.png](ios_3)

第二步：

    ![image.png](ios_4)

4. 权限声明，在 `info.plist` 文件中配置“相机、麦克风、定位”相关使用权限说明：


|  **字段**  |  **描述**  |
| --- | --- |
| Privacy -  Camera Usage Description | 允许访问使用“相机”功能权限 |
| Privacy - Microphone Usage Description | 允许访问使用“麦克风”功能权限 |
| Privacy - Location When In Use Usage Description | 允许访问使用“定位”功能权限 |

## 快速接入

### 初始化 VeGameManager 配置

参考以下代码，初始化并启动云游戏：

参考示例：

```objectivec
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // 设置配置信息，建议在程序一启动的时候就调用
    [VeGameManager setupConfigWithAccountId: AccountId];
}
```

### 创建 VeGameManager 实例

描述：初始化云游戏实例、设置画布、配置 VeGameConfigObject 配置参数，然后调用 startWithConfig: configObj 接口，申请云游戏服务。

参考示例：

```objectivec
// 初始化云游戏实例
[VeGameManager sharedManagerWithContainerView: self.gameDisplayVc.containerView delegate: self];
// 启动云游戏
VeGameConfigObject *configObj = [VeGameConfigObject new];
configObj.ak = @"";
configObj.sk = @"";
configObj.token = @"";
configObj.userId = @"";
configObj.gameId = @"";
configObj.roundId = @"";
[[VeGameManager sharedInstance] startWithConfig: configObj];

#pragma mark - VeGameManagerDelegate
- (void)gameManager:(VeGameManager *)manager startSucceedResult:(NSString *)gameId videoStreamProfileId:(NSInteger)streamProfileId reservedId:(NSString *)reservedId extra:(NSDictionary *)extra
{
    // 启动成功
}

- (void)gameManager:(VeGameManager *)manager changedDeviceRotation:(NSInteger)rotation
{
    // 旋转回调
}

- (void)gameManager:(VeGameManager *)manager onWarning:(VeGameWarningCode)warnCode
{
    // 警告回调
}

- (void)gameManager:(VeGameManager *)manager onError:(VeGameErrorCode)errCode
{
    // 错误回调
}
```

#### 配置参数

`VeGameConfigObject` 配置参数如下：

```objectivec
@interface BDCPGameConfiguration : NSObject
/** 自定义用户ID，需与调用服务端PodStart接口启动实例时传入的客户端用户ID一致 */
@property (nonatomic, copy) NSString *userId;
/** 游戏类型 */
@property (nonatomic, assign) BDCPGameType gameType;
/** 应用ID，通过火山引擎控制台右上角 个人>我的应用 中创建应用并获取分配的APPID */
@property (nonatomic, copy) NSString *appId;
/** 房间号，云游戏流媒体中的房间 ID，通过调用服务端PodStart接口返回 */
@property (nonatomic, copy) NSString *roomId;
/** Pod ID，通过云游戏控制台实例管理页面获取 */
@property (nonatomic, copy) NSString *podId;
/** 用户签名，加入流媒体房间的令牌，通过调用服务端PodStart接口返回 */
@property (nonatomic, copy) NSString *userToken;
/** pod端用户ID，流媒体服务推流所用 User ID，通过调用服务端PodStart接口返回 */
@property (nonatomic, copy) NSString *podUserId;
/** 游戏ID，通过云游戏控制台游戏管理页面获取 */
@property (nonatomic, copy) NSString *gameId;
/** 云端实例中游戏的旋转角度 */
@property (nonatomic, assign) NSInteger gameRotation;

@end
```

### 接口说明

控制云游戏的主要接口和说明如下（包括流媒体控制等接口）：

```
/// 云游戏启动接口
/// @param gameConfigObj 配置项
/// @param viewController 当前控制器
- (void)startGameWithConfigObject:(BDCPGameConfigObject *)gameConfigObj fromViewController:(UIViewController *)viewController;

/// 当前业务的场景，默认：nil
/// @param scenes 例如：CloudGame_TT（头条云游戏）
- (void)configureCurrentScenes:(NSString *)scenes;

/// 开始循环检测
/// @param interval 间隔，最小值：1s
- (void)startLoopDelayDetectionWithTimeInterval:(NSTimeInterval)interval;

/// 停止循环检测
- (void)stopLoopDelayDetection;

/// 设置启动时的超时时限（仅针对Rtc, 且在调用启动接口前设置时生效）
/// @param timeoutLimit 超时时限 默认为10s
- (void)setLaunchTimeoutLimit:(NSInteger)timeoutLimit;

/// 启动时网络探测开关，默认：关闭，每次退出房间时关闭，若要打开，每次启动云游戏或云手机前需要重新设置
/// @param enable 是否打开网络探测
/// @param detectionTimes 网络探测次数，该参数仅在enable为YES时生效，若参数值不合理，sdk将使用默认值进行探测
- (void)launchTimeNetworkDetectionEnable:(BOOL)enable andDetectionTimes:(NSInteger)detectionTimes;

/// 切换订阅视频流
/// @param videoIndex 需要订阅的视频流下标
- (void)switchVideoStreamWithIndex:(NSInteger)videoIndex;

/// 获取当前视频流，全部分辨率
- (NSArray<NSString *> *)getCurrentStreamAllVideoStreamDescriptions;

/// 是否关注该消息的Ack，当attention为YES，会收到消息发送结果的回调（gameManager:didReceiveTextMessageWithMessageId:andResult:）
/// @param message 消息实体
/// @param attention 是否关注
- (void)sendTextMessage:(BDCPTextMessage *)message withAttention:(BOOL)attention;

/// 打开/切换游戏
/// @param packageName 即将打开/切换到的游戏的instanceId或包名，优先匹配instanceId，然后包名。若为空，将对当前前台游戏进行操作
- (void)launchGame:(NSString * _Nullable)packageName;

/// 关闭游戏
/// @param packageName 需要关闭的游戏的instanceId或包名，优先匹配instanceId，然后包名。若为空，将对当前前台游戏进行操作
- (void)closeGame:(NSString * _Nullable)packageName;

/// 重启游戏
/// @param packageName 需要重启的游戏的instanceId或包名，优先匹配instanceId，然后包名。若为空，将对当前前台游戏进行操作
- (void)restartGame:(NSString * _Nullable)packageName;

/// 设置游戏手柄
/// @param gamepadView 手柄视图
- (void)setGamepadView:(UIView<BDCPCustomOperationViewProtocol> * _Nullable)gamepadView;

/// 数据通道
/// @param operationType 操作类型
/// @param jsonString JSON信息
- (void)sendOperationWithType:(BDCPOperationType)operationType andData:(NSString *)jsonString;

/// 退出，注销资源
- (void)exit;

/// 提高远端pod音量
- (void)turnUpPodVolume;

/// 降低远端pod音量
- (void)turnDownPodVolume;

/// 暂停开关
/// @param isPaused 默认：NO
- (void)setIsPaused:(BOOL)isPaused;
```

#### 主流开关

控制云游戏的主要开关类接口和说明如下（包括传感器开关等接口）：

```
/// 加速计开关
/// @param enable 默认：YES
- (void)setAccelerometerEnabled:(BOOL)enable;

/// 磁力计开关
/// @param enable 默认：YES
- (void)setMagnetometerEnabled:(BOOL)enable;

/// 陀螺仪开关
/// @param enable 默认：YES
- (void)setGyroEnabled:(BOOL)enable;

/// 重力传感器开关
/// @param enable 默认：YES
- (void)setGravityEnabled:(BOOL)enable;

/// 方向开关
/// @param enable 默认：YES
- (void)setOrientationEnabled:(BOOL)enable;

/// 地理位置请求开关
/// @param enable 默认：YES
- (void)setLocationRequestEnabled:(BOOL)enable;

/// 相机开关
/// @param enable 默认：NO
/// @param customMediaManager 自定义媒体服务，如果值为nil，将使用默认的媒体服务
/// @param fromViewController 展示相机控制界面的ViewController
- (void)setCameraEnable:(BOOL)enable withCustomMediaManager:(NSObject<BDCPMediaManagerProtocol> * _Nullable)customMediaManager withFromViewController:(UIViewController * _Nonnull)fromViewController;

/// 启动时网络探测开关，默认：关闭，每次退出房间时关闭，若要打开，每次启动云游戏或云手机前需要重新设置
/// @param enable 是否打开网络探测
/// @param detectionTimes 网络探测次数，该参数仅在enable为YES时生效，若参数值不合理，sdk将使用默认值进行探测
- (void)launchTimeNetworkDetectionEnable:(BOOL)enable andDetectionTimes:(NSInteger)detectionTimes;
```

#### API 接口参考

有关云游戏的所有 API 接口，可以参考 `BDCPCloudGameManager.h` 文件。


### 云手机接口说明

#### 初始化

参考以下代码，初始化并启动云手机：

```
// 创建 BDCPCloudPhoneManager 实例
self. cloudPhoneManager = [[BDCPCloudGameManager alloc] initWithCustomDisplayVc: nil podScreenRatio: 0.5 delegate: self];
// 传入参数启动云手机
// phoneConfiguration 参考BDCPPhoneConfiguration
// self 一般为当前控制器
[self.cloudPhoneManager startWithPhoneConfiguration: phoneConfiguration fromViewController: self];

#pragma mark - BDCPCloudPhoneManagerDelegate

- (void)startPhoneSuccess
{
  // 启动成功，收到首帧回调
}

- (void)didPhoneOccurError:(BDCPPhoneErrorCode)errorCode
{
  // 加载失败，详情见BDCPPhoneErrorCode
}
```

#### 启动配置文件

启动云手机的配置参数如下：

```
@interface BDCPPhoneConfiguration : NSObject
/** 应用 ID，通过云手机控台应用管理页面获取 */
@property (nonatomic, copy) NSString *appId;
/** 自定义用户ID，需与调用服务端PodStart接口启动实例时传入的客户端用户ID一致 */
@property (nonatomic, copy) NSString *userId;
/** 房间ID，云游戏流媒体中的房间ID，通过调用服务端PodStart接口返回 */
@property (nonatomic, copy) NSString *roomId;
/** 用户签名，加入流媒体房间的令牌，通过调用服务端PodStart接口返回 */
@property (nonatomic, copy) NSString *userToken;
/** 云端用户ID，流媒体服务推流所用 User ID，通过调用服务端PodStart接口返回 */
@property (nonatomic, copy) NSString *gamePodUserId;

@end
```

#### 主流接口

控制云手机的主要接口和说明如下：

```
/// 云手机启动接口
/// @param phoneConfigObj 配置项
/// @param viewController 当前控制器
- (void)startWithPhoneConfigObject:(BDCPPhoneConfigObject *)phoneConfigObj fromViewController:(UIViewController *)viewController;

/// 当前业务的场景，默认：nil
/// @param scenes 例如：CloudPhone_TT（头条云手机）
- (void)configureCurrentScenes:(NSString *)scenes;

/// 设置启动时的超时时限（仅针对Rtc, 且在调用启动接口前设置时生效）
/// @param timeoutLimit 超时时限 默认：10s
- (void)setLaunchTimeoutLimit:(NSInteger)timeoutLimit;

/// 启动循环延迟检测
/// @param interval 间隔，最小值：1s
- (void)startLoopDelayDetectionWithTimeInterval:(NSTimeInterval)interval;

/// 停止循环延迟检测
- (void)stopLoopDelayDetection;

/// 切换订阅视频流
/// @param videoIndex 需要订阅的视频流下标
- (void)switchVideoStreamWithIndex:(NSInteger)videoIndex;

/// 获取当前视频流，全部分辨率
- (NSArray<NSString *> *)getCurrentStreamAllVideoStreamDescriptions;

/// 接入方需要实现的的文件上传服务，在使用通讯录服务或相机服务等需要文件上传作为支持的服务前，务必设置该服务
/// @param fileUploadService 文件上传服务实例
- (void)setFileUploadService:(NSObject<BDCPFileUploadServiceProtocol> *)fileUploadService;

/// 启动时网络探测开关，默认：关闭，每次退出房间时关闭，若要打开，每次启动云游戏或云手机前需要重新设置
/// @param enable 是否打开网络探测
/// @param detectionTimes 网络探测次数，该参数仅在enable为YES时生效，若参数值不合理，sdk将使用默认值进行探测
- (void)launchTimeNetworkDetectionEnable:(BOOL)enable andDetectionTimes:(NSInteger)detectionTimes;

/// 是否关注该消息的Ack，当attention为YES，会收到消息发送结果的回调（phoneManager:didReceiveTextMessageWithMessageId:andResult:）
/// @param message 消息实体
/// @param attention 是否关注
- (void)sendTextMessage:(BDCPTextMessage *)message withAttention:(BOOL)attention;

/// 退出，注销资源
- (void)exit;

/// 暂停开关
/// @param isPaused 默认：NO
- (void)setIsPaused:(BOOL)isPaused;
```

#### 主流开关

控制云手机的主要开关类接口和说明如下：

```
/// 剪帖板同步开关
/// @param enable 默认：YES
- (void)setPasteBoardSyncEnable:(BOOL)enable;

/// 地理位置开关
/// @param enable 默认：YES
- (void)setLocationRequestEnable:(BOOL)enable;

/// 本地键盘开关
/// @param enable 默认：YES
- (void)setLocationKeyboardEnable:(BOOL)enable;

/// 云通知开关
/// @param enable 默认：NO
- (void)setCloudNotificationEnable:(BOOL)enable;

/// 相机开关
/// @param enable 默认：NO
/// @param customMediaManager 自定义媒体服务，如果值为nil，将使用默认的媒体服务
/// @param fromViewController 展示相机控制界面的ViewController
- (void)setCameraEnable:(BOOL)enable withCustomMediaManager:(NSObject<BDCPMediaManagerProtocol> * _Nullable)customMediaManager withFromViewController:(UIViewController * _Nonnull)fromViewController;

/// 通讯录开关
/// @param enable 默认：YES
/// @param customContactsService 自定义的通讯录服务，值为nil时将使用默认的通讯录服务，所有通讯录服务仅在总开关打开时有效
- (void)setContactsServiceEnable:(BOOL)enable withCustomContactsService:(NSObject<BDCPContactsServiceProtocol> * _Nullable)customContactsService;
```

#### API 接口参考

有关云手机的所有 API 接口，可以参考 `BDCPCloudPhoneManager.h` 文件。

## 调试和日志

### 回调接口

调试和日志相关接口如下：

```
// 云游戏错误回调
- (void)gameManager:(BDCPCloudGameManager *)manager didGameOccurError:(BDCPGameErrorCode)errorCode

// 云手机错误回调
- (void)phoneManager:(BDCPCloudPhoneManager *)manager didPhoneOccurError:(BDCPPhoneErrorCode)errorCode
```

### 错误码

可能返回的报错信息如下：

|  **错误码**  |  **说明**  |
| --- | --- |
| 1000 | 初始化失败 |
| 1001 | 定位服务权限受限 |
| 1002 | 定位服务权限申请被拒 |
| 1003 | 麦克风权限受限 |
| 1004 | 麦克风权限申请被拒 |
| 1005 | 通知权限申请被拒 |
| 1006 | 通讯录权限申请被拒 |
| 1007 | 文件上传服务实例为空 |
| 1008 | 相机权限申请被拒 |
| 1009 | 传输内容的大小超出限制 |
| 1010 | 消息发送失败 |
| 1011 | 订阅失败 |
| 1012 | Token 无效 |
| 1013 | 加入房间错误 |
| 1014 | 没有发布音视频流权限 |
| 1015 | 没有订阅音视频流权限 |
| 1016 | 用户重复登录 |
| 1017 | 订阅音视频流总数超过上限 |
| 1018 | 发布流失败，发布流总数超过上限 |
| 1019 | 发布屏幕流失败，发布流总数超过上限 |
| 1020 | 发布视频流总数超过上限 |
