//
//  VeBaseDefines.h
//  VePlayer
//
//  Created by changwuguo on 2022/7/12.
//  Copyright © 2022 ByteDance Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef NS_ENUM(NSUInteger, VeBaseCameraId) {
    /** 前置摄像头 */
    VeBaseCameraIdFront = 0,
    /** 后置摄像头 */
    VeBaseCameraIdBack = 1,
};

typedef NS_ENUM(NSUInteger, VeBaseMirrorType) {
    /** 关闭本地镜像 */
    VeBaseMirrorTypeNone = 0,
    /** 开启本地镜像 */
    VeBaseMirrorTypeRender = 1,
};

typedef NS_ENUM(NSUInteger, VeBaseRemoteAppSwitchedType) {
    /** 主动切换成功 */
    VeBaseRemoteAppSwitchedTypeManualSucceed = 0,
    /** 自动切换成功 */
    VeBaseRemoteAppSwitchedTypeAutoSucceed = 1,
};

typedef NS_ENUM(NSUInteger, VeBaseNetProbeErrorCode) {
    /** 网络错误 */
    VeBaseNetProbeErrorCodeBadNetwork = 1,
    /** 用户取消探测 */
    VeBaseNetProbeErrorCodeCancelByUser = 2,
    /** 数据为空 */
    VeBaseNetProbeErrorCodeEmptyStats = 3,
};

typedef NS_ENUM(NSUInteger, VeBaseNetProbeQuality) {
    /** 极致 */
    VeBaseNetProbeQualityExcellent = 1,
    /** 一般 */
    VeBaseNetProbeQualityGood = 2,
    /** 糟糕 */
    VeBaseNetProbeQualityPoor = 3,
};

typedef NS_ENUM(NSInteger, VeBaseNetworkType) {
    /** 网络连接类型未知 */
    VeBaseNetworkTypeUnknown = -1,
    /** 网络连接已断开 */
    VeBaseNetworkTypeDisconnected = 0,
    /** 网络连接类型为：LAN */
    VeBaseNetworkTypeLAN = 1,
    /** 网络连接类型为：Wi-Fi（包含热点）*/
    VeBaseNetworkTypeWIFI = 2,
    /** 网络连接类型为：2G移动网络 */
    VeBaseNetworkTypeMobile2G = 3,
    /** 网络连接类型为：3G移动网络 */
    VeBaseNetworkTypeMobile3G = 4,
    /** 网络连接类型为：4G移动网络 */
    VeBaseNetworkTypeMobile4G = 5,
    /** 网络连接类型为：5G移动网络 */
    VeBaseNetworkTypeMobile5G = 6,
};

typedef NS_ENUM(NSUInteger, VeBaseConnectionState) {
    /** 连接中断 */
    VeBaseConnectionStateDisconnected = 1,
    /** 连接建立中 */
    VeBaseConnectionStateConnecting = 2,
    /** 连接已建立 */
    VeBaseConnectionStateConnected = 3,
    /** 连接重连中 */
    VeBaseConnectionStateReconnecting = 4,
    /** 连接已重连 */
    VeBaseConnectionStateReconnected = 5,
    /** 网络连接断开超过 10 秒，仍然会继续重连 */
    VeBaseConnectionStateLost = 6,
    /** 连接失败，服务端状态异常 */
    VeBaseConnectionStateFailed = 7,
};

typedef NS_ENUM(NSUInteger, VeBaseLocalAudioStreamState) {
    /** 本地音频默认初始状态 */
    VeBaseLocalAudioStreamStateStopped = 0,
    /** 本地音频录制设备启动成功 */
    VeBaseLocalAudioStreamStateRecording = 1,
    /** 本地音频首帧编码成功 */
    VeBaseLocalAudioStreamStateEncoding = 2,
    /** 本地音频启动失败 */
    VeBaseLocalAudioStreamStateFailed = 3,
};

typedef NS_ENUM(NSUInteger, VeBaseLocalAudioStreamError) {
    /** 本地音频状态正常 */
    VeBaseLocalAudioStreamErrorOk = 0,
    /** 本地音频出错原因未知 */
    VeBaseLocalAudioStreamErrorFailure = 1,
    /** 没有权限启动本地音频录制设备 */
    VeBaseLocalAudioStreamErrorDeviceNoPermission = 2,
    /** 本地音频录制设备已经在使用中（该错误码暂未使用） */
    VeBaseLocalAudioStreamErrorDeviceBusy = 3,
    /** 本地音频录制失败，建议你检查录制设备是否正常工作 */
    VeBaseLocalAudioStreamErrorRecordFailure = 4,
    /** 本地音频编码失败 */
    VeBaseLocalAudioStreamErrorEncodeFailure = 5,
    /** 没有可用的音频录制设备 */
    VeBaseLocalAudioStreamErrorNoRecordingDevice = 6
};

