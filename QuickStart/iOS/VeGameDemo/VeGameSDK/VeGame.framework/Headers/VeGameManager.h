//
//  VeGameManager.h
//  VePlayer
//
//  Created by Jihua Huang on 2020/6/30.
//  Copyright © 2020 ByteDance Ltd. All rights reserved.
//

#import "VeGameDefines.h"

@class VeGameManager;

@protocol VeGameManagerDelegate <NSObject>

@optional
#pragma mark - “核心”回调
/// 启动成功回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - gameId: 游戏 ID
///   - streamProfileId: 清晰度 ID
///   - reservedId: 预锁定 ID
///   - planId: 火山侧套餐 ID
///   - extra: 自定义扩展信息
- (void)gameManager:(VeGameManager *)manager startSucceedResult:(NSString *)gameId videoStreamProfileId:(NSInteger)streamProfileId reservedId:(NSString *)reservedId planId:(NSString *)planId extra:(NSDictionary *)extra;

/// 远端“首帧音频”回调
/// - Parameter manager: VeGameManager 对象
- (void)firstRemoteAudioFrameArrivedFromGameManager:(VeGameManager *)manager;

/// 远端“旋转”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - rotation: 旋转度
- (void)gameManager:(VeGameManager *)manager changedDeviceRotation:(NSInteger)rotation;

/// 本地“操作延迟”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - delayTime: 时间 ms
- (void)gameManager:(VeGameManager *)manager operationDelay:(NSInteger)delayTime;

/// Pod端退出回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - errCode: 错误码
- (void)gameManager:(VeGameManager *)manager onPodExit:(VeGameErrorCode)errCode;

/// ”MCC“错误回调
/// - Parameters:
///   - manager: VeGameManager
///   - errCode: 错误码
- (void)gameManager:(VeGameManager *)manager onMessageChannleError:(VeGameErrorCode)errCode;

/// ”服务连接状态“回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - state: 连接状态
- (void)gameManager:(VeGameManager *)manager connectionChangedToState:(VeBaseConnectionState)state;

/// 本地“网络类型改变”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - networkType: 变更后的网络类型
- (void)gameManager:(VeGameManager *)manager networkTypeChangedToType:(VeBaseNetworkType)networkType;

/// ”网络质量“回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - quality: 网络质量
- (void)gameManager:(VeGameManager *)manager onNetworkQuality:(VeBaseNetworkQuality)quality;

/// “键盘开关”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - on: 开关
- (void)gameManager:(VeGameManager *)manager keyboardEnable:(BOOL)on;

/// PC游戏“触摸操控事件”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - touchArray: 触摸操控事件数组
- (void)gameManager:(VeGameManager *)manager onTouchEvent:(NSArray<VeGameTouchEventItem *> *)touchArray;

/// PC游戏“远端鼠标事件可见性”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - visible: 是否可见
- (void)gameManager:(VeGameManager *)manager remoteMouseVisibility:(BOOL)visible;

/// 远端“切换分辨率”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - result: 切换结果
///   - index1: from 分辨率
///   - index2: current 分辨率
///   - paramsDict: 目标参数
- (void)gameManager:(VeGameManager *)manager switchVideoStreamProfile:(BOOL)result fromIndex:(NSInteger)index1 toIndex:(NSInteger)index2 targetParams:(NSDictionary *)paramsDict;

/// 拉流过程中，切换”当前会话模式“回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - mode: 会话模式
///   - result: 切换结果
- (void)gameManager:(VeGameManager *)manager onSessionMode:(VeBaseSessionMode)mode result:(BOOL)result;

#pragma mark - ”网络探测“回调
/// 开始“网络探测”回调
/// - Parameter manager: VeGameManager 对象
- (void)onNetProbeStartedFromEngineManager:(VeGameManager *)manager;

/// “网络探测数据统计”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - stats: 统计数据
- (void)gameManager:(VeGameManager *)manager onNetProbeProcess:(VeGameNetworkProbeStats *)stats;

/// “网络探测完成”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - stats: 统计数据
///   - quality: 网络质量
- (void)gameManager:(VeGameManager *)manager onNetProbeCompleted:(VeGameNetworkProbeStats *)stats quality:(VeBaseNetProbeQuality)quality;

/// “网络探测完成”错误
/// - Parameters:
///   - manager: VeGameManager 对象
///   - code: 错误码
- (void)gameManager:(VeGameManager *)manager onNetProbeError:(VeBaseNetProbeErrorCode)code;

