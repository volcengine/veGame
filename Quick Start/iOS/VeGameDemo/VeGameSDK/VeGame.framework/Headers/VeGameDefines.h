//
//  VeGameDefines.h
//  VePlayer
//
//  Created by changwuguo on 2022/7/12.
//  Copyright © 2022 ByteDance Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef NS_ENUM(NSUInteger, VeGamepadOperationType) {
    /** 按键 */
    VeGamepadOperationTypeKey = 1,
    /** 操作杆 */
    VeGamepadOperationTypeJoystick = 2,
};

typedef NS_ENUM(NSUInteger, VeGameRoleType) {
    /** 观看者 */
    VeGameRoleTypeViewer = 0,
    /** 操作者 */
    VeGameRoleTypePlayer = 1,
};

typedef NS_ENUM(NSUInteger, VeGameWarningCode) {
    /** 启动失败，原因：连续调用了两次 Start 接口之间没有调用 Stop 接口 */
    WARNING_START_NO_STOP_BEFORE = 10010,
    /** 设置挂机服务时长非法 */
    WARNING_START_INVALID_AUTO_RECYCLE_TIME = 10019,
    /** 伴随程序：全部未找到 */
    WARNING_START_WITH_FRAMEWORK_NOT_FOUND = 10023,
    /** 伴随程序：部分找到 */
    WARNING_START_WITH_FRAMEWORK_PART_MATCH = 10024,
    /** 伴随程序：格式错误，解析失败 */
    WARNING_START_WITH_FRAMEWORK_WRONG_INPUT_FORMAT = 10025,
    /** SDK 获取本地定位服务失败 */
    WARNING_SDK_LACK_OF_LOCATION_PERMISSION = 30007,
    /** 用户重复调用切换后台接口 */
    WARNING_LOCAL_ALREADY_SET_BACKGROUND = 40037,
    /** 用户重复调用切换前台接口 */
    WARNING_LOCAL_ALREADY_SET_FOREGROUND = 40038,
    /** 切换前后台错误，PackageName为空 */
    WARNING_FOREGROUND_CHANGE_STATUS_NO_PACKAGENAME = 40040,
    /** 切换前后台错误，重复切换 */
    WARNING_FOREGROUND_CHANGE_STATUS_REPEAT = 40041,
    /** 切换前后台错误，PackageName没找到 */
    WARNING_FOREGROUND_CHANGE_STATUS_NOTFOND = 40043,
    /** 游戏停止，一般指云端运行的游戏主动退出了 */
    WARNING_GAME_STOPPED_INGAME_EXIT = 40044,
};

