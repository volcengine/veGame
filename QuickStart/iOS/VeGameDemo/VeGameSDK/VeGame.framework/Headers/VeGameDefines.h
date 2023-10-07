//
//  VeGameDefines.h
//  VePlayer
//
//  Created by changwuguo on 2022/7/12.
//  Copyright © 2022 ByteDance Ltd. All rights reserved.
//

#import "VeBaseDefines.h"

/** ---------------- 键盘按钮类型 ----------------- */
typedef NS_ENUM(NSUInteger, VeGameKeyboardKeyType) {
    VeGameKeyboardKeyNone = 0,
    VeGameKeyboardKeyBack = 8,
    VeGameKeyboardKeyTab = 9,
    VeGameKeyboardKeyClear = 12,
    VeGameKeyboardKeyReturn = 13,
    VeGameKeyboardKeyShift = 16,
    VeGameKeyboardKeyControl = 17,
    VeGameKeyboardKeyMenu = 18,
    VeGameKeyboardKeyPause = 19,
    VeGameKeyboardKeyCapital = 20,
    VeGameKeyboardKeyKana = 21,
    VeGameKeyboardKeyImeOn = 22,
    VeGameKeyboardKeyJunja = 23,
    VeGameKeyboardKeyFinal = 24,
    VeGameKeyboardKeyHanja = 25,
    VeGameKeyboardKeyImeOff = 26,
    VeGameKeyboardKeyEscape = 27,
    VeGameKeyboardKeyConvert = 28,
    VeGameKeyboardKeyNonConvert = 29,
    VeGameKeyboardKeyAccept = 30,
    VeGameKeyboardKeyModeChange = 31,
    VeGameKeyboardKeySpace = 32,
    VeGameKeyboardKeyPrior = 33,
    VeGameKeyboardKeyNext = 34,
    VeGameKeyboardKeyEnd = 35,
    VeGameKeyboardKeyHome = 36,
    VeGameKeyboardKeyLeft = 37,
    VeGameKeyboardKeyUp = 38,
    VeGameKeyboardKeyRight = 39,
    VeGameKeyboardKeyDown = 40,
    VeGameKeyboardKeySelect = 41,
    VeGameKeyboardKeyPrint = 42,
    VeGameKeyboardKeyExecute = 43,
    VeGameKeyboardKeySnapshot = 44,
    VeGameKeyboardKeyInsert = 45,
    VeGameKeyboardKeyDelete = 46,
    VeGameKeyboardKeyHelp = 47,
    VeGameKeyboardKey0 = 48,
    VeGameKeyboardKey1 = 49,
    VeGameKeyboardKey2 = 50,
    VeGameKeyboardKey3 = 51,
    VeGameKeyboardKey4 = 52,
    VeGameKeyboardKey5 = 53,
    VeGameKeyboardKey6 = 54,
    VeGameKeyboardKey7 = 55,
    VeGameKeyboardKey8 = 56,
    VeGameKeyboardKey9 = 57,
    VeGameKeyboardKeyA = 65,
    VeGameKeyboardKeyB = 66,
    VeGameKeyboardKeyC = 67,
    VeGameKeyboardKeyD = 68,
    VeGameKeyboardKeyE = 69,
    VeGameKeyboardKeyF = 70,
    VeGameKeyboardKeyG = 71,
    VeGameKeyboardKeyH = 72,
    VeGameKeyboardKeyI = 73,
    VeGameKeyboardKeyJ = 74,
    VeGameKeyboardKeyK = 75,
    VeGameKeyboardKeyL = 76,
    VeGameKeyboardKeyM = 77,
    VeGameKeyboardKeyN = 78,
    VeGameKeyboardKeyO = 79,
    VeGameKeyboardKeyP = 80,
    VeGameKeyboardKeyQ = 81,
    VeGameKeyboardKeyR = 82,
    VeGameKeyboardKeyS = 83,
    VeGameKeyboardKeyT = 84,
    VeGameKeyboardKeyU = 85,
    VeGameKeyboardKeyV = 86,
    VeGameKeyboardKeyW = 87,
    VeGameKeyboardKeyX = 88,
    VeGameKeyboardKeyY = 89,
    VeGameKeyboardKeyZ = 90,
    VeGameKeyboardKeyLwin = 91,
    VeGameKeyboardKeyRwin = 92,
    VeGameKeyboardKeyApps = 93,
    VeGameKeyboardKeySleep = 95,
    VeGameKeyboardKeyNumpad0 = 96,
    VeGameKeyboardKeyNumpad1 = 97,
    VeGameKeyboardKeyNumpad2 = 98,
    VeGameKeyboardKeyNumpad3 = 99,
    VeGameKeyboardKeyNumpad4 = 100,
    VeGameKeyboardKeyNumpad5 = 101,
    VeGameKeyboardKeyNumpad6 = 102,
    VeGameKeyboardKeyNumpad7 = 103,
    VeGameKeyboardKeyNumpad8 = 104,
    VeGameKeyboardKeyNumpad9 = 105,
    VeGameKeyboardKeyMultiply = 106,
    VeGameKeyboardKeyAdd = 107,
    VeGameKeyboardKeySeparator = 108,
    VeGameKeyboardKeySubtract = 109,
    VeGameKeyboardKeyDecimal = 110,
    VeGameKeyboardKeyDivide = 111,
    VeGameKeyboardKeyF1 = 112,
    VeGameKeyboardKeyF2 = 113,
    VeGameKeyboardKeyF3 = 114,
    VeGameKeyboardKeyF4 = 115,
    VeGameKeyboardKeyF5 = 116,
    VeGameKeyboardKeyF6 = 117,
    VeGameKeyboardKeyF7 = 118,
    VeGameKeyboardKeyF8 = 119,
    VeGameKeyboardKeyF9 = 120,
    VeGameKeyboardKeyF10 = 121,
    VeGameKeyboardKeyF11 = 122,
    VeGameKeyboardKeyF12 = 123,
    VeGameKeyboardKeyF13 = 124,
    VeGameKeyboardKeyF14 = 125,
    VeGameKeyboardKeyF15 = 126,
    VeGameKeyboardKeyF16 = 127,
    VeGameKeyboardKeyF17 = 128,
    VeGameKeyboardKeyF18 = 129,
    VeGameKeyboardKeyF19 = 130,
    VeGameKeyboardKeyF20 = 131,
    VeGameKeyboardKeyF21 = 132,
    VeGameKeyboardKeyF22 = 133,
    VeGameKeyboardKeyF23 = 134,
    VeGameKeyboardKeyF24 = 135,
    VeGameKeyboardKeyNavigationView = 136,
    VeGameKeyboardKeyNavigationMenu = 137,
    VeGameKeyboardKeyNavigationUp = 138,
    VeGameKeyboardKeyNavigationDown = 139,
    VeGameKeyboardKeyNavigationLeft = 140,
    VeGameKeyboardKeyNavigationRight = 141,
    VeGameKeyboardKeyNavigationAccept = 142,
    VeGameKeyboardKeyNavigationCancel = 143,
    VeGameKeyboardKeyNumlock = 144,
    VeGameKeyboardKeyScroll = 145,
    VeGameKeyboardKeyOemNecEqual = 146,
    VeGameKeyboardKeyOemFjMasshou = 147,
    VeGameKeyboardKeyOemFjTouroku = 148,
    VeGameKeyboardKeyOemFjLoya = 149,
    VeGameKeyboardKeyOemFjRoya = 150,
    VeGameKeyboardKeyLshift = 160,
    VeGameKeyboardKeyRshift = 161,
    VeGameKeyboardKeyLcontrol = 162,
    VeGameKeyboardKeyRcontrol = 163,
    VeGameKeyboardKeyLmenu = 164,
    VeGameKeyboardKeyRmenu = 165,
    VeGameKeyboardKeyBrowserBack = 166,
    VeGameKeyboardKeyBrowserForward = 167,
    VeGameKeyboardKeyBrowserRefresh = 168,
    VeGameKeyboardKeyBrowserStop = 169,
    VeGameKeyboardKeyBrowserSearch = 170,
    VeGameKeyboardKeyBrowserFavorites = 171,
    VeGameKeyboardKeyBrowserHome = 172,
    VeGameKeyboardKeyVolumeMute = 173,
    VeGameKeyboardKeyVolumeDown = 174,
    VeGameKeyboardKeyVolumeUp = 175,
    VeGameKeyboardKeyMediaNextTrack = 176,
    VeGameKeyboardKeyMediaPrevTrack = 177,
    VeGameKeyboardKeyMediaStop = 178,
    VeGameKeyboardKeyMediaPlayPause = 179,
    VeGameKeyboardKeyLaunchMail = 180,
    VeGameKeyboardKeyLaunchMediaSelect = 181,
    VeGameKeyboardKeyLaunchApp1 = 182,
    VeGameKeyboardKeyLaunchApp2 = 183,
    VeGameKeyboardKeyOem1 = 186,
    VeGameKeyboardKeyOemPlus = 187,
    VeGameKeyboardKeyOemComma = 188,
    VeGameKeyboardKeyOemMinus = 189,
    VeGameKeyboardKeyOemPeriod = 190,
    VeGameKeyboardKeyOem2 = 191,
    VeGameKeyboardKeyOem3 = 192,
    VeGameKeyboardKeyGamepadA = 195,
    VeGameKeyboardKeyGamepadB = 196,
    VeGameKeyboardKeyGamepadX = 197,
    VeGameKeyboardKeyGamepadY = 198,
    VeGameKeyboardKeyGamepadRightShoulder = 199,
    VeGameKeyboardKeyGamepadLeftShoulder = 200,
    VeGameKeyboardKeyGamepadLeftTrigger = 201,
    VeGameKeyboardKeyGamepadRightTrigger = 202,
    VeGameKeyboardKeyGamepadDpadUp = 203,
    VeGameKeyboardKeyGamepadDpadDown = 204,
    VeGameKeyboardKeyGamepadDpadLeft = 205,
    VeGameKeyboardKeyGamepadDpadRight = 206,
    VeGameKeyboardKeyGamepadMenu = 207,
    VeGameKeyboardKeyGamepadView = 208,
    VeGameKeyboardKeyGamepadLeftThumbStickButton = 209,
    VeGameKeyboardKeyGamepadRightThumbStickButton = 210,
    VeGameKeyboardKeyGamepadLeftThumbStickUp = 211,
    VeGameKeyboardKeyGamepadLeftThumbStickDown = 212,
    VeGameKeyboardKeyGamepadLeftThumbStickRight = 213,
    VeGameKeyboardKeyGamepadLeftThumbStickLeft = 214,
    VeGameKeyboardKeyGamepadRightThumbStickUp = 215,
    VeGameKeyboardKeyGamepadRightThumbStickDown = 216,
    VeGameKeyboardKeyGamepadRightThumbStickRight = 217,
    VeGameKeyboardKeyGamepadRightThumbStickLeft = 218,
    VeGameKeyboardKeyOem4 = 219,
    VeGameKeyboardKeyOem5 = 220,
    VeGameKeyboardKeyOem6 = 221,
    VeGameKeyboardKeyOem7 = 222,
    VeGameKeyboardKeyOem8 = 223,
    VeGameKeyboardKeyOemAx = 225,
    VeGameKeyboardKeyOem102 = 226,
    VeGameKeyboardKeyIcoHelp = 227,
    VeGameKeyboardKeyIco00 = 228,
    VeGameKeyboardKeyProcessKey = 229,
    VeGameKeyboardKeyIcoClear = 230,
    VeGameKeyboardKeyPacket = 231,
    VeGameKeyboardKeyOemReset = 233,
    VeGameKeyboardKeyOemJump = 234,
    VeGameKeyboardKeyOemPa1 = 235,
    VeGameKeyboardKeyOemPa2 = 236,
    VeGameKeyboardKeyOemPa3 = 237,
    VeGameKeyboardKeyOemWsCtrl = 238,
    VeGameKeyboardKeyOemCuSel = 239,
    VeGameKeyboardKeyOemAttn = 240,
    VeGameKeyboardKeyOemFinish = 241,
    VeGameKeyboardKeyOemCopy = 242,
    VeGameKeyboardKeyOemAuto = 243,
    VeGameKeyboardKeyOemEnlw = 244,
    VeGameKeyboardKeyOemBackTab = 245,
    VeGameKeyboardKeyAttn = 246,
    VeGameKeyboardKeyCrSel = 247,
    VeGameKeyboardKeyExSel = 248,
    VeGameKeyboardKeyErEof = 249,
    VeGameKeyboardKeyPlay = 250,
    VeGameKeyboardKeyZoom = 251,
    VeGameKeyboardKeyNoName = 252,
    VeGameKeyboardKeyPa1 = 253,
    VeGameKeyboardKeyOemClear = 254,
};