typedef NS_ENUM(NSUInteger, VeBaseRemoteAudioStreamState) {
    /** 远端音频流默认初始状态 */
    VeBaseRemoteAudioStreamStateStopped = 0,
    /** 开始接收远端音频流首包 */
    VeBaseRemoteAudioStreamStateStarting = 1,
    /** 远端音频流正在解码，正常播放 */
    VeBaseRemoteAudioStreamStateDecoding = 2,
    /** 远端音频流卡顿 */
    VeBaseRemoteAudioStreamStateFrozen = 3,
};

typedef NS_ENUM(NSUInteger, VeBaseRemoteAudioStreamStateChangeReason) {
    /** 内部原因 */
    VeBaseRemoteAudioStreamStateChangeReasonInternal = 0,
    /** 网络阻塞 */
    VeBaseRemoteAudioStreamStateChangeReasonNetworkCongestion = 1,
    /** 网络恢复正常 */
    VeBaseRemoteAudioStreamStateChangeReasonNetworkRecovery = 2,
    /** 本地用户停止接收远端音频流 */
    VeBaseRemoteAudioStreamStateChangeReasonLocalMuted = 3,
    /** 本地用户恢复接收远端音频流 */
    VeBaseRemoteAudioStreamStateChangeReasonLocalUnmuted = 4,
    /** 远端用户停止发送音频流 */
    VeBaseRemoteAudioStreamStateChangeReasonRemoteMuted = 5,
    /** 远端用户恢复发送音频流 */
    VeBaseRemoteAudioStreamStateChangeReasonRemoteUnmuted = 6,
    /** 远端用户离开房间 */
    VeBaseRemoteAudioStreamStateChangeReasonRemoteOffline = 7,
};

typedef NS_ENUM(NSUInteger, VeBaseAudioRoute) {
    /** 有线耳机 */
    VeBaseAudioRouteHeadset = 1,
    /** 听筒。设备自带的，一般用于通话的播放硬件 */
    VeBaseAudioRouteEarpiece = 2,
    /** 扬声器。设备自带的，一般用于免提播放的硬件 */
    VeBaseAudioRouteSpeakerphone = 3,
    /** 蓝牙耳机 */
    VeBaseAudioRouteHeadsetBluetooth = 4,
    /** USB设备 */
    VeBaseAudioRouteHeadsetUSB = 5,
};

typedef NS_ENUM(NSUInteger, VeBaseRenderMode) {
    /** 视窗填满优先（视频帧等比缩放，直至视窗被视频填满）*/
    VeBaseRenderModeHidden = 1,
    /** 视频帧内容全部显示优先（视频帧等比缩放，直至视频帧能够在视窗上全部显示）*/
    VeBaseRenderModeFit = 2,
    /** 视频帧自适应视窗（视频帧非等比缩放，直至视窗被视频填满）*/
    VeBaseRenderModeAdaptive = 3,
};

typedef NS_ENUM(NSUInteger, VeBaseVideoEncoderPreference) {
    /** 无偏好 */
    VeBaseVideoEncoderPreferenceDisabled = 0,
    /** 帧率优先 */
    VeBaseVideoEncoderPreferenceMaintainFramerate = 1,
    /** 质量优先 */
    VeBaseVideoEncoderPreferenceMaintainQuality = 2,
    /** 平衡质量与帧率 */
    VeBaseVideoEncoderPreferenceBalance = 3,
};

typedef NS_ENUM(NSUInteger, VeBaseRemoteVideoStreamState) {
    /** 远端视频流默认初始状态 */
    VeBaseRemoteVideoStreamStateStopped = 0,
    /** 本地用户已接收远端视频首包 */
    VeBaseRemoteVideoStreamStateStarting = 1,
    /** 远端视频流正在解码，正常播放 */
    VeBaseRemoteVideoStreamStateDecoding = 2,
    /** 远端视频流卡顿 */
    VeBaseRemoteVideoStreamStateFrozen = 3,
    /** 远端音频流播放失败 */
    VeBaseRemoteVideoStreamStateFailed = 4,
};

typedef NS_ENUM(NSUInteger, VeBaseRemoteVideoStreamStateChangeReason) {
    /** 内部原因 */
    VeBaseRemoteVideoStreamStateChangeReasonInternal = 0,
    /** 网络阻塞 */
    VeBaseRemoteVideoStreamStateChangeReasonNetworkCongestion = 1,
    /** 网络恢复正常 */
    VeBaseRemoteVideoStreamStateChangeReasonNetworkRecovery = 2,
    /** 本地用户停止接收远端视频流或本地用户禁用视频模块 */
    VeBaseRemoteVideoStreamStateChangeReasonLocalMuted = 3,
    /** 本地用户恢复接收远端视频流或本地用户启用视频模块 */
    VeBaseRemoteVideoStreamStateChangeReasonLocalUnmuted = 4,
    /** 远端用户停止发送视频流或远端用户禁用视频模块 */
    VeBaseRemoteVideoStreamStateChangeReasonRemoteMuted = 5,
    /** 远端用户恢复发送视频流或远端用户启用视频模块 */
    VeBaseRemoteVideoStreamStateChangeReasonRemoteUnmuted = 6,
    /** 远端用户离开房间 */
    VeBaseRemoteVideoStreamStateChangeReasonRemoteOffline = 7,
};