#pragma mark - “数据通道”回调
/// 远端“MCC”在线状态回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - channel_uid: 消息通道ID
- (void)gameManager:(VeGameManager *)manager onRemoteMessageOnline:(NSString *)channel_uid;

/// 远端“MCC”离线状态回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - channel_uid: 消息通道ID
- (void)gameManager:(VeGameManager *)manager onRemoteMessageOffline:(NSString *)channel_uid;

/// 远端通过“MCC”发送消息回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - message: 消息
- (void)gameManager:(VeGameManager *)manager onReceiveMessage:(VeBaseChannelMessage *)message;

/// 本地通过“MCC”发送消息结果回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - result: 结果；YES：成功 NO：超时失败
///   - mid: 消息id
- (void)gameManager:(VeGameManager *)manager onSendMessageResult:(BOOL)result messageId:(NSString *)mid;

/// 远端发送“剪贴板数据”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - datArray: 数据
- (void)gameManager:(VeGameManager *)manager receivedClipBoardMessage:(NSArray *)datArray;

/// 远端发送“通用字符串消息”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - dataDict: 消息体
- (void)gameManager:(VeGameManager *)manager receivedGeneralStringMessage:(NSString *)dataString;

/// 远端发送“通用二进制消息”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - dataDict: 消息体
- (void)gameManager:(VeGameManager *)manager receivedGeneralBinaryMessage:(NSData *)data;

#pragma mark - “音频采集”回调
/// 远端“开始音频采集”请求回调
/// - Parameter manager: VeGameManager 对象
- (void)startAudioCaptureRequestFromGameManager:(VeGameManager *)manager;

/// 远端“停止音频采集”请求回调
/// - Parameter manager: VeGameManager 对象
- (void)stopAudioCaptureRequestFromGameManager:(VeGameManager *)manager;

/// “音频采集设备状态”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - state: 设备状态
///   - error: 错误信息
- (void)gameManager:(VeGameManager *)manager onAudioCaptureDeviceState:(VeBaseMediaDeviceState)state deviceError:(VeBaseMediaDeviceError)error;

/// 本地“音频播放设备变化”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - audioRoute: 设备类型
- (void)gameManager:(VeGameManager *)manager onAudioRouteChanged:(VeBaseAudioRoute)audioRoute;

/// 远端用户的音频变化
/// - Parameters:
///   - manager: VeGameManager 对象
///   - volume: 远端音量，[0, 100]
///   - uid: 远端用户id
- (void)gameManager:(VeGameManager *)manager onRemoteAudioPlaybackVolumeChanged:(NSInteger)volume fromUid:(NSString *)uid;

#pragma mark - “视频采集”回调
/// 远端“开始视频采集”请求回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - cameraId: 摄像头类型
- (void)gameManager:(VeGameManager *)manager startVideoCaptureRequest:(VeBaseCameraId)cameraId;

/// 远端“停止视频采集”请求回调
/// - Parameter manager: VeGameManager 对象
- (void)stopVideoCaptureRequestFromGameManager:(VeGameManager *)manager;

/// 本地“采集视频”首帧回调
/// - Parameter manager: VeGameManager 对象
- (void)firstLocalVideoFrameCapturedFromGameManager:(VeGameManager *)manager;

/// “视频采集设备状态”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - state: 设备状态
///   - error: 错误信息
- (void)gameManager:(VeGameManager *)manager onVideoCaptureDeviceState:(VeBaseMediaDeviceState)state deviceError:(VeBaseMediaDeviceError)error;

#pragma mark - “无操作回收时长”回调
/// 设置“无操作回收时长”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - code: 错误码；0：成功 -1：失败
///   - time: 无操作时长
- (void)gameManager:(VeGameManager *)manager setAutoRecycleTimeCallback:(NSInteger)code time:(NSInteger)time;

/// 获取“无操作回收时长”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - code: 错误码；0：成功 -1：失败
///   - time: 无操作时长
- (void)gameManager:(VeGameManager *)manager getAutoRecycleTimeCallback:(NSInteger)code time:(NSInteger)time;

#pragma mark - 切换“APP前后台”回调
/// 本地“切换前后台”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - code 0：后台；1：前台
- (void)gameManager:(VeGameManager *)manager onBackgroundSwitched:(NSInteger)code;

/// 远端“游戏切换到前台”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - switchType: 切换类型
- (void)gameManager:(VeGameManager *)manager onRemoteGameSwitchedForeground:(VeBaseRemoteAppSwitchedType)switchType;

