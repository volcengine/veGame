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
| 10002 | ERROR_START_GAME_ID_OR_CUSTOM_GAME_ID_NOT_EXIST | 开始游戏失败。原因：当前游戏 ID 或 自定义游戏 ID 不存在。建议：请通过火山引擎云游戏控制台『游戏管理』页面获取正确的游戏 ID。 |
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

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| accountId | NSString | 火山引擎用户账号，可通过火山引擎官网页面右上角 **用户 > 账号管理 > 主账号信息** 获取 |

```objectivec
+ (void)setupConfigWithAccountId:(NSString *)accountId;
```

### 创建 VeGameManager 单例对象

描述：创建 VeGameManager 单例对象。

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| containerView | UIView | 视图容器 |
| delegate | NSObject | 代理 |

```objectivec
+ (instancetype)sharedManagerWithContainerView:(UIView *)containerView delegate:(NSObject<VeGameManagerDelegate> *)delegate;
```

### 获取 VeGameManager 单例对象

描述：获取  VeGameManager 单例对象。

```objectivec
+ (VeGameManager *)sharedInstance;
```

### 获取 SDK 版本信息

描述：获取 veGameSDK 的当前版本号。

```objectivec
+ (NSString *)currentVersion;
```

### 获取设备 ID

描述：获取 SDK 生成的设备 ID，用于后台查询 SDK 日志，进行调试和问题排查（建议接入方在需要问题排查时，提供给火山引擎云游戏技术支持）。

```objectivec
+ (NSString *)currentDeviceId;
```

### 获取设备唯一标识

描述：在完成 SDK 初始化后，获取用户终端设备的唯一标识。

> 最佳实践：保存获取到的设备唯一标识到业务的服务端。在通过调用服务端 PreAllocateResource 接口锁定资源、请求游戏服务时，透传保存的设备唯一标识，用于确定是否支持游戏多开。

```objectivec
+ (NSString *)serviceDeviceId;
```

### 开始网络探测
描述：在开始游戏之前，对客户端网络质量进行探测，提示用户当前的网络状态。

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| configObj  | VeGameConfigObject | 启动游戏配置参数，只需指定 ak、sk、token、userId 参数（与启动游戏接口使用的配置参数一致） |

```objectivec
- (void)probeStart:(VeGameConfigObject *)configObj;
```

### 停止网络探测

描述：在网络探测过程中，停止网络探测。

```objectivec
- (void)probeInterrupt;
```

#### 网络探测统计数据说明

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| rtt | Int | 往返时延时长；单位：毫秒 |
| uploadBandwidth | Int | 上行网络带宽；单位：Kbit/s |
| downloadBandwidth | Int | 下行网络带宽；单位：Kbit/s |
| uploadJitter | Int | 上行网络抖动时长；单位：毫秒 |
| downloadJitter | Int | 下行网络抖动时长；单位：毫秒 |
| uploadLossPercent | Float | 上行网络丢包率；单位：百分比 |
| downloadLossPercent | Float | 下行网络丢包率；单位：百分比 |

```objectivec
@interface VeGameNetworkProbeStats : NSObject
/** 往返时延, 单位 ms */
@property (nonatomic, assign) int rtt;
/** 上行网络带宽, 单位 kbit/s */
@property (nonatomic, assign) int uploadBandwidth;
/** 下行网络带宽, 单位 kbit/s */
@property (nonatomic, assign) int downloadBandwidth;
/** 上行网络 jitter, 单位 ms */
@property (nonatomic, assign) int uploadJitter;
/** 下行网络 jitter, 单位 ms */
@property (nonatomic, assign) int downloadJitter;
/** 上行网络丢包率, 单位 % */
@property (nonatomic, assign) float uploadLossPercent;
/** 下行网络丢包率, 单位 % */
@property (nonatomic, assign) float downloadLossPercent;

@end
```

### 启动游戏

