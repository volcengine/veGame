//
//  VeGameManager.h
//  VePlayer
//
//  Created by Jihua Huang on 2020/6/30.
//  Copyright © 2020 ByteDance Ltd. All rights reserved.
//

#import "VeGameDefines.h"
#import "VeBaseDefines.h"

@class VeGameManager;

@protocol VeGameManagerDelegate <NSObject>

@optional
#pragma mark - “核心”回调
/// 启动成功回调
/// @param manager VeGameManager 对象
/// @param gameId 游戏 ID
/// @param streamProfileId 清晰度 ID
/// @param reservedId 预锁定 ID
/// @param extra 自定义信息
- (void)gameManager:(VeGameManager *)manager startSucceedResult:(NSString *)gameId videoStreamProfileId:(NSInteger)streamProfileId reservedId:(NSString *)reservedId extra:(NSDictionary *)extra;

/// 远端“首帧音频”回调
/// @param manager VeGameManager 对象
- (void)firstRemoteAudioFrameArrivedFromEngineManager:(VeGameManager *)manager;

/// 远端“旋转”回调
/// @param manager VeGameManager 对象
/// @param rotation 旋转度
- (void)gameManager:(VeGameManager *)manager changedDeviceRotation:(NSInteger)rotation;

/// 本地“操作延迟”回调
/// @param manager VeGameManager 对象
/// @param delayTime 时间 ms
- (void)gameManager:(VeGameManager *)manager operationDelay:(NSInteger)delayTime;

/// 连接状态回调
/// @param manager VeGameManager 对象
/// @param state 连接状态
- (void)gameManager:(VeGameManager *)manager connectionChangedToState:(VeBaseConnectionState)state;

/// 本地“网络类型改变”回调
/// @param manager VeGameManager 对象
/// @param networkType 变更后的网络类型
- (void)gameManager:(VeGameManager *)manager networkTypeChangedToType:(VeBaseNetworkType)networkType;

/// 网络质量
/// @param manager VeGameManager 对象
/// @param quality 网络质量
- (void)gameManager:(VeGameManager *)manager onNetworkQuality:(VeBaseNetworkQuality)quality;

/// “键盘开关”回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - enable: 开关
- (void)gameManeger:(VeGameManager *)manager keyboardEnable:(BOOL)enable;

/// 远端“切换分辨率”回调
/// @param manager VeGameManager 对象
/// @param code 错误码（成功：0 失败：other）
/// @param index1 from分辨率
/// @param index2 current分辨率
- (void)gameManager:(VeGameManager *)manager switchVideoStreamProfileWithCode:(NSInteger)code fromIndex:(NSInteger)index1 toIndex:(NSInteger)index2;

#pragma mark - ”网络探测“回调
/// 开始“网络探测”回调
- (void)onNetProbeStartedFromEngineManager:(VeGameManager *)manager;

/// “网络探测数据统计”回调
/// @param manager VeGameManager 对象
/// @param stats 统计数据
- (void)gameManager:(VeGameManager *)manager onNetProbeProcess:(VeGameNetworkProbeStats *)stats;

/// “网络探测完成”回调
/// @param manager VeGameManager 对象
/// @param stats 统计数据
/// @param quality 网络质量
- (void)gameManager:(VeGameManager *)manager onNetProbeCompleted:(VeGameNetworkProbeStats *)stats quality:(VeBaseNetProbeQuality)quality;

/// “网络探测完成”错误
/// @param manager VeGameManager 对象
/// @param code 错误码
- (void)gameManager:(VeGameManager *)manager onNetProbeError:(VeBaseNetProbeErrorCode)code;

#pragma mark - “数据通道”回调
/// 远端“MessageChannel”在线状态回调
/// @param manager VeGameManager 对象
/// @param channel_uid 消息通道ID
- (void)gameManager:(VeGameManager *)manager onRemoteMessageOnline:(NSString *)channel_uid;

/// 远端“MessageChannel”离线状态回调
/// @param manager VeGameManager 对象
/// @param channel_uid 消息通道ID
- (void)gameManager:(VeGameManager *)manager onRemoteMessageOffline:(NSString *)channel_uid;

/// 远端发送消息回调
/// @param manager VeGameManager 对象
/// @param message 消息
- (void)gameManager:(VeGameManager *)manager onReceiveMessage:(VeBaseChannelMessage *)message;