/// 远端“游戏切换到后台”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - switchType: 切换类型
- (void)gameManager:(VeGameManager *)manager onRemoteGameSwitchedBackground:(VeBaseRemoteAppSwitchedType)switchType;

/// 远端“游戏切换前后台失败”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - warningCode: 警告码
///   - errorMsg: 错误信息
- (void)gameManager:(VeGameManager *)manager onRemoteGameSwitchedFailedWithCode:(VeGameWarningCode)warningCode errorMsg:(NSString *)errorMsg;

#pragma mark - 设置“用户配置信息保存还原路径”回调
/// 设置“用户配置信息保存还原路径”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - type: 0：设置；1：还原
///   - result: 结果
- (void)gameManager:(VeGameManager *)manager setUserProfilePathType:(NSInteger)type result:(BOOL)result;

/// 获取“用户配置信息保存还原路径”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - stats: list 配置列表
- (void)gameManager:(VeGameManager *)manager getUserProfilePathList:(NSArray<NSString *> *)list;

#pragma mark - 媒体流“统计数据”回调
/// 本地“流数据统计及网络质量”回调，本地发布流成功后，SDK 会周期性（2s）的通知用户发布的流在此次统计周期内的质量统计信息
/// - Parameters:
///   - manager: VeGameManager 对象
///   - stats: 统计数据
- (void)gameManager:(VeGameManager *)manager onLocalStreamStats:(VeBaseLocalStreamStats *)stats;

/// 远端“流数据统计及网络质量”回调，本地订阅流成功后，SDK 会周期性（2s）的通知用户发布的流在此次统计周期内的质量统计信息
/// - Parameters:
///   - manager: VeGameManager 对象
///   - stats: 统计数据
- (void)gameManager:(VeGameManager *)manager onRemoteStreamStats:(VeBaseRemoteStreamStats *)stats;

#pragma mark - 游戏“控制权”转移回调
/// 首次加入房间后，当前“控制权”信息回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - role: 当前用户角色
///   - reason: 初始化角色 与 最终角色的结果，0：成功；其他：失败
///   - player_uid: 当前 Player 的 Uid
- (void)gameManager:(VeGameManager *)manager onJoinRoomRoleResult:(VeBaseRoleType)role reason:(NSInteger)reason playerUserId:(NSString *)player_uid;

/// 游戏过程中“控制权”发生转移，给发起方的结果回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - role: 目标用户最终角色
///   - result: 目标用户之前角色 与 最终角色的结果，0：成功；其他：失败
///   - dest_uid: 目标用户的 Uid
- (void)gameManager:(VeGameManager *)manager onChangeRoleCallBack:(VeBaseRoleType)role result:(NSInteger)result destUserId:(NSString *)dest_uid;

/// 游戏过程中“控制权”发生转移，向房间内所有人进行广播的回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - player_uid: 当前房间内 Player 的 Uid
- (void)gameManager:(VeGameManager *)manager onPlayerChanged:(NSString *)player_uid;

#pragma mark - ”大文件传输下载“回调
/// 文件传输下载开始
/// - Parameters:
///   - manager: VeGameManager 对象
///   - file: 下载对象
///   - options: 自定义参数
- (void)gameManager:(VeGameManager *)manager downloadOnStart:(VeFile *)file options:(NSDictionary <NSString *, NSString *>*)options;

/// 文件传输下载开始
/// - Parameters:
///   - manager: VeGameManager 对象
///   - file: 下载对象
///   - options: 自定义参数
///   - progress: 下载进度
- (void)gameManager:(VeGameManager *)manager downloadOnProgress:(VeFile *)file options:(NSDictionary <NSString *, NSString *>*)options progress:(NSInteger)progress;

/// 文件传输下载完成
/// - Parameters:
///   - manager: VeGameManager 对象
///   - file: 下载对象
///   - options: 自定义参数
- (void)gameManager:(VeGameManager *)manager downloadOnComplete:(VeFile *)file options:(NSDictionary <NSString *, NSString *>*)options;

/// 文件传输下载错误
/// - Parameters:
///   - manager: VeGameManager 对象
///   - file: 下载对象
///   - options: 自定义参数
///   - err: 错误码
- (void)gameManager:(VeGameManager *)manager downloadOnError:(VeFile *)file options:(NSDictionary <NSString *, NSString *>*)options err:(VeGameErrorCode)err;

/// 文件传输下载取消
/// - Parameters:
///   - manager: VeGameManager 对象
///   - file: 下载对象
///   - options: 自定义参数
- (void)gameManager:(VeGameManager *)manager downloadOnCancel:(VeFile *)file options:(NSDictionary <NSString *, NSString *>*)options;

