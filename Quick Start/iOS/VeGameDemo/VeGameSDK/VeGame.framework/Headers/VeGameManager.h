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

#pragma mark - “核心”代理回调
/// 启动成功回调
/// @param manager VeGameManager 对象
/// @param gameId 游戏 ID
/// @param streamProfileId 清晰度 ID
/// @param reservedId 预定 ID
/// @param extra 自定义信息
- (void)gameManager:(VeGameManager *)manager startSucceedResult:(NSString *)gameId videoStreamProfileId:(NSInteger)streamProfileId reservedId:(NSString *)reservedId extra:(NSDictionary *)extra;

/// 远端“首帧音频”回调
/// @param manager VeGameManager 对象
- (void)firstRemoteAudioFrameArrivedFromEngineManager:(VeGameManager *)manager;

/// 远端“横竖旋转”回调
/// @param manager VeGameManager 对象
/// @param rotation 旋转度
- (void)gameManager:(VeGameManager *)manager changedDeviceRotation:(NSInteger)rotation;

/// 本地“操作延迟”回调
/// @param manager VeGameManager 对象
/// @param delayTime 时间 ms
- (void)gameManager:(VeGameManager *)manager operationDelay:(NSInteger)delayTime;

/// 远端“切换分辨率”结果回调
/// @param manager VeGameManager 对象
/// @param code 错误码（成功：0 失败：other）
/// @param index1 from分辨率
/// @param index2 current分辨率
- (void)gameManager:(VeGameManager *)manager switchVideoStreamProfileWithCode:(NSInteger)code fromIndex:(NSInteger)index1 toIndex:(NSInteger)index2;

/// 本地“网络类型改变”回调
/// @param manager VeGameManager 对象
/// @param networkType 变更后的网络类型
- (void)gameManager:(VeGameManager *)manager networkTypeChangedToType:(VeBaseNetworkType)networkType;

/// 本地与信令服务器连接状态回调
/// @param manager VeGameManager 对象
/// @param state 当前信令服务器连接状态
- (void)gameManager:(VeGameManager *)manager connectionChangedToState:(VeBaseConnectionState)state;

#pragma mark - “数据通道”代理回调
/// 远端“MessageChannel”连接状态回调
/// @param manager VeGameManager 对象
/// @param online 是否在线
- (void)gameManager:(VeGameManager *)manager onRemoteMessageChannelState:(BOOL)online;

/// 远端“MessageChannel”消息回调
/// @param manager VeGameManager 对象
/// @param msgId 消息id
/// @param result 发送结果
- (void)gameManager:(VeGameManager *)manager onRemoteMessageChannelWithMsgId:(NSString *)msgId onResult:(BOOL)result;

/// 远端“剪贴板数据”回调
/// @param manager VeGameManager 对象
/// @param datArray 数据
- (void)gameManager:(VeGameManager *)manager receivedClipBoardMessage:(NSArray *)datArray;

/// 远端“通用消息”回调
/// @param manager VeGameManager 对象
/// @param dataDict 消息体
- (void)gameManager:(VeGameManager *)manager receivedGeneralMessage:(NSDictionary *)dataDict;

#pragma mark - “音频采集”代理回调
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

/// 获取“本地音频采集音量”回调
/// @param manager VeGameManager 对象
/// @param volume 音量，范围：[0, 100]，单位%
- (void)gameManager:(VeGameManager *)manager getLocalCaptureAudioVolume:(NSInteger)volume;

/// 获取“远端音频播放音量”回调
/// @param manager VeGameManager 对象
/// @param volume 音量，范围：[0, 100]，单位%
- (void)gameManager:(VeGameManager *)manager getRemoteAudioPlaybackVolume:(NSInteger)volume;

/// 本地“音频采集设备启动成功”回调
/// @param manager VeGameManager 对象
/// @param success 是否成功
- (void)gameManager:(VeGameManager *)manager onAudioCaptureDeviceStartState:(BOOL)success;

/// 本地“音频播放设备变化”回调
/// @param manager VeGameManager 对象
/// @param audioRoute 设备类型
- (void)gameManager:(VeGameManager *)manager onAudioRouteChanged:(VeBaseAudioRoute)audioRoute;

#pragma mark - “视频采集”代理回调
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

#pragma mark - 设置“挂机时长”代理回调
/// 本地设置“挂机时长”的结果回调
/// @param manager VeGameManager 对象
/// @param code 错误码，0：success -1：fail
/// @param time 当前挂机时长
- (void)gameManager:(VeGameManager *)manager setAutoRecycleTimeCallback:(NSInteger)code time:(NSInteger)time;

/// 本地获取“挂机时长”回调
/// @param manager VeGameManager 对象
/// @param code 错误码，0：success -1：fail
/// @param time 当前挂机时长
- (void)gameManager:(VeGameManager *)manager getAutoRecycleTimeCallback:(NSInteger)code time:(NSInteger)time;

#pragma mark - 切换“APP前后台”代理回调
/// 本地“切换前后台”回调
/// @param manager VeGameManager 对象
/// @param result 结果
- (void)gameManager:(VeGameManager *)manager onBackgroundSwitched:(BOOL)result;

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

