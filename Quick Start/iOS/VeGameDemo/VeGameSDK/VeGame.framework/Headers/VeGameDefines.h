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

typedef NS_ENUM(NSInteger, VeGameWarningCode) {
    /** 设置无操作回收服务时长非法 */
    WARNING_START_INVALID_AUTO_RECYCLE_TIME = 10019,
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

typedef NS_ENUM(NSInteger, VeGameErrorCode) {
    /** 通用错误 */
    ERROR_START_GENERAL = 10000,
    /** 火山服务鉴权失败 */
    ERROR_START_AUTHENTICATION_FAILED = 10001,
    /** 当前游戏ID不存在 */
    ERROR_START_GAME_ID_OR_CUSTOM_GAME_ID_NOT_EXIST = 10002,
    /** 当前游戏尚在适配中 */
    ERROR_START_GAME_ID_NOT_READY = 10003,
    /** 套餐 ID 不存在 */
    ERROR_START_CONFIGURATION_CODE_NOT_EXIST = 10004,
    /** 套餐 ID 未就绪 */
    ERROR_START_CONFIGURATION_CODE_NOT_REDAY = 10005,
    /** 当前游戏订购资源未就绪 */
    ERROR_START_RESOURCE_NOT_READY = 10006,
    /** 当前游戏没有订购资源 */
    ERROR_START_RESOURCE_CAPACITY_NOT_ENOUGH = 10007,
    /** 火山服务鉴权 Token 过期 */
    ERROR_START_AUTHENTICATION_KEY_FAILED = 10009,
    /** 启动失败，原因：连续调用了两次 Start 之间没有调用Stop */
    ERROR_START_NO_STOP_BEFORE = 10010,
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
    /** 游戏串流错误 */
    ERROR_STREAM_GENERAL = 20000,
    /** 切换清晰度失败，原因：在非播放状态 */
    ERROR_STREAM_CHANGE_CLARITY_ID_NOT_IN_START_STATE = 20002,
    /** SDK 通用错误 */
    ERROR_SDK_GENERAL = 30000,
    /** 初始化 SDK 实例化失败 */
    ERROR_SDK_INIT_FAILED = 30001,
    /** 配置参数 or 鉴权参数 为空*/
    ERROR_SDK_CONFIG_OR_AUTH_PARAMETER_EMPTY = 30002,
    /** SDK 获取本地定位服务失败 */
    ERROR_SDK_LACK_OF_LOCATION_PERMISSION = 30007,
    /** 画布尺寸无效 */
    ERROR_SDK_INVALID_VIDEO_CONTAINER = 30008,
    /** 游戏停止，原因，云端服务异常退出 */
    ERROR_GAME_ABNORMAL_EXIT = 40000,
    /** 服务端游戏奔溃 */
    ERROR_GAME_CRASH = 40001,
    /** 当前游戏被下架 */
    ERROR_GAME_OFFLINE = 40002,
    /** TBD */
    ERROR_GAME_EXIT_TIMEOUT = 40003,
    /** 5分钟未操作，服务端自动断开游戏连接 */
    ERROR_GAME_STOPPED_IDLE = 40004,
    /** 服务端主动重置了游戏 */
    ERROR_GAME_STOPPED_RESET = 40005,
    /** 服务端主动停止了游戏 */
    ERROR_GAME_STOPPED_API = 40006,
    /** 游戏停止，原因：在不同设备上，前后使用了相同的UserId、GameID，前一个Start请求会返回此错误码 */
    ERROR_GAME_STOPPED_DUPLICATE_START = 40007,
    /** ReservedId 过期 */
    MESSAGE_START_RESERVED_ID_EXPIRED = 40011,
    /** ReservedId 没进行 PodStart */
    MESSAGE_RESERVED_ID_NOT_APPLIED = 40012,
    /** ReservedId 对应的实例已退订 */
    MESSAGE_START_RESERVED_ID_RELEASED = 40013,
    /** 实例离线 */
    MESSAGE_RESOURCE_OFFLINE = 40014,
    /** 实例升级 */
    MESSAGE_RESOURCE_UPDATING = 40015,
    /** 实例重启 */
    MESSAGE_RESOURCE_REBOOTING = 40016,
    /** 实例运维 */
    MESSAGE_RESOURCE_MAINTANENCE = 40017,
    /** 实例的云服务升级 */
    MESSAGE_RESOURCE_SERVICE_UPDATING = 40018,
    /** 实例被占用 */
    MESSAGE_RESOURCE_OCCUPIED = 40019,
    /** 实例正在启动游戏 */
    MESSAGE_RESOURCE_LAUNCHING_GAME = 40020,
    /** 实例正在运行 */
    MESSAGE_RESOURCE_IS_RUNNING = 40021,
    /** 实例正在推流 */
    MESSAGE_RESOURCE_IS_PUBLISH = 40022,
    /** 实例正常释放 */
    MESSAGE_RESOURCE_RELEASED_NORMAL = 40023,
    /** 实例异常释放：客户端超时未加入 */
    MESSAGE_RESOURCE_RELEASED_NO_USER = 40024,
    /** 实例异常释放：客户端无操作释放 */
    MESSAGE_RESOURCE_RELEASED_IDLE = 40025,
    /** 实例异常释放：游戏镜像缺失 */
    MESSAGE_RESOURCE_RELEASED_OS_MISSED = 40026,
    /** 实例异常释放：游戏启动失败 */
    MESSAGE_RESOURCE_RELEASED_GAME_START_FAILURE = 40027,
    /** 实例异常释放：rtc推流成功，但是推流过程中出现异常 */
    MESSAGE_RESOURCE_RELEASED_STREAMING_ERROR = 40028,
    /** 实例异常释放：伴随包镜像缺失 */
    MESSAGE_RESOURCE_RELEASED_3RD_APP_MISSED = 40029,
    /** 实例故障 */
    MESSAGE_RESOURCE_ERROR = 40030,
    /** 伴随包启动失败 */
    MESSAGE_3RD_APP_START_FAILURE = 40031,
    /** 游戏频繁崩溃 */
    MESSAGE_CLOUD_GAME_CRASH_OFTEN = 40032,
    /** Rtc推流不成功 */
    MESSAGE_GAME_STEAMING_FAILURE = 40033,
    /** 消息通用错误 */
    ERROR_MESSAGE_GENERAL = 50000,
    /** 发送的消息失败，原因：消息包体过大 */
    ERROR_MESSAGE_OVER_SIZED = 50007,
    /** 消息发送超时时间非法 */
    ERROR_MESSAGE_TIMEOUT_ILLEGAL = 50009,
    /** 网络请求失败 */
    ERROR_NET_REQUEST_ERROR = 60001,
};

// 配置参数
@interface VeGameConfigObject : NSObject
/** 鉴权参数（Required）*/
@property (nonatomic, copy) NSString *ak;
/** 鉴权参数（Required）*/
@property (nonatomic, copy) NSString *sk;
/** 鉴权参数（Required）*/
@property (nonatomic, copy) NSString *token;
/** 用户 ID（Required）*/
@property (nonatomic, copy) NSString *userId;
/** 游戏标识，非空字符串（Required） */
@property (nonatomic, copy) NSString *roundId;
/** 游戏 ID（二选一，GameId 优先级大于 CustomGameId）*/
@property (nonatomic, copy) NSString *gameId;
/** 自定义游戏 ID（二选一，GameId 优先级大于 CustomGameId）*/
@property (nonatomic, copy) NSString *customGameId;
/** 火山侧套餐 ID */
@property (nonatomic, copy) NSString *planId;
/** 用户标签，业务方指定 */
@property (nonatomic, copy) NSString *userTag;
/** 预定 ID */
@property (nonatomic, copy) NSString *reservedId;
/** 根据业务需要，自定义附加参数 */
@property (nonatomic, strong) NSDictionary *extraDict;
/** 挂机时长 */
@property (nonatomic, assign) NSInteger autoRecycleTime;
/** 清晰度 ID，不传使用默认档位 */
@property (nonatomic, assign) NSInteger videoStreamProfileId;
/** 游戏用户的配置路径列表 */
@property (nonatomic, strong) NSArray<NSString *> *userProfilePathList;

@end