/** ---------------- 鼠标Action类型 --------------- */
typedef NS_ENUM(NSUInteger, VeGameMouseActionType) {
    /** 按下 */
    VeGameMouseActionTypeDown = 0,
    /** 抬起 */
    VeGameMouseActionTypeUp,
    /** 光标位置 */
    VeGameMouseActionTypeCursorPos,
    /** 鼠标移动 */
    VeGameMouseActionTypeMove,
    /** 滚轮 */
    VeGameMouseActionTypeWheel
};

/** ---------------- 鼠标按钮类型 ----------------- */
typedef NS_ENUM(NSUInteger, VeGameMouseButtonType) {
    /** 左键 */
    VeGameMouseButtonTypeLeft = 0,
    /** 中间 */
    VeGameMouseButtonTypeMiddle,
    /** 右键 */
    VeGameMouseButtonTypeRight,
    /** XButton1 */
    VeGameMouseButtonTypeXButton1,
    /** XButton2 */
    VeGameMouseButtonTypeXButton2
};

/** ---------------- 触控Action类型 --------------- */
typedef NS_ENUM(NSUInteger, VeGameTouchActionType) {
    /** 开始 */
    VeGameTouchActionTypeStart = 0,
    /** 移动 */
    VeGameTouchActionTypeMoved,
    /** 结束 */
    VeGameTouchActionTypeEnded
};

