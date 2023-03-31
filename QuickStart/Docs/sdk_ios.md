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

![alt 工程配置](media/ios_1.png)

2. 添加 `VeGame.framework` 和 `VolcEngineRTC.framework`：

![alt 工程配置](media/ios_2.png)

3. 修改 Build Settings  设置：

第一步：

![alt 工程配置](media/ios_3.png)

第二步：

![alt 工程配置](media/ios_4.png)

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

|  **参数**  |  **类型**  |  **是否必填**  |  **描述**  |
| --- | --- | --- | --- |
| ak | NSString | 是 | 用于用户鉴权的临时 Access Key，通过调用服务端 STSToken 接口获取，参考 [生成临时 Token 接口]() |
| sk | NSString | 是 | 用于用户鉴权的临时 Secret Key，通过调用服务端 STSToken 接口获取，参考 [生成临时 Token 接口]() |
| token | NSString | 是 | 用于用户鉴权的临时 Token，通过调用服务端 STSToken 接口获取，参考 [生成临时 Token 接口]() |
| userId | NSString | 是 | 自定义客户端用户 ID，用于标识用户在游戏房间中的身份，命名规则如下：  <br>-最大长度为64位的非空字符串，支持的字符集范围为:  <br>-大写字母 A ~ Z  <br>-小写字母 a ~ z  <br>-数字 0 ~ 9  <br>-下划线、减号 |
| roundId | NSString | 是 | 当次游戏生命周期的标识符，命名规则如下：  <br>-最大长度为128位的非空字符串，支持的字符集范围为:  <br>-大写字母 A ~ Z  <br>-小写字母 a ~ z  <br>-数字 0 ~ 9  <br>-下划线、减号 |
| gameId | NSString | 是 | 游戏 ID，可通过火山引擎云游戏控制台『游戏管理』页面获取，例如：1428112352161312345；当传入 customGameId 时，可不传入（以 game_id 优先） |
| customGameId | NSString | 是 | 注册游戏时指定的用户自定义游戏 ID；当传入 gameId 时，可不传入 |
| planId | NSString | 否 | 火山侧套餐 ID，可通过调用服务端 ListResourceSet 接口获取（configuration_code 字段） |
| reservedId | NSString | 否 | 资源预锁定 ID，通过调用服务端 PreAllocateResource 接口获取（如不传入，将在开始播放成功回调中返回）；资源预锁定 ID 用于申请火山引擎云游戏的每次服务，建议业务记录此 ID，用于每次服务会话的唯一标识，可用于调试和问题排查 |
| control | VeGameControlObject | 否 | 游戏支持控制权转移功能；详细参数配置，参考以下 [游戏控制权转移参数](#游戏控制权转移参数) |
| videoStreamProfileId | NSInteger | 否 | 游戏视频流清晰度 ID；如不传入，则使用默认清晰度（清晰度档位 ID 列表和详细说明，参考 [云游戏清晰度档位说明]()） |
| autoRecycleTime | NSInteger  | 否 | 设置无操作自动回收服务时长，单位秒（如不设置或设置为0，则使用默认时长300秒，支持设置的上限值为7200，即2小时） |
| userProfilePath | NSArray<NSString> | 否 | 保存用户游戏配置文件的路径列表（如需在游戏进行中通过 setUserProfilePath 接口设置保存配置文件的路径，需要配置该参数） |
| userTag | NSString | 否 | 用户标签，由业务方指定，可通过调用服务端 CreateUserTag 接口创建，参考 [创建用户属性]() |
| keyboardEnable | BOOL | 否 | 是否允许用户使用键盘进行信息输入，默认：YES |
| extraDict | NSDictionary | 否 | 根据业务需要，自定义的扩展参数；详细信息，参考 [自定义扩展参数列表]() |

参考示例：

```objectivec
// 配置选项
@interface VeGameConfigObject : NSObject

/**--------------------------必选参数----------------------------------*/
/** 鉴权参数 */
@property (nonatomic, copy) NSString *ak;
/** 鉴权参数 */
@property (nonatomic, copy) NSString *sk;
/** 鉴权参数 */
@property (nonatomic, copy) NSString *token;
/** 自定义客户端用户 ID */
@property (nonatomic, copy) NSString *userId;
/** 游戏 ID（当传入 customGameId 时，可不传入，以 gameId 优先）*/
@property (nonatomic, copy) NSString *gameId;
/** 用户自定义游戏 ID（当传入 gameId 时，可不传入）*/
@property (nonatomic, copy) NSString *customGameId;
/** 游戏标识，非空字符串 */
@property (nonatomic, copy) NSString *roundId;

/**--------------------------可选参数----------------------------------*/
/** 火山侧套餐 ID */
@property (nonatomic, copy) NSString *planId;
/** 资源预锁定 ID */
@property (nonatomic, copy) NSString *reservedId;
/** 支持游戏控制权转移 */
@property (nonatomic, strong) VeGameControlObject *control;
/** 清晰度 ID，不传使用默认档位 */
@property (nonatomic, assign) NSInteger videoStreamProfileId;
/** 无操作自动回收服务时长 */
@property (nonatomic, assign) NSInteger autoRecycleTime;
/** 保存用户游戏配置文件的路径列表 */
@property (nonatomic, strong) NSArray<NSString *> *userProfilePathList;
/** 用户标签，业务方指定 */
@property (nonatomic, copy) NSString *userTag;
/** 键盘开关，默认：YES */
@property (nonatomic, assign) BOOL keyboardEnable;
/** 根据业务需要，自定义附加参数 */
@property (nonatomic, strong) NSDictionary *extraDict;

@end
```

#### 游戏控制权转移参数

`VeGameControlObject` 配置参数如下：

|  **参数**  |  **类型**  |  **是否必填**  |  **描述**  |
| --- | --- | --- | --- |
| role | VeBaseRoleType | 是 | 当前用户进入游戏的默认角色，可选项：  <br>0：观看者，默认  <br>1：操作者  <br>说明：指定 `roomType` 和该参数后，可在游戏中调用 [changeRole]() 接口将观看者设置为操作者，转移游戏控制权。进入游戏的观看者也会占用实例资源，建议控制观看者数量。 |
| roomType | VeBaseRoomType | 是 | 游戏房间类型，可选项：  <br>0：单人控制单人观看，默认  <br>1：单人控制多人观看，不可转移游戏控制权  <br>2：单人控制多人观看，可转移游戏控制权 |

```objectivec
@interface VeGameControlObject : NSObject
/**
 * 用户进入游戏的角色
 * 默认：VeBaseRoleTypePlayer
 */
@property (nonatomic, assign) VeBaseRoleType role;
/**
 * 游戏房间类型
 * 默认：VeGameRoomTypeDefault
 */
@property (nonatomic, assign) VeBaseRoomType roomType;

@end
```

#### 错误码

初始化和启动游戏可能返回的错误码和相关说明如下：

|  **错误码**  |  **错误信息**  |  **说明**  |
| --- | --- | --- |
| 10000 | ERROR_START_GENERAL | 开始游戏失败。原因：通用错误。建议：请检查开始游戏 start() 接口参数。 |
| 10001 | ERROR_START_AUTHENTICATION_FAILED | 开始游戏失败。原因：火山引擎服务鉴权失败。建议：检查您的 AK、SK、Token 生成，参考 生成临时 Token 接口。 |
| 10002 | ERROR_START_GAME_ID_NOT_EXIST | 开始游戏失败。原因：当前游戏 ID 或 自定义游戏 ID 不存在。建议：请通过火山引擎云游戏控制台『游戏管理』页面获取正确的游戏 ID。 |
| 10003 | ERROR_START_GAME_ID_NOT_READY | 开始游戏失败。原因：当前游戏尚在适配中。建议：游戏适配需要一定时间，如需加急，可联系火山引擎云游戏服务对接人员。 |
| 10004 | ERROR_START_CONFIGURATION_CODE_NOT_EXIST | 开始游戏失败。原因：资源套餐 ID 不存在。建议：可通过调用服务端 ListResourceSet 接口获取（configuration_code 字段）获取正确的套餐信息。 |
| 10005 | ERROR_START_CONFIGURATION_CODE_NOT_REDAY | 开始游戏失败。原因：游戏尚未配置资源套餐或套餐 ID 无效。建议：请通过火山引擎云游戏控制台『游戏管理』页面为游戏配置部署策略。 |
| 10006 | ERROR_START_RESOURCE_NOT_READY | 开始游戏失败。原因：当前业务没有订购资源。建议：请通过火山引擎云游戏控制台『资源管理』页面购买资源。 |
| 10007 | ERROR_START_RESOURCE_CAPACITY_NOT_ENOUGH | 开始游戏失败。原因：当前游戏资源用量超限或无可用资源。建议：请通过火山引擎云游戏控制台『资源管理』页面购买资源。 |
| 10009 | ERROR_START_AUTHENTICATION_KEY_FAILED | 开始游戏失败。原因：火山引擎服务鉴权失败。建议：请检查临时鉴权 Token 的有效期，参考 生成临时 Token 接口。 |
| 10011 | ERROR_START_CONNECTION_ENDED | 开始游戏失败。原因：在调用 start() 接口启动游戏、启动成功回调触发之前，游戏被停止（可能原因包括：服务端游戏崩溃导致开始游戏失败、调用了服务端 GameStop 接口停止游戏服务）。 |
| 10012 | ERROR_START_RESERVED_ID_NOT_FOUND | 开始游戏失败。原因：资源预锁定 ID（reservedId）不存在。建议：请确保指定的 reservedId 正确；可通过调用服务端 PreAllocateResource 接口获取。 |
| 10013 | ERROR_START_RESERVED_ID_EXPIRED | 开始游戏失败。原因：资源预锁定 ID（reservedId）已失效。建议：可通过调用服务端 PreAllocateResource 接口重新获取。 |
| 10014 | ERROR_START_RESERVED_ID_ALREADY_USED_RELEASE | 开始游戏失败。原因：资源预锁定 ID（reservedId）已被使用且已释放。建议：可通过调用服务端 PreAllocateResource 接口重新获取。 |
| 10015 | ERROR_START_RESERVED_ID_USING | 开始游戏失败。原因：资源预锁定 ID（reservedId）正在被使用中。建议：可通过调用服务端 PreAllocateResource 接口重新获取。 |
| 10016 | ERROR_START_RESERVED_ID_MISMATCH_PREPARE | 开始游戏失败。原因：资源预锁定 ID（reservedId）与调用 PreAllocateResource 接口时指定的参数不一致。建议：请检查资源预锁定 ID（reservedId）与调用 PreAllocateResource 接口时指定的参数是否一致。 |
| 10017 | ERROR_START_NO_SUFFICIENT_FUND | 开始游戏失败。原因：启动游戏时，后付费账户欠费，导致服务不可用。建议：请通过火山引擎『费用中心』充值付费，充值到账后服务自动开启。 |
| 10018 | ERROR_START_USER_CONFLICT | 开始游戏失败。原因：用户使用多个设备申请游戏服务时，触发了游戏多开限制。建议：需要客户端提示用户不要进行多开操作。 |
| 10026 | ERROR_START_MISMATCH_ACCOUNTID | 开始游戏失败。原因：指定的火山引擎账号校验失败。建议：通过火山引擎官网页面右上角 用户 > 账号管理 > 主账号信息 获取正确的账号。 |
| 10027 | ERROR_START_INVALID_LOCAL_TIME | 开始游戏失败。原因：用户手机时间和服务端时间相差超过7天，导致鉴权 Token 过期。建议：需要客户端提示用户把手机时间修正为标准时间。 |


## 接口说明

### 设置配置信息

描述：设置配置信息（建议在程序启动的时候就调用，时机越早越好）。


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