描述：启动游戏接口。

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| configObj | VeGameConfigObject | 启动游戏配置参数，可参考 [配置参数说明](#配置参数) |

```objectivec
- (void)startWithConfig:(VeGameConfigObject *)configObj;
```

### 重启游戏

描述：重启当前游戏。

```objectivec
- (void)restartGame
```

### 停止游戏

描述：停止从云端拉流，并且退出，但不会回收客户端本地资源。

```objectivec
- (void)stop;
```

### 设置保活时间

描述：在游戏中动态设置客户端切后台之后，云端游戏服务的保活时间（如不设置，则使用默认保活时长300秒；支持设置的上限值为7200，即2小时；如需设置更长时间，可联系火山引擎云游戏技术支持）。

> 当客户端切后台时开始计时。如用户在设置的保活时间到达时未切回前台，服务端将断开游戏连接。

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| time | NSInteger | 保活时间，单位秒 |

```objectivec
- (void)setIdleTime:(NSInteger)time;
```

### 设置无操作回收服务时间

描述：在游戏中动态设置无操作回收服务时间（如不设置或设置为0，则使用默认时长300秒；支持设置的上限值为7200，即2小时；如需设置更长时间，可联系火山引擎云游戏技术支持）。

> - 如果已在 VeGameConfigObject 配置参数中通过 autoRecycleTime 参数设置，动态设置的时长优先级高。
> - 调用此接口，会触发 gameManager:setAutoRecycleTimeCallback:time: 回调。

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| time | NSInteger | 无操作回收服务时间，单位秒 |

```objectivec
- (void)setAutoRecycleTime:(NSInteger)time;
```

### 获取无操作回收服务时间

描述：获取已设置的无操作回收服务时间。

> 调用此接口，会触发 gameManager:getAutoRecycleTimeCallback:time: 回调。

```objectivec
- (void)getAutoRecycleTime;
```

### 设置保存用户配置文件的路径

描述：设置保存游戏云端配置文件的路径，当用户正常退出或异常退出（切前后台保活到期、踢下线、游戏崩溃等），按照配置的路径保存用户的配置文件。当用户再次进入游戏时，拉取和还原已保存的用户配置信息。

> 调用此接口，会触发 gameManager:setUserProfilePathType:result: 回调。

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| pathList | NSArray<NSString> | 保存配置文件的路径列表 |

```objectivec
- (void)setUserProfilePathList:(NSArray<NSString *> *)pathList;
```

### 获取保存用户配置文件的路径

描述：当用户再次进入游戏时，获取保存用户配置文件的路径，拉取和还原已保存的用户配置信息。

> 调用此接口，会触发 gameManager:getUserProfilePathList: 回调。

```objectivec
- (void)getUserProfilePathList;
```

### 设置游戏玩家角色

描述：在游戏中，设置游戏玩家的角色。

>调用此接口，会触发 gameManeger:onChangeRoleCallBack:result:destUserId: 回调。

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| destUid | NSString | 游戏玩家用户 ID |
| role | VeBaseRoleType | 游戏玩家角色（0：观看者；1：操作者） |

```objectivec
- (void)changeRole:(NSString *)destUid role:(VeBaseRoleType)role;
```

### 获取当前玩家的角色

描述：获取当前游戏玩家的角色。

```objectivec
- (VeGameRoleType)getCurrentRole;
```

### 清晰度切换

描述：在游戏运行过程中，可通过指定清晰度 ID，切换视频流的清晰度。

> 调用此接口，会触发 gameManager:switchVideoStreamProfileWithCode:fromIndex:toIndex: 回调。

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| streamProfileId | NSInteger | 清晰度档位 ID（清晰度档位 ID 列表和详细说明，参考 [云游戏清晰度档位说明]()） |

```objectivec
- (void)switchVideoStreamProfile:(NSInteger)streamProfileId;
```

### 客户端前后台切换

描述：设置客户端应用或游戏切换前后台的状态。

> 调用此接口，会触发 gameManager:onBackgroundSwitched: 回调。

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| on | BOOL | 客户端切换前后台：  <br>YES（切后台）  <br>NO（切前台） |

```objectivec
- (void)switchBackground:(BOOL)on;
```

### 云端游戏切换前台

描述：将云端游戏切换到前台。

> 调用此接口，会触发 gameManager:onRemoteAppSwitchedForeground:switchType: 和 gameManager:onRemoteAppSwitchedBackground:switchType: 回调。

```objectivec
- (void)setRemoteGameForeground;
```

### 游戏手柄

描述：设置自定义游戏手柄操作视图。

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| gamepadView | UIView | 游戏手柄操作视图 |

```objectivec
- (void)setGamepadView:(UIView *)gamepadView;
```

### 设置键盘开关

描述：设置是否允许用户使用键盘进行信息输入的能力。

> 调用此接口，会触发 gameManeger:keyboardEnable: 回调。

|  **参数**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| on | BOOL | 是否允许用户启用键盘进行信息输入：  <br>YES（允许，默认）  <br>NO（禁止） |

```objectivec
- (void)setKeyboardEnable:(BOOL)on;
```

### 获取键盘开关状态

描述：获取键盘开关状态（即允许/禁止用户使用键盘输入信息）。

```objectivec
- (BOOL)getKeyboardEnable;
```