/// 本地发送消息结果回调
/// @param manager VeGameManager 对象
/// @param result 结果 YES：成功 NO：超时失败
/// @param mid 消息id
- (void)gameManager:(VeGameManager *)manager onSendMessageResult:(BOOL)result messageId:(NSString *)mid;

/// 远端“剪贴板数据”回调
/// @param manager VeGameManager 对象
/// @param datArray 数据
- (void)gameManager:(VeGameManager *)manager receivedClipBoardMessage:(NSArray *)datArray;

/// 远端“通用消息”回调
/// @param manager VeGameManager 对象
/// @param dataDict 消息体
- (void)gameManager:(VeGameManager *)manager receivedGeneralMessage:(NSDictionary *)dataDict;

#pragma mark - “音频采集”回调
/// 远端“开始音频采集”请求回调
/// @param manager VeCoreEngineManager 对象
- (void)startAudioCaptureRequestFromGameManager:(VeGameManager *)manager;

/// 远端“停止音频采集”请求回调
/// @param manager VeCoreEngineManager 对象
- (void)stopAudioCaptureRequestFromGameManager:(VeGameManager *)manager;

/// 本地“音频流状态”回调
/// @param manager VeGameManager 对象
/// @param state 状态
/// @param error 错误码
- (void)gameManager:(VeGameManager *)manager onLocalAudioStreamState:(VeBaseLocalAudioStreamState)state streamError:(VeBaseLocalAudioStreamError)error;

/// 远端“音频流状态”回调
/// @param manager VeGameManager 对象
/// @param state 状态
/// @param reason 原因
- (void)gameManager:(VeGameManager *)manager onRemoteAudioStreamState:(VeBaseRemoteAudioStreamState)state streamReason:(VeBaseRemoteAudioStreamStateChangeReason)reason;

/// 本地“音频采集设备启动成功”回调
/// @param manager VeGameManager 对象
/// @param success 是否成功
- (void)gameManager:(VeGameManager *)manager onAudioCaptureDeviceStartState:(BOOL)success;

/// 本地“音频播放设备变化”回调
/// @param manager VeGameManager 对象
/// @param audioRoute 设备类型
- (void)gameManager:(VeGameManager *)manager onAudioRouteChanged:(VeBaseAudioRoute)audioRoute;

#pragma mark - “视频采集”回调
/// 远端“开始视频采集”请求回调
/// @param manager VeGameManager 对象
/// @param cameraId 前后摄像头
- (void)gameManager:(VeGameManager *)manager startVideoCaptureRequest:(VeBaseCameraId)cameraId;

/// 远端“停止视频采集”请求回调
/// @param manager VeGameManager 对象
- (void)stopVideoCaptureRequestFromGameManager:(VeGameManager *)manager;

/// 本地“视频流状态”回调
/// @param manager VeGameManager 对象
/// @param state 状态
/// @param error 错误码
- (void)gameManager:(VeGameManager *)manager onLocalVideoStreamState:(VeBaseLocalVideoStreamState)state streamError:(VeBaseLocalVideoStreamError)error;

/// 远端“视频流状态”回调
/// @param manager VeGameManager 对象
/// @param state 状态
/// @param reason 原因
- (void)gameManager:(VeGameManager *)manager onRemoteVideoStreamState:(VeBaseRemoteVideoStreamState)state streamReason:(VeBaseRemoteVideoStreamStateChangeReason)reason;

#pragma mark - “挂机时长”回调
/// 本地设置“挂机时长”回调
/// @param manager VeGameManager 对象
/// @param code 错误码，0：成功 -1：失败
/// @param time 当前挂机时长
- (void)gameManager:(VeGameManager *)manager setAutoRecycleTimeCallback:(NSInteger)code time:(NSInteger)time;

/// 本地获取“挂机时长”回调
/// @param manager VeGameManager 对象
/// @param code 错误码，0：成功 -1：失败
/// @param time 当前挂机时长
- (void)gameManager:(VeGameManager *)manager getAutoRecycleTimeCallback:(NSInteger)code time:(NSInteger)time;

#pragma mark - 切换“APP前后台”回调
/// 本地“切换前后台”回调
/// @param manager VeGameManager 对象
/// @param code 0：后台，1：前台
- (void)gameManager:(VeGameManager *)manager onBackgroundSwitched:(NSInteger)code;