#pragma mark - “定位信息”回调
/// 远端请求定位时，位置信息回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - location: 位置信息
- (void)gameManager:(VeGameManager *)manager onSentLocalLocation:(VeBaseLocationInfo *)location;

#pragma mark - “启动游戏排队”回调
/// “启动游戏排队”回调
/// - Parameters:
///   - manager: VeGameManager
///   - queueInfoList: 排队信息[{@"configuration_code": @"ARMSoC_Standard", @"user_position": @1, @"total": @1}]
- (void)gameManager:(VeGameManager *)manager onQueueUpdate:(NSArray<NSDictionary *> *)queueInfoList;

/// “启动游戏排队完成”回调
/// - Parameters:
///   - manager: VeGameManager
///   - remainTime:
- (void)gameManager:(VeGameManager *)manager onQueueSuccessAndStart:(NSInteger)remainTime;

#pragma mark - “警告&错误”回调
/// “警告”回调
/// - Parameters:
///   - manager: VeGameManager
///   - warnCode: 警告码
- (void)gameManager:(VeGameManager *)manager onWarning:(VeGameWarningCode)warnCode;

/// “错误”回调
/// - Parameters:
///   - manager: VeGameManager
///   - errCode: 错误码
- (void)gameManager:(VeGameManager *)manager onError:(VeGameErrorCode)errCode;

@end

@interface VeGameManager : NSObject
/**
 * 代理
 */
@property (nonatomic, weak) id<VeGameManagerDelegate>delegate;
/**
 * 容器视图，接入方自定义
 */
@property (nonatomic, strong) UIView *containerView;
/**
 * 设置”陀螺仪“开关
 * 默认：NO
 * 支持拉流过程中修改
 */
@property (nonatomic, assign) BOOL gyroEnable;
/**
 * 设置”震动“开关
 * 默认：NO
 * 支持拉流过程中修改
 */
@property (nonatomic, assign) BOOL vibratorEnable;
/**
 * 设置”定位“开关
 * 默认：NO
 * 支持拉流过程中修改
 */
@property (nonatomic, assign) BOOL locationEnable;
/**
 * 设置”方向“开关
 * 默认：NO
 * 支持拉流过程中修改
 */
@property (nonatomic, assign) BOOL oritationEnable;
/**
 * 设置”磁力“开关
 * 默认：NO
 * 支持拉流过程中修改
 */
@property (nonatomic, assign) BOOL magnetometerEnable;
/**
 * 设置”加速度“开关
 * 默认：NO
 * 支持拉流过程中修改
 */
@property (nonatomic, assign) BOOL accelerometerEnable;
/**
 * 设置"扩展"参数
 */
@property (nonatomic, strong) NSDictionary *extraParameters;
/**
 * 设置”游戏手柄“
 */
@property (nonatomic, strong) UIView *gamePadView;

/// 初始化SDK，设置全局配置信息
/// - Parameter accountId: 火山账号ID
/// - Notes 建议在程序启动的时候调用，时机越早越好
- (void)initWithAccountId:(NSString *)accountId;

/// 获取管理器实例
/// - Reutrn VeGameManager 对象
+ (VeGameManager *)sharedInstance;

/// 版本号
/// - Reutrn 版本号
+ (NSString *)currentVersion;

/// 获取设备ID
/// - Reutrn 16位，纯数字
+ (NSString *)currentDeviceId;

/// 获取“服务端”设备ID
/// - Reutrn 36位，字母数字组合
+ (NSString *)serviceDeviceId;

/// 启动接口
/// - Parameter configObj: 配置参数
- (void)startWithConfig:(VeGameConfigObject *)configObj;

/// 停止退出
- (void)stop;

/// 重启游戏
- (void)restartGame;

/// 开始网络探测
/// - Parameter configObj: 配置参数
/// - Notes configObj 只需 ak、sk、token、userId
- (void)probeStart:(VeGameConfigObject *)configObj;

/// 停止网络探测
- (void)probeInterrupt;

/// 音频开关
/// - Parameter on: 开关
- (void)muteAudio:(BOOL)on;

/// 视频开关
/// - Parameter on: 开关
- (void)muteVideo:(BOOL)on;

/// 音视频开关
/// - Parameter on: 开关
- (void)switchPaused:(BOOL)on;

/// 发送消息
/// - Parameter payload: 消息体
/// - Return MessageChannel 消息参数
- (VeBaseChannelMessage *)sendMessage:(NSString *)payload;