typedef NS_ENUM(NSUInteger, VeBaseLocalVideoStreamState) {
    /** 本地视频默认初始状态 */
    VeBaseLocalVideoStreamStateStopped = 0,
    /** 本地视频录制设备启动成功 */
    VeBaseLocalVideoStreamStateRecording = 1,
    /** 本地视频首帧编码成功 */
    VeBaseLocalVideoStreamEncoding = 2,
    /** 本地视频启动失败 */
    VeBaseLocalVideoStreamStateFailed = 3,
};

typedef NS_ENUM(NSUInteger, VeBaseLocalVideoStreamError) {
    /** 本地视频状态正常 */
    VeBaseLocalVideoStreamErrorOk = 0,
    /** 本地视频流发布失败 */
    VeBaseLocalVideoStreamErrorFailure = 1,
    /** 没有权限启动本地视频采集设备 */
    VeBaseLocalVideoStreamErrorDeviceNoPermission = 2,
    /** 本地视频采集设备被占用 */
    VeBaseLocalVideoStreamErrorDeviceBusy = 3,
    /** 本地视频采集设备不存在或已移除 */
    VeBaseLocalVideoStreamErrorDeviceNotFound = 4,
    /** 本地视频采集失败，建议检查采集设备是否正常工作 */
    VeBaseLocalVideoStreamErrorCaptureFailure = 5,
    /** 本地视频编码失败 */
    VeBaseLocalVideoStreamErrorEncodeFailure = 6,
};

typedef NS_ENUM(NSUInteger, VeBaseNetworkQuality) {
    /** 网络质量未知 */
    VeBaseNetworkQualityUnknown = 0,
    /** 网络质量极好 */
    VeBaseNetworkQualityExcellent = 1,
    /** 主观感觉和 VeBaseNetworkQualityExcellent 差不多，但码率可能略低 */
    VeBaseNetworkQualityGood = 2,
    /** 主观感受有瑕疵但不影响沟通 */
    VeBaseNetworkQualityPoor = 3,
    /** 勉强能使用但不顺畅 */
    VeBaseNetworkQualityBad = 4,
    /** 网络质量非常差，基本不能沟通 */
    VeBaseNetworkQualityVeryBad = 5,
};

typedef NS_ENUM(NSUInteger, VeBaseMediaSourceType) {
    /** 外部采集 */
    VeBaseMediaSourceTypeExternal = 0,
    /** 内部采集 */
    VeBaseMediaSourceTypeInternal,
};

typedef NS_ENUM(NSInteger, VeBaseAudioChannel) {
    /** 默认设置 */
    VeBaseAudioChannelAuto = -1,
    /** 单声道 */
    VeBaseAudioChannelMono = 1,
    /** 双声道 */
    VeBaseAudioChannelStereo = 2,
};

typedef NS_ENUM(NSInteger, VeBaseAudioSampleRate) {
    /** 默认设置 */
    VeBaseAudioSampleRateAuto = -1,
    /** 8000 Hz */
    VeBaseAudioSampleRate8000 = 8000,
    /** 16000 Hz */
    VeBaseAudioSampleRate16000 = 16000,
    /** 32000 Hz */
    VeBaseAudioSampleRate32000 = 32000,
    /** 44100 Hz */
    VeBaseAudioSampleRate44100 = 44100,
    /** 48000 Hz */
    VeBaseAudioSampleRate48000 = 48000
};

typedef NS_ENUM(NSInteger, VeBaseVideoRotation) {
    /** 视频不做旋转 */
    VeBaseVideoRotation0 = 0,
    /** 视频顺时针旋转 90 度 */
    VeBaseVideoRotation90 = 90,
    /** 视频顺时针旋转 180 度 */
    VeBaseVideoRotation180 = 180,
    /** 视频顺时针旋转 270 度 */
    VeBaseVideoRotation270 = 270,
};

@interface VeBaseChannelMessage : NSObject
/** 消息Id */
@property (nonatomic, copy) NSString *mid;
/** 消息内容 */
@property (nonatomic, copy) NSString *payload;
/** 发送消息时间 */
@property (nonatomic, copy) NSString *time;
/** 发送方的 Uid */
@property (nonatomic, copy) NSString *srcUid;
/** 接收方的 Uid */
@property (nonatomic, copy) NSString *destUid;