/// 远端“游戏切换到前台”回调
/// @param manager VeGameManager 对象
/// @param switchType 切换类型
- (void)gameManager:(VeGameManager *)manager onRemoteGameSwitchedForeground:(VeBaseRemoteAppSwitchedType)switchType;

/// 远端“游戏切换到后台”回调
/// @param manager VeGameManager 对象
/// @param switchType 切换类型
- (void)gameManager:(VeGameManager *)manager onRemoteGameSwitchedBackground:(VeBaseRemoteAppSwitchedType)switchType;

/// 远端“游戏切换前后台失败”回调
/// @param manager VeGameManager 对象
/// @param warningCode 警告码
/// @param errorMsg 错误信息
- (void)gameManager:(VeGameManager *)manager onRemoteGameSwitchedFailedWithCode:(VeGameWarningCode)warningCode errorMsg:(NSString *)errorMsg;

#pragma mark - 设置“用户配置信息保存还原路径”回调
/// 设置“用户配置信息保存还原路径”结果回调（调用“setUserProfilePathList:”触发）
/// @param manager VeGameManager 对象
/// @param type 0：设置 1：还原
/// @param result 结果
- (void)gameManager:(VeGameManager *)manager setUserProfilePathType:(NSInteger)type result:(BOOL)result;

/// 获取“用户配置信息保存还原路径”回调（调用“getUserProfilePathList”触发）
/// @param manager VeGameManager 对象
/// @param list 配置列表
- (void)gameManager:(VeGameManager *)manager getUserProfilePathList:(NSArray<NSString *> *)list;

#pragma mark - 媒体流“统计数据”回调
/// 本地“流数据统计”回调
/// @param manager VeGameManager 对象
/// @param stats 统计数据
- (void)gameManager:(VeGameManager *)manager onLocalStreamStats:(VeBaseLocalStreamStats *)stats;

/// 远端“流数据统计”回调
/// @param manager VeGameManager 对象
/// @param stats 统计数据
- (void)gameManager:(VeGameManager *)manager onRemoteStreamStats:(VeBaseRemoteStreamStats *)stats;

#pragma mark - 游戏“控制权”转移回调
/// 首次加入房间后，当前“控制权”信息回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - role: 当前用户角色
///   - reason: 初始化角色 与 最终角色的结果，0：成功；其他：失败
///   - player_uid: 当前 Player 的 Uid
- (void)gameManeger:(VeGameManager *)manager onJoinRoomRoleResult:(VeGameRoleType)role reason:(NSInteger)reason playerUserId:(NSString *)player_uid;

/// 游戏过程中“控制权”发生转移，给发起方的结果回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - role: 目标用户最终角色
///   - result: 目标用户之前角色 与 最终角色的结果，0：成功；其他：失败
///   - dest_uid: 目标用户的 Uid
- (void)gameManeger:(VeGameManager *)manager onChangeRoleCallBack:(VeGameRoleType)role result:(NSInteger)result destUserId:(NSString *)dest_uid;

/// 游戏过程中“控制权”发生转移，向房间内所有人进行广播的回调
/// - Parameters:
///   - manager: VeGameManager 对象
///   - player_uid: 当前房间内 Player 的 Uid
- (void)gameManeger:(VeGameManager *)manager onPlayerChanged:(NSString *)player_uid;

#pragma mark - ”大文件传输下载“回调
/// 文件传输下载开始
/// - Parameters:
///   - manager: VeGameManager 对象
///   - file: 下载对象
- (void)gameManager:(VeGameManager *)manager downloadOnStart:(VeFile *)file;

/// 文件传输下载开始
/// - Parameters:
///   - manager: VeGameManager 对象
///   - file: 下载对象
///   - progress: 下载进度
- (void)gameManager:(VeGameManager *)manager downloadOnProgress:(VeFile *)file progress:(NSInteger)progress;

/// 文件传输下载完成
/// - Parameters:
///   - manager: VeGameManager 对象
///   - file: 下载对象
- (void)gameManager:(VeGameManager *)manager downloadOnComplete:(VeFile *)file;

/// 文件传输下载错误
/// - Parameters:
///   - manager: VeGameManager 对象
///   - file: 下载对象
///   - err: 错误码
- (void)gameManager:(VeGameManager *)manager downloadOnError:(VeFile *)file err:(VeGameErrorCode)err;