/// 发送消息
/// - Parameters:
///   - payload: 消息体
///   - channel_uid: 消息通道ID（云端初始化 veMessageChannelClient 时传入的 Uid）
/// - Return MessageChannel 消息参数
- (VeBaseChannelMessage *)sendMessage:(NSString *)payload channel:(NSString *)channel_uid;

/// 发送消息
/// - Parameters:
///   - payload: 消息体
///   - time: 超时时间，ms
/// - Return MessageChannel 消息参数
/// - Notes 调用此接口，会触发“gameManager:onSendMessageResult:messageId:”回调
- (VeBaseChannelMessage *)sendMessage:(NSString *)payload timeout:(NSInteger)time;

/// 发送消息
/// - Parameters:
///   - payload: 消息体
///   - time: 超时时间，ms
///   - channel_uid: 消息通道ID（云端初始化 veMessageChannelClient 时传入的 Uid）
/// - Return MessageChannel 消息参数
/// - Notes 调用此接口，会触发“gameManager:onSendMessageResult:messageId:”回调
- (VeBaseChannelMessage *)sendMessage:(NSString *)payload timeout:(NSInteger)time channel:(NSString *)channel_uid;

/// 发送通用字符串消息
/// @param message 消息体
- (void)sendGeneralStringMessage:(NSString *)message;

/// 发送通用二进制消息
/// @param message 消息体
- (void)sendGeneralBinaryMessage:(NSData *)message;

/// 剪贴板数据通道
/// - Parameter dataArray: 数据
/// - Notes 调用此接口，会触发“gameManager:receivedClipBoardMessage:”回调
- (void)sendClipBoardMessage:(NSArray<NSString *> *)dataArray;

/// 游戏过程中，切换视频流清晰度
/// - Parameter streamProfileId: 清晰度Id
/// - Notes 调用此接口，会触发“gameManager:switchVideoStreamProfile:fromIndex:toIndex:targetParams:”回调
- (void)switchVideoStreamProfile:(NSInteger)streamProfileId;

/// PC游戏，发生手柄数据
/// - Parameters gamepad: 手柄消息
- (void)sendGamePadData:(VeGameGamePadMessage *)gamepad;

/// PC游戏，发送鼠标事件
/// - Parameter mouse: 鼠标消息
- (void)sendMouseData:(VeGameMouseMessage *)mouse;

/// PC游戏，发送键盘事件
/// - Parameters:
///   - key: 按键类型
///   - state: YES：按下；NO：抬起
- (void)sendKeyboardData:(VeGameKeyboardKeyType)key state:(BOOL)state;

/// 开启音频采集
- (void)startAudioStream;

/// 停止音频采集
- (void)stopAudioStream;

/// 调节“本地音频”播放音量
/// - Parameter volume: 默认：100%，范围：[0, 100]，单位%
- (void)setLocalAudioPlaybackVolume:(NSInteger)volume;

/// 获取“本地音频”播放音量
/// - Return 默认：100%，范围：[0, 100]，单位%
- (NSInteger)getLocalAudioPlaybackVolume;

/// 调节“本地音频”采集音量
/// - Parameter volume: 默认：100%，范围：[0, 100]，单位%
- (void)setLocalAudioCaptureVolume:(NSInteger)volume;

/// 获取“本地音频”采集音量
/// - Return 默认：100%，范围：[0, 100]，单位%
- (NSInteger)getLocalAudioCaptureVolume;

/// 调节“远端音频”播放音量
/// - Parameter volume: 默认：100%，范围：[0, 100]，单位%
- (void)setRemoteAudioPlaybackVolume:(NSInteger)volume;

/// 获取“远端音频”播放音量
/// - Return 默认：100%，范围：[0, 100]，单位%
- (NSInteger)getRemoteAudioPlaybackVolume;

/// 设置音频播放设备
/// - Parameter audioRoute: 设备类型
- (void)setAudioPlaybackDevice:(VeBaseAudioRoute)audioRoute;

/// 获取音频播放设备
/// - Return 设备类型
- (VeBaseAudioRoute)getAudioPlaybackDevice;

/// 开始视频采集
/// - Parameter cameraId: 摄像头类型
- (void)startVideoStream:(VeBaseCameraId)cameraId;

/// 切换摄像头
/// - Parameter cameraId: 摄像头类型
- (void)switchCamera:(VeBaseCameraId)cameraId;

/// 停止视频采集
- (void)stopVideoStream;