typedef NS_ENUM(NSUInteger, VeGameErrorCode) {
    /** 启动通用错误 */
    ERROR_START_GENERAL = 10000,
    /** 火山服务鉴权失败 */
    ERROR_START_AUTHENTICATION_FAILED = 10001,
    /** 当前游戏ID不存在 */
    ERROR_START_GAME_ID_OR_CUSTOM_GAME_ID_NOT_EXIST = 10002,
    /** 当前游戏尚在适配中 */
    ERROR_START_GAME_ID_NOT_READY = 10003,
    /** 套餐不存在 */
    ERROR_START_CONFIGURATION_CODE_NOT_EXIST = 10004,
    /** 套餐未就绪 */
    ERROR_START_CONFIGURATION_CODE_NOT_REDAY = 10005,
    /** 当前游戏订购资源未就绪 */
    ERROR_START_RESOURCE_NOT_READY = 10006,
    /** 当前游戏没有订购资源 */
    ERROR_START_RESOURCE_CAPACITY_NOT_ENOUGH = 10007,
    /** 火山服务鉴权 Token 过期 */
    ERROR_START_AUTHENTICATION_KEY_FAILED = 10009,
    /** 启动失败，原因：在调用 Start 接口后，成功回调触发前，游戏被停止 */
    ERROR_START_CONNECTION_ENDED = 10011,
    /** ReservedId 不存在 */
    ERROR_START_RESERVED_ID_NOT_FOUND = 10012,
    /** ReservedId 过期 */
    ERROR_START_RESERVED_ID_EXPIRED = 10013,
    /** ReservedId 无效 */
    ERROR_START_RESERVED_ID_ALREADY_USED_RELEASE = 10014,
    /** ReservedId 已被使用 */
    ERROR_START_RESERVED_ID_USING = 10015,
    /** ReservedId 相应的预锁定与 Start 参数不匹配  */
    ERROR_START_RESERVED_ID_MISMATCH_PREPARE = 10016,
    /** 后付费账户欠费，服务不可用 */
    ERROR_START_NO_SUFFICIENT_FUND = 10017,
    /** 启动失败，原因：触发了游戏多开限制 */
    ERROR_START_USER_CONFLICT = 10018,
    /** 火山账号不匹配 */
    ERROR_START_MISMATCH_ACCOUNTID = 10026,
    /** 本地时间导致token过期 */
    ERROR_START_INVALID_LOCAL_TIME = 10027,
    /** 游戏串流错误 */
    ERROR_STREAM_GENERAL = 20000,
    /** 切换清晰度失败，原因：在非播放状态 */
    ERROR_STREAM_CHANGE_CLARITY_ID_NOT_IN_START_STATE = 20002,
    /** SDK 通用错误 */
    ERROR_SDK_GENERAL = 30000,
    /** 初始化 SDK 实例化失败 */
    ERROR_SDK_INIT_FAILED = 30001,
    /** 配置参数/鉴权参数为空*/
    ERROR_SDK_CONFIG_OR_AUTH_PARAMETER_EMPTY = 30002,
    /** 画布尺寸无效 */
    ERROR_SDK_INVALID_VIDEO_CONTAINER = 30008,
    /** 火山账户ID非法 */
    ERROR_INIT_ACCOUNT_ID_ILLEGAL = 30009,
    /** 游戏停止，原因，云端服务异常退出 */
    ERROR_GAME_ABNORMAL_EXIT = 40000,
    /** 服务端游戏奔溃 */
    ERROR_GAME_CRASH = 40001,
    /** 当前游戏被下架 */
    ERROR_GAME_OFFLINE = 40002,
    /** TBD */
    ERROR_GAME_EXIT_TIMEOUT = 40003,
    /** 长期未操作，服务端自动断开游戏连接 */
    ERROR_GAME_STOPPED_IDLE = 40004,
    /** 服务端主动重置了游戏 */
    ERROR_GAME_STOPPED_RESET = 40005,
    /** 服务端主动停止了游戏 */
    ERROR_GAME_STOPPED_API = 40006,
    /** 游戏停止，在不同设备上，前后使用了相同的UserId、GameId请求Start */
    ERROR_GAME_STOPPED_DUPLICATE_START = 40007,
    /** 后台超时 */
    ERROR_POD_STOPPED_BACKGROUND_TIMEOUT = 40008,
    /** 实例正常释放 */
    MESSAGE_RESOURCE_RELEASED_NORMAL = 40023,
    /** 实例异常释放：客户端超时未加入 */
    MESSAGE_RESOURCE_RELEASED_NO_USER = 40024,
    /** 实例异常释放：游戏镜像缺失 */
    MESSAGE_RESOURCE_RELEASED_OS_MISSED = 40026,
    /** 实例异常释放：游戏启动失败 */
    MESSAGE_RESOURCE_RELEASED_GAME_START_FAILURE = 40027,
    /** 实例异常释放：rtc推流成功，但是推流过程中出现异常 */
    MESSAGE_RESOURCE_RELEASED_STREAMING_ERROR = 40028,
    /** 实例异常释放：伴随包镜像缺失 */
    MESSAGE_RESOURCE_RELEASED_3RD_APP_MISSED = 40029,
    /** 伴随包启动失败 */
    MESSAGE_3RD_APP_START_FAILURE = 40031,
    /** 游戏频繁崩溃 */
    MESSAGE_CLOUD_GAME_CRASH_OFTEN = 40032,
    /** RTC推流不成功 */
    MESSAGE_GAME_STEAMING_FAILURE = 40033,
    /** Pod收到的参数非法 */
    MESSAGE_RESOURCE_RELEASED_INVALID_PARAMETER = 40047,
    /** 实例离线 */
    MESSAGE_RESOURCE_RELEASED_HEART_BEAT_TIMEOUT = 40048,
    /** 游戏侧主动退出 */
    MESSAGE_RESOURCE_RELEASED_INGAME_EXIT = 40049,
    /** 消息通道通用错误 */
    ERROR_MESSAGE_GENERAL = 50000,
    /** 消息通道无连接 */
    ERROR_MESSAGE_NOT_CONNECTED = 50001,
    /** 消息通道数据解析失败 */
    ERROR_MESSAGE_FAILED_TO_PARSE_MSG = 50002,
    /** 消息通道ID非法 */
    ERROR_MESSAGE_CHANNEL_UID_ILLEGAL = 50003,
    /** 发送的消息失败，原因：消息超过60kb */
    ERROR_MESSAGE_OVER_SIZED = 50007,
    /** 消息发送超时时间非法 */
    ERROR_MESSAGE_TIMEOUT_ILLEGAL = 50009,
    /** 网络请求失败 */
    ERROR_NET_REQUEST_ERROR = 60001,
    /** 网络请求失败 */
    ERROR_HTTP_REQUEST_ERROR = 60002,
    /** 上传或下载的文件不存在 */
    ERROR_FILE_CHANNEL_FILE_NOT_FOUND = 71001,
    /** 上传或下载的文件无权限访问 */
    ERROR_FILE_CHANNEL_FILE_NO_ACCESS = 71002,
    /** 上传或下载文件过程中网络异常中断 */
    ERROR_FILE_CHANNEL_NETWORK_ABORT = 71003,
    /** 上传或下载文件过程中超时 */
    ERROR_FILE_CHANNEL_TIMEOUT = 71004,
    /** 上传或下载文件磁盘空间不足 */
    ERROR_FILE_CHANNEL_STORAGE_NOT_ENOUGH = 71005,
};