/// 文件传输下载取消
/// - Parameters:
///   - manager: VeGameManager 对象
///   - file: 下载对象
- (void)gameManager:(VeGameManager *)manager downloadOnCancel:(VeFile *)file;

#pragma mark - “警告&错误”回调
/// 警告回调
/// @param manager VeGameManager 对象
/// @param warnCode 警告码
- (void)gameManager:(VeGameManager *)manager onWarning:(VeGameWarningCode)warnCode;

/// 错误回调
/// @param manager VeGameManager 对象
/// @param errCode 错误码
- (void)gameManager:(VeGameManager *)manager onError:(VeGameErrorCode)errCode;

/// ”MessageChannel错误“回调
/// @param manager VeGameManager 对象
/// @param errCode 警告码
- (void)gameManager:(VeGameManager *)manager onMessageChannleError:(VeGameErrorCode)errCode;

@end

@interface VeGameManager : NSObject
/** 陀螺仪开关，默认：NO */
@property (nonatomic, assign) BOOL gyroEnable;
/** 震动开关，默认：YES */
@property (nonatomic, assign) BOOL vibratorEnable;
/** 定位开关，默认：NO */
@property (nonatomic, assign) BOOL locationEnable;
/** 方向开关，默认：NO */
@property (nonatomic, assign) BOOL oritationEnable;
/** 磁力开关，默认：NO */
@property (nonatomic, assign) BOOL magnetometerEnable;
/** 加速度开关，默认：NO */
@property (nonatomic, assign) BOOL accelerometerEnable;
/// 扩展参数
@property (nonatomic, strong) NSDictionary *extraParameters;
/// 游戏手柄
@property (nonatomic, strong) UIView *gamepadView;
/// 大文件传输通道
@property (nonatomic, copy, readonly) NSString *fileChannel;

/// 设置配置信息（建议在程序启动的时候调用，时机越早越好）
/// - Parameter accountId: 火山账号ID
+ (void)setupConfigWithAccountId:(NSString *)accountId;

/// 创建“VeGameManager”单例对象
/// - Parameters:
///   - containerView: 视图容器
///   - delegate: 代理
+ (instancetype)sharedManagerWithContainerView:(UIView *)containerView
                                      delegate:(NSObject<VeGameManagerDelegate> *)delegate;

/// 获取“VeGameManager”单例对象
+ (VeGameManager *)sharedInstance;

/// 版本号
+ (NSString *)currentVersion;

/// 当前设备ID
+ (NSString *)currentDeviceId;

/// 当前服务端设备ID
+ (NSString *)serviceDeviceId;

/// 开启后，会将日志写入本地，并输出到控制台，默认：NO
- (void)logEnable:(BOOL)enable;

/// 开始网络探测
/// @param configObj 配置参数（只需ak、sk、token、userId）
- (void)probeStart:(VeGameConfigObject *)configObj;

/// 停止网络探测
- (void)probeInterrupt;

/// 启动接口
/// @param configObj 配置参数
- (void)startWithConfig:(VeGameConfigObject *)configObj;

/// 停止退出
- (void)stop;

/// 重启游戏
- (void)restartGame;

/// 音频开关
- (void)muteAudio:(BOOL)on;

/// 视频开关
- (void)muteVideo:(BOOL)on;

/// 音视频开关
- (void)switchPaused:(BOOL)on;

/// 发送消息
/// @param payload 消息体
/// @notes 调用此接口 不会触发 gameManager:onSendMessageResult:messageId: 回调
- (VeBaseChannelMessage *)sendMessage:(NSString *)payload;

/// 发送消息
/// @param payload 消息体
/// @param channel_uid 消息通道ID（云端初始化 veMessageChannelClient 时传入的 Uid）
/// @notes 调用此接口 不会触发 gameManager:onSendMessageResult:messageId: 回调
- (VeBaseChannelMessage *)sendMessage:(NSString *)payload channel:(NSString *)channel_uid;

/// 发送消息
/// @param payload 消息体
/// @param time 超时时间，ms
/// @notes 调用此接口 会触发 gameManager:onSendMessageResult:messageId: 回调
- (VeBaseChannelMessage *)sendMessage:(NSString *)payload timeout:(NSInteger)time;