/** ----------------- 警告码 ----------------- */
typedef NS_ENUM(NSUInteger, VeGameWarningCode) {
    /** 启动失败，原因：连续调用了两次 Start 接口之间没有调用 Stop 接口 */
    WARNING_START_NO_STOP_BEFORE = 10010,
    /** 设置无操作回收时长非法 */
    WARNING_START_INVALID_AUTO_RECYCLE_TIME = 10019,
    /** 伴随程序：全部未找到 */
    WARNING_START_WITH_FRAMEWORK_NOT_FOUND = 10023,
    /** 伴随程序：部分找到 */
    WARNING_START_WITH_FRAMEWORK_PART_MATCH = 10024,
    /** 伴随程序：格式错误，解析失败 */
    WARNING_START_WITH_FRAMEWORK_WRONG_INPUT_FORMAT = 10025,
    /** 排队完发现还需要继续排队 */
    WARNING_QUEUEING_LACK_RESOURCE = 10030,
    /** SDK 获取本地定位服务失败 */
    WARNING_SDK_LACK_OF_LOCATION_PERMISSION = 30007,
    /** VeBaseRoleTypeViewer 操作被调用 */
    WARNING_VIEWER_METHOD_CALLED = 30011,
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
    /** 游戏实例退出 */
    WARNING_GAME_STOPPED_INGAME_EXIT = 40044,
    /** 套餐不支持 */
    WARNING_VIDEO_PROFILE_NOT_SUPPORT_CURRENT_PLAN = 40052,
    /** 网络请求取消 */
    WARNING_START_NET_REQUEST_CANCEL = 61001,
};