// 网络探测数据统计
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

// 配置参数
@interface VeGameConfigObject : NSObject
/**--------必选参数--------*/
/** 鉴权参数 */
@property (nonatomic, copy) NSString *ak;
/** 鉴权参数 */
@property (nonatomic, copy) NSString *sk;
/** 鉴权参数 */
@property (nonatomic, copy) NSString *token;
/** 用户 ID */
@property (nonatomic, copy) NSString *userId;
/** 游戏标识，非空字符串（Required） */
@property (nonatomic, copy) NSString *roundId;
/** 游戏 ID（GameId 优先级大于 CustomGameId）*/
@property (nonatomic, copy) NSString *gameId;
/** 自定义游戏 ID */
@property (nonatomic, copy) NSString *customGameId;
/**--------可选参数--------*/
/** 火山侧套餐 ID */
@property (nonatomic, copy) NSString *planId;
/** 用户标签，业务方指定 */
@property (nonatomic, copy) NSString *userTag;
/** 预定 ID */
@property (nonatomic, copy) NSString *reservedId;
/** 角色（PC云游戏有效） */
@property (nonatomic, assign) VeGameRoleType role;
/** 0：单人控制，单人读（默认）；1：单人控制多人读, 控制权不可转移；2：单人控制多人读, 控制权可转移。（PC云游戏有效）*/
@property (nonatomic, assign) NSUInteger roomType;
/** 根据业务需要，自定义附加参数 */
@property (nonatomic, strong) NSDictionary *extraDict;
/** 挂机时长 */
@property (nonatomic, assign) NSInteger autoRecycleTime;
/** 清晰度 ID，不传使用默认档位 */
@property (nonatomic, assign) NSInteger videoStreamProfileId;
/** 游戏用户的配置路径列表 */
@property (nonatomic, strong) NSArray<NSString *> *userProfilePathList;
/** 键盘开关，默认：YES */
@property (nonatomic, assign) BOOL keyboardEnable;

@end