/// 发送消息
/// @param payload 消息体
/// @param time 超时时间，ms
/// @param channel_uid 消息通道ID（云端初始化 veMessageChannelClient 时传入的 Uid）
/// @notes 调用此接口 会触发 gameManager:onSendMessageResult:messageId: 回调
- (VeBaseChannelMessage *)sendMessage:(NSString *)payload timeout:(NSInteger)time channel:(NSString *)channel_uid;

/// 发送通用消息
/// @param message 消息体
- (void)sendGeneralMessage:(NSString *)message;

/// 剪贴板数据通道
/// @param dataArray 数据
- (void)sendClipBoardMessage:(NSArray<NSString *> *)dataArray;

/// 游戏过程中，切换视频流清晰度
/// @param streamProfileId 清晰度Id
- (void)switchVideoStreamProfile:(NSInteger)streamProfileId;

/// 游戏手柄数据通道
/// @param type 操作类型
/// @param data 操作数据
- (void)sendGamepadOperationType:(VeGamepadOperationType)type data:(NSString *)data;

/// 开启音频采集
- (void)startAudioStream;

/// 停止音频采集
- (void)stopAudioStream;

/// 调节音频采集音量，默认：100%，范围：[0, 100]，单位%
- (void)setLocalAudioCaptureVolume:(NSInteger)volume;

/// 调节远端音频播放音量，默认：100%，范围：[0, 100]，单位%
- (void)setRemoteAudioPlaybackVolume:(NSInteger)volume;

/// 获取本地音频音量，默认：100%，范围：[0, 100]，单位%
- (NSInteger)getLocalAudioPlaybackVolume;

/// 获取远端音频音量，默认：100%，范围：[0, 100]，单位%
- (NSInteger)getRemoteAudioPlaybackVolume;

/// 设置音频播放设备
- (void)setAudioRoute:(VeBaseAudioRoute)audioRoute;

/// 开始视频采集
- (void)startVideoStream:(VeBaseCameraId)cameraId;

/// 切换摄像头
/// @param cameraId 摄像头类型
- (void)switchCamera:(VeBaseCameraId)cameraId;

/// 停止视频采集
- (void)stopVideoStream;

/// 设置本地视频渲染时，使用的视图，默认渲染模式：VeGameRenderModeHidden
- (void)setLocalVideoCanvas:(UIView *)view;

/// 设置本地视频渲染时，使用的视图，并设置渲染模式
- (void)setLocalVideoCanvas:(UIView *)view renderMode:(VeBaseRenderMode)mode;

/// 设置镜像类型
- (void)setLocalVideoMirrorType:(VeBaseMirrorType)type;

/// 设置视频编码传输时的视频参数
- (void)setVideoEncoderConfig:(NSArray<VeBaseVideoSolution *> *)videoSolutions;

/// 设置切后台保活时长，默认：300s
- (void)setIdleTime:(NSInteger)time;

/// 设置挂机时长，默认：300s
- (void)setAutoRecycleTime:(NSInteger)time;

/// 获取游戏挂机时长
- (void)getAutoRecycleTime;

/// 设置“用户配置信息保存还原”路径
- (void)setUserProfilePathList:(NSArray<NSString *> *)pathList;

/// 获取“用户配置信息保存还原”路径
- (void)getUserProfilePathList;

/// 切换前后台
- (void)switchBackground:(BOOL)enable;

/// 切换远端游戏到前台
- (void)setRemoteGameForeground;

/// 修改目标用户的角色（只针对PC云游戏有效）
- (void)changeRole:(NSString *)destUid role:(VeGameRoleType)role;

/// 获取当前用户的角色（只针对PC云游戏有效）
- (VeGameRoleType)getCurrentRole;

/// 设置键盘开关
- (void)setKeyboardEnable:(BOOL)enable;

/// 查询当前用户键盘开关
- (BOOL)getKeyboardEnable;

/// 开始上传文件
/// - Parameters:
///   - file: 文件路径
///   - progress: 上传进度回调
///   - complete: 上传完成回调
///   - cancel: 取消上传回调
///   - error: 上传错误回调
- (void)startSendFile:(VeFile *)file
           onProgress:(void(^)(VeFile *file, NSInteger progress))progress
           onComplete:(void(^)(VeFile *file))complete
             onCancel:(void(^)(VeFile *file))cancel
              onError:(void(^)(VeFile *file, int error))error;

/// 终止传输任务
/// - Parameter file: 文件
- (void)stopSendFile;

/// 终止传输任务
/// - Parameter file: 文件
- (void)stopReceiveFile;

@end