/** ---------------- 错误码 ---------------- */
typedef NS_ENUM(NSUInteger, VeGameErrorCode) {
    /** 启动通用错误 */
    ERROR_START_GENERAL = 10000,
    /** 火山服务鉴权失败 */
    ERROR_START_AUTHENTICATION_FAILED = 10001,
    /** 当前游戏ID不存在 */
    ERROR_START_GAME_ID_OR_CUSTOM_GAME_ID_NOT_EXIST = 10002,
    /** 当前游戏正在适配中 */
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
    /** 观察者启用挂机模式报错 */
    ERROR_START_INVALID_ROLE = 10028,
    /** 退出排队队列 */
    ERROR_USER_NOT_IN_QUEUE = 10029,
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
    /** ContainerView 尺寸无效 */
    ERROR_SDK_INVALID_VIDEO_CONTAINER = 30008,
    /** 火山账户ID为空 */
    ERROR_INIT_ACCOUNT_ID_ILLEGAL = 30009,
    /** 云端游戏异常退出 */
    ERROR_GAME_ABNORMAL_EXIT = 40000,
    /** 云端游戏崩溃 */
    ERROR_GAME_CRASH = 40001,
    /** 长期未操作，云端游戏自动断开 */
    ERROR_GAME_STOPPED_IDLE = 40004,
    /** 服务端主动停止云端游戏 */
    ERROR_GAME_STOPPED_API = 40006,
    /** 游戏停止，原因：在不同设备上使用相同参数请求Start */
    ERROR_GAME_STOPPED_DUPLICATE_START = 40007,
    /** 云端后台超时 */
    ERROR_POD_STOPPED_BACKGROUND_TIMEOUT = 40008,
    /** 云端游戏退出 */
    ERROR_POD_EXIT_GENERAL = 40009,
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
    /** 存档下载失败，Pod启动失败 */
    MESSAGE_RESOURCE_RELEASED_START_ERROR_ARCHIVE_DOWNLOAD_FAILURE = 40050,
    /** 内部错误 */
    ERROR_GAME_EXIT_INTERNAL_ERROR = 40051,
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

/** ----------- 网络探测数据统计 ------------- */
@interface VeGameNetworkProbeStats : NSObject
/**
 * 往返时延
 * 单位：ms
 */
@property (nonatomic, assign) int rtt;
/**
 * 上行网络带宽
 * 单位：kbit/s
 */
@property (nonatomic, assign) int uploadBandwidth;
/**
 * 下行网络带宽
 * 单位：kbit/s
 */
@property (nonatomic, assign) int downloadBandwidth;
/**
 * 上行网络 jitter
 * 单位：ms
 */
@property (nonatomic, assign) int uploadJitter;
/**
 * 下行网络 jitter
 * 单位：ms
 */
@property (nonatomic, assign) int downloadJitter;
/**
 * 上行网络丢包率
 * 单位：%
 */
@property (nonatomic, assign) float uploadLossPercent;
/**
 * 下行网络丢包率
 * 单位：%
 */
@property (nonatomic, assign) float downloadLossPercent;

@end

/** ------------ 控制权转移 ------------- */
@interface VeGameControlObject : NSObject
/**
 * 用户角色
 * 默认：VeBaseRoleTypePlayer
 */
@property (nonatomic, assign) VeBaseRoleType role;
/**
 * 房间类型
 * 默认：VeGameRoomTypeDefault
 */
@property (nonatomic, assign) VeBaseRoomType roomType;

@end

/** ------------ 触控操作 ------------- */
@interface VeGameTouchEventItem : NSObject
/**
 * 游戏画面 X 轴绝对值，[0, 1]
 */
@property (nonatomic, assign) CGFloat x;
/**
 * 游戏画面 Y 轴绝对值，[0, 1]
 */
@property (nonatomic, assign) CGFloat y;
/**
 * 触控唯一id，标识某个手指
 */
@property (nonatomic, assign) NSInteger pointerId;
/**
 * 触控动作
 */
@property (nonatomic, assign) VeGameTouchActionType action;

@end

/** ------------ 鼠标操作 ------------- */
@interface VeGameMouseMessage : NSObject
/**
 * 当 action = VeGameMouseActionTypeMove 时，为游戏画面 X 轴相对值，差值
 * 当 action = VeGameMouseActionTypeCursorPos 时，为游戏画面 X 轴绝对值，[0, 1]
 */
@property (nonatomic, assign) CGFloat x;
/**
 * 当 action = VeGameMouseActionTypeMove 时，为游戏画面 Y 轴相对值，差值
 * 当 action = VeGameMouseActionTypeCursorPos 时，为游戏画面 Y 轴绝对值，[0, 1]
 */
@property (nonatomic, assign) CGFloat y;
/**
 * 滚轮值
 */
@property (nonatomic, assign) int32_t wheel;
/**
 * 按钮类型
 */
@property (nonatomic, assign) VeGameMouseButtonType button;
/**
 * 动作类型
 */
@property (nonatomic, assign) VeGameMouseActionType action;

@end

/** ------------ 手柄操作 ------------- */
@interface VeGameGamePadMessage : NSObject
/**
 * 左扳机，YES：按下；NO：抬起
 */
@property (nonatomic, assign) BOOL lt_status;
/**
 * 右扳机，YES：按下；NO：抬起
 */
@property (nonatomic, assign) BOOL rt_status;
/**
 * 按钮值集合；按下：btn_values |= value；抬起：btn_values &= (~value)
 * BtnUp = 1, BtnDown = 2, BtnLeft = 4, BtnRight = 8, BtnStart = 16, BtnBack = 32, BtnLb = 64, BtnRb = 128, BtnA = 4096, BtnB = 8192, BtnX = 16384, BtnY = 32768
 */
@property (nonatomic, assign) int32_t btn_values;
/**
 * 左摇杆，X方向比例值，(-1, 1)
 */
@property (nonatomic, assign) CGFloat left_joystick_x;
/**
 * 左摇杆，Y方向比例值，(-1, 1)
 */
@property (nonatomic, assign) CGFloat left_joystick_y;
/**
 * 右摇杆，X方向比例值，(-1, 1)
 */
@property (nonatomic, assign) CGFloat right_joystick_x;
/**
 * 右摇杆，Y方向比例值，(-1, 1)
 */
@property (nonatomic, assign) CGFloat right_joystick_y;

@end

/** ------------ 配置参数 ------------- */
@interface VeGameConfigObject : NSObject
/**
 * 鉴权参数（必填）
 */
@property (nonatomic, copy) NSString *ak;
/**
 * 鉴权参数（必填）
 */
@property (nonatomic, copy) NSString *sk;
/**
 * 鉴权参数（必填）
 */
@property (nonatomic, copy) NSString *token;
/**
 * 用户ID（必填）
 */
@property (nonatomic, copy) NSString *userId;
/**
 * 游戏ID（必填）
 * gameId、customGameId 二选一，同时设置时，gameId 优先级大于 customGameId
 */
@property (nonatomic, copy) NSString *gameId;
/**
 * 用户自定义游戏ID（必填）
 * gameId、customGameId 二选一，同时设置时，gameId 优先级大于 customGameId
 */
@property (nonatomic, copy) NSString *customGameId;
/**
 * 当次游戏的生命周期标识（必填）
 * 用该参数查询游戏使用时长，最大长度为128位：大写字母A~Z，小写字母a~z，数字0~9，下划线“_”，减号"-"
 */
@property (nonatomic, copy) NSString *roundId;
/**
 * 火山侧套餐ID（选填）
 * 指定套餐，Debug使用
 */
@property (nonatomic, copy) NSString *planId;
/**
 * 用户标签（选填）
 * 业务方指定，需要用已经创建的标签
 */
@property (nonatomic, copy) NSString *userTag;
/**
 * 预锁定ID（选填）
 */
@property (nonatomic, copy) NSString *reservedId;
/**
 * 键盘开关（选填）
 * 默认：YES
 */
@property (nonatomic, assign) BOOL keyboardEnable;
/**
 * 会话模式（选填）
 * 只支持手游，PC游戏暂不支持
 * 默认：VeBaseSessionModeNormal
 */
@property (nonatomic, assign) VeBaseSessionMode sessionMode;
/**
 * 无操作保活时长（选填）
 * 小于 0 时无效
 */
@property (nonatomic, assign) NSInteger autoRecycleTime;
/**
 * 控制权转移（选填）
 */
@property (nonatomic, strong) VeGameControlObject *control;
/**
 * 排队优先级（选填）
 * 默认值：0，不参与排队，范围 0 ～ 99，数值越小，优先级越高
 */
@property (nonatomic, assign) NSInteger queuePriority;
/**
 * PC游戏，禁用触控操作事件
 * 默认：NO
 * 当设置为 YES 时，接入方会收到“gameManager:onTouchEvent:”回调，然后根据游戏场景调用”sendMouseData:“发送鼠标事件
 */
@property (nonatomic, assign) BOOL disableTouchEvent;
/**
 * 清晰度ID（选填）
 * 默认：使用服务端设定的档位
 */
@property (nonatomic, assign) NSInteger videoStreamProfileId;
/**
 * 游戏用户的配置路径列表（选填）
 */
@property (nonatomic, strong) NSArray<NSString *> *userProfilePathList;
/**
 * 根据业务需要，自定义附加参数（选填）
 */
@property (nonatomic, strong) NSDictionary<NSString *, NSString *> *extraDict;

@end