#pragma mark - 设置“用户配置信息保存还原路径”代理回调
/// 设置“用户配置信息保存还原路径”结果回调（调用“setUserProfilePathList:”触发）
/// @param manager VeGameManager 对象
/// @param type 0：设置 1：还原
/// @param result 结果
- (void)gameManager:(VeGameManager *)manager setUserProfilePathType:(NSInteger)type result:(BOOL)result;

/// 获取“用户配置信息保存还原路径”回调（调用“getUserProfilePathList”触发）
/// @param manager VeGameManager 对象
/// @param list 配置列表
- (void)gameManager:(VeGameManager *)manager getUserProfilePathList:(NSArray<NSString *> *)list;

#pragma mark - 媒体流“统计数据”代理回调
/// 本地“流数据统计”回调
/// @param manager VeGameManager 对象
/// @param stats 统计数据
- (void)gameManager:(VeGameManager *)manager onLocalStreamStats:(VeBaseLocalStreamStats *)stats;

/// 远端“流数据统计”回调
/// @param manager VeGameManager 对象
/// @param stats 统计数据
- (void)gameManager:(VeGameManager *)manager onRemoteStreamStats:(VeBaseRemoteStreamStats *)stats;

#pragma mark - “警告及错误”代理回调
/// SDK发生警告回调
/// @param manager VeGameManager 对象
/// @param code 错误码
- (void)gameManager:(VeGameManager *)manager onWarning:(VeGameWarningCode)code;

/// SDK发生错误回调
/// @param manager VeGameManager 对象
/// @param code 错误码
- (void)gameManager:(VeGameManager *)manager onError:(VeGameErrorCode)code;

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
/// 扩展字段
@property (nonatomic, strong) NSDictionary *extra;

/// 设置配置信息，建议在程序一启动的时候就调用
+ (void)setConfigInfo;

/// 创建 VeGameManager “单例”对象
/// @param containerView 自定义渲染视图
/// @param delegate 代理对象
+ (instancetype)sharedManagerWithContainerView:(UIView *)containerView delegate:(NSObject<VeGameManagerDelegate> *)delegate;

/// 获取“单例”对象
+ (VeGameManager *)sharedInstance;

/// 版本号
+ (NSString *)currentVersion;

/// 当前设备ID
+ (NSString *)currentDeviceId;

/// 当前服务端设备ID
+ (NSString *)getServiceDeviceId;

/// 销毁单例，释放所有资源
+ (void)destroy;

/// 启用后，在 Debug 模式下会将日志输出到控制台，默认：NO
- (void)consoleLogEnable:(BOOL)enable;

/// 启动接口
/// @param configObj 配置参数
- (void)startWithConfig:(VeGameConfigObject *)configObj;

/// 停止退出
- (void)stop;

/// 重启游戏
- (void)restartGame;

/// 静音开关
- (void)muteAudio:(BOOL)on;

/// 暂停开关
- (void)switchPaused:(BOOL)on;

/// 自定义操作视图
- (void)setCustomOperationView:(UIView *)customOperationView;

/// 发送消息
/// @param payload 消息体
- (void)sendMessage:(NSString *)payload;

/// 发送消息
/// @param payload 消息体
/// @param time 超时时间，ms
- (void)sendMessage:(NSString *)payload timeout:(NSInteger)time;

/// 发送通用消息
/// @param message 消息体
- (void)sendGeneralMessage:(NSString *)message;

/// 剪贴板数据通道
/// @param dataArray 数据
- (void)sendClipBoardMessage:(NSArray<NSString *> *)dataArray;

/// 切换视频流清晰度
/// @param streamProfileId 清晰度Id
- (void)switchVideoStreamProfile:(NSInteger)streamProfileId;

/// 游戏手柄数据通道
/// @param type 操作类型
/// @param json 操作数据
- (void)sendGamepadOperationType:(VeGamepadOperationType)type data:(NSString *)json;

/// 更新鉴权信息
- (void)updateAk:(NSString *)ak sk:(NSString *)sk token:(NSString *)token;

/// 开启音频采集
- (void)startAudioStream;

/// 停止音频采集
- (void)stopAudioStream;

/// 调节音频采集音量，默认：100%，范围：[0, 100]，单位%
- (void)setLocalAudioCaptureVolume:(NSInteger)volume;

/// 调节本地音频播放音量，默认：100%，范围：[0, 100]，单位%
- (void)setLocalAudioPlaybackVolume:(NSInteger)volume;

/// 调节远端音频播放音量，默认：100%，范围：[0, 100]，单位%
- (void)setRemoteAudioPlaybackVolume:(NSInteger)volume;

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

/// 设置游戏无操作保活时长，默认：300s
- (void)setIdleTime:(NSInteger)time;

/// 设置游戏挂机时长，默认：300s
- (void)setAutoRecycleTime:(NSInteger)time;

/// 获取游戏挂机时长
- (void)getAutoRecycleTime;

/// 设置“用户配置信息保存还原”路径
- (void)setUserProfilePathList:(NSArray<NSString *> *)patchList;

/// 获取“用户配置信息保存还原”路径
- (void)getUserProfilePathList;

/// 切换前后台
- (void)switchBackground:(BOOL)enable;

/// 切换远端游戏到前台
- (void)setRemoteGameForeground;

@end