/// 设置本地视频渲染视图
/// - Parameter view: 自定义视图
/// - Notes 默认渲染模式：VeBaseRenderModeHidden
- (void)setLocalVideoCanvas:(UIView *)view;

/// 设置本地视频渲染视图
/// - Parameters:
///   - view: 自定义视图
///   - mode: 渲染模式
- (void)setLocalVideoCanvas:(UIView *)view renderMode:(VeBaseRenderMode)mode;

/// 设置镜像类型
/// - Parameter type: 镜像
- (void)setLocalVideoMirrorType:(VeBaseMirrorType)type;

/// 设置视频编码传输时的视频参数
/// - Parameter videoSolutions: 视频参数
- (void)setVideoEncoderConfig:(NSArray<VeBaseVideoSolution *> *)videoSolutions;

/// 切换前后台
/// - Parameter on: YES，后台；NO，前台
/// - Notes 调用此接口，会触发“gameManager:onBackgroundSwitched:”回调
- (void)switchBackground:(BOOL)on;

/// 设置后台保活时长
/// - Parameter time: 默认300s
/// - Return 0：接口调用成功；-2：time小于等于0
- (NSInteger)setIdleTime:(NSInteger)time;

/// 设置无操作回收时长
/// - Parameter time: 默认300s
/// - Return 0：接口调用成功；-2：time小于等于0
/// - Notes 调用此接口，会触发“gameManager:setAutoRecycleTimeCallback:time:”回调
- (NSInteger)setAutoRecycleTime:(NSInteger)time;

/// 获取无操作回收时长
/// - Notes 调用此接口，会触发“gameManager:getAutoRecycleTimeCallback:time:”回调
- (void)getAutoRecycleTime;

/// 设置“用户配置信息保存还原”路径
/// - Notes 调用此接口，会触发”gameManager:setUserProfilePathType:result:”回调
- (void)setUserProfilePathList:(NSArray<NSString *> *)pathList;

/// 获取“用户配置信息保存还原”路径
/// - Notes 调用此接口，会触发 gameManager:getUserProfilePathList: 回调
- (void)getUserProfilePathList;

/// 切换远端游戏到前台
/// - Notes 调用此接口，会触发”gameManager:onRemoteAppSwitchedForeground:switchType:、gameManager:onRemoteAppSwitchedBackground:switchType:”回调
- (void)setRemoteGameForeground;

/// 修改目标用户角色
/// - Parameters:
///   - destUid: 目标用户Id
///   - role: 目标用户角色
/// - Notes 调用此接口，会触发”gameManager:onChangeRoleCallBack:result:destUserId:”回调
- (void)changeRole:(NSString *)destUid role:(VeBaseRoleType)role;

/// 获取当前用户角色
- (VeBaseRoleType)getCurrentRole;

/// 设置键盘开关
/// - Parameter on: 开关
/// - Notes 调用此接口，会触发”gameManager:keyboardEnable:”回调
- (void)setKeyboardEnable:(BOOL)on;

/// 查询当前用户键盘开关
- (BOOL)getKeyboardEnable;

/// 设置当前会话模式
/// - Parameter mode: 会话模式
/// - Notes 只支持手游，调用此接口，会触发”gameManager:onSessionMode:result:”回调
- (void)setSessionMode:(VeBaseSessionMode)mode;

/// 开始上传文件
/// - Parameters:
///   - file: 文件路径
///   - options: 自定义参数
///   - start: 流程开始
///   - progress: 上传进度回调
///   - complete: 上传完成回调
///   - cancel: 取消上传回调
///   - error: 上传错误回调
- (void)startSendFile:(VeFile *)file
              options:(NSDictionary <NSString *, NSString *>*)options
              onStart:(void(^)(VeFile *file, NSDictionary <NSString *, NSString *>*options))start
           onProgress:(void(^)(VeFile *file, NSDictionary <NSString *, NSString *>*options, NSInteger progress))progress
           onComplete:(void(^)(VeFile *file, NSDictionary <NSString *, NSString *>*options))complete
             onCancel:(void(^)(VeFile *file, NSDictionary <NSString *, NSString *>*options))cancel
              onError:(void(^)(VeFile *file, NSDictionary <NSString *, NSString *>*options, VeGameErrorCode err))error;

/// 终止文件传输任务
/// - Parameters:
///   - file: 文件对象
- (void)stopSendFile:(VeFile *)file;

/// 终止文件接收任务
/// - Parameters:
///   - file: 文件对象
- (void)stopReceiveFile:(VeFile *)file;

@end