@end

@interface VeBaseVideoSolution : NSObject
/** 视频宽度，单位：px */
@property(nonatomic, assign) NSInteger width;
/** 视频高度，单位：px */
@property(nonatomic, assign) NSInteger height;
/** 视频帧率，单位：fps */
@property(nonatomic, assign) NSInteger frameRate;
/** 最大编码码率，单位：kbps */
@property(nonatomic, assign) NSInteger maxBitrate;
/** 视频编码质量策略 */
@property (nonatomic, assign) VeBaseVideoEncoderPreference encoderPreference;

@end

// 本地流数据统计
@interface VeBaseLocalStreamStats : NSObject
/** 音频往返时延，ms */
@property (nonatomic, assign) NSInteger audioRtt;
/** 音频发送的码率，kbps */
@property (nonatomic, assign) CGFloat audioSentKBitrate;
/** 音频丢包率，0 ~ 1 */
@property (nonatomic, assign) CGFloat audioLossRate;
/** 音频声道数 */
@property (nonatomic, assign) NSInteger numChannels;
/** 音频发送采样率，Hz */
@property (nonatomic, assign) NSInteger sentSampleRate;
/** 音频采集采样率，Hz */
@property (nonatomic, assign) NSInteger recordSampleRate;
/** 视频发送码率，kbps */
@property (nonatomic, assign) CGFloat videoSentKBitrate;
/** 视频采集帧率，fps */
@property (nonatomic, assign) NSInteger inputFrameRate;
/** 视频发送帧率，fps */
@property (nonatomic, assign) NSInteger sentFrameRate;
/** 视频编码器输出帧率，fps */
@property (nonatomic, assign) NSInteger encoderOutputFrameRate;
/** 视频本地渲染帧率，fps */
@property (nonatomic, assign) NSInteger rendererOutputFrameRate;
/** 视频丢包率，0 ~ 1 */
@property (nonatomic, assign) CGFloat videoLossRate;
/** 视频往返时延，ms */
@property (nonatomic, assign) NSInteger videoRtt;
/** 视频编码码率，kbps */
@property (nonatomic, assign) NSInteger encodedBitrate;
/** 视频编码宽度，px */
@property (nonatomic, assign) NSInteger encodedFrameWidth;
/** 视频编码高度，px */
@property (nonatomic, assign) NSInteger encodedFrameHeight;
/** 视频帧总数 */
@property (nonatomic, assign) NSInteger encodedFrameCount;

@end

// 远端流数据统计
@interface VeBaseRemoteStreamStats : NSObject
/** 远端视频宽度 */
@property (nonatomic, assign) NSInteger width;
/** 远端视频高度 */
@property (nonatomic, assign) NSInteger height;
/** 音频往返时延，ms */
@property (nonatomic, assign) NSInteger audioRtt;
/** 视频往返时延，ms */
@property (nonatomic, assign) NSInteger videoRtt;
/** 远端音频丢包率 */
@property (nonatomic, assign) CGFloat audioLossRate;
/** 远端视频丢包率 */
@property (nonatomic, assign) CGFloat videoLossRate;
/** 远端音频卡顿次数 */
@property (nonatomic, assign) NSInteger audioStallCount;
/** 远端视频卡顿次数 */
@property (nonatomic, assign) NSInteger videoStallCount;
/** 远端音频卡顿时长，ms */
@property (nonatomic, assign) NSInteger audioStallDuration;
/** 远端视频卡顿时长，ms */
@property (nonatomic, assign) NSInteger videoStallDuration;
/** 远端音频接收码率 */
@property (nonatomic, assign) NSInteger receivedAudioKBitrate;
/** 远端视频接收码率 */
@property (nonatomic, assign) NSInteger receivedVideoKBitrate;
/** 远端视频解码输出帧率 */
@property (nonatomic, assign) NSInteger decoderOutputFrameRate;
/** 远端视频渲染输出帧率 */
@property (nonatomic, assign) NSInteger rendererOutputFrameRate;
/** 音频声道数 */
@property (nonatomic, assign) NSInteger numChannels;
/** 播放采样率 */
@property (nonatomic, assign) NSInteger playoutSampleRate;

@end

@interface VeBaseAudioFrame : NSObject
/** PCM 数据 */
@property(nonatomic, strong) NSData *buffer;
/** 采样点个数 */
@property(nonatomic, assign) int samples;
/** 音频声道 */
@property(nonatomic, assign) VeBaseAudioChannel channel;
/** 采样率 */
@property(nonatomic, assign) VeBaseAudioSampleRate sampleRate;

@end

@interface VeFile : NSObject

@property (nonatomic, copy) NSString *name;
@property (nonatomic, strong) NSData *fileData;
@property (nonatomic, copy) NSString *md5;

@end
