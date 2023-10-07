//
//  VeBaseDefines.h
//  VePlayer
//
//  Created by changwuguo on 2022/7/12.
//  Copyright © 2022 ByteDance Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef NS_ENUM(NSUInteger, VeBaseVideoRenderMode) {
    /** 等比缩放居中模式；
     *  画面等比缩放居中显示于 Container，当推流分辨率比例与 Container 比例一致时效果为全屏显示；
     *  当推流分辨率与 Container 比例不一致时效果为一个方向占满画布，另一个方向在两侧会有同等比例黑边；
     *  无画面内容丢失；
     */
    VeBaseVideoRenderModeFit = 0,
    /** 非等比拉伸模式；
     *  非等比拉伸画面并充满整个 Container；
     *  无画面内容丢失；
     */
    VeBaseVideoRenderModeFill = 1,
    /** 等比例缩放模式；
     *  画面短边撑满容器，长边被裁切；
     *  有画面内容丢失；
     */
    VeBaseVideoRenderModeCover = 2
};

typedef NS_ENUM(NSUInteger, VeBaseRotationMode) {
    /** 竖屏 */
    VeBaseRotationModePortrait = 0,
    /** 横屏 */
    VeBaseRotationModeLandscape = 1,
    /** 自适应 */
    VeBaseRotationModeAutoRotation = 2,
};

typedef NS_ENUM(NSUInteger, VeBaseRoleType) {
    /** 观看者 */
    VeBaseRoleTypeViewer = 0,
    /** 操作者 */
    VeBaseRoleTypePlayer = 1,
};

typedef NS_ENUM(NSUInteger, VeBaseRoomType) {
    /** 单人控制单人读 */
    VeBaseRoomTypeDefault = 0,
    /** 单人控制多人读，控制权不能转移 */
    VeBaseRoomTypeNonTransferable = 1,
    /** 单人控制多人读，控制权可以转移 */
    VeBaseRoomTypeControlTransferable = 2,
};

typedef NS_ENUM(NSUInteger, VeBaseStreamType) {
    /** 订阅音视频 */
    VeBaseStreamTypeBoth = 0,
    /** 订阅音频 */
    VeBaseStreamTypeAudio = 1,
    /** 订阅视频 */
    VeBaseStreamTypeVideo = 2,
};

typedef NS_ENUM(NSUInteger, VeBaseSessionMode) {
    /** 正常模式 */
    VeBaseSessionModeNormal = 0,
    /** 挂机模式 */
    VeBaseSessionModeHangUp = 1,
};

typedef NS_ENUM(NSUInteger, VeBaseLocationMode) {
    /** 自动模式 */
    VeBaseLocationModeAuto = 0,
    /** 手动模式 */
    VeBaseLocationModeManual = 1,
};

typedef NS_ENUM(NSUInteger, VeBaseMediaSourceType) {
    /** SDK内部采集 */
    VeBaseMediaSourceTypeInternal = 0,
    /** 自定义采集 */
    VeBaseMediaSourceTypeExternal = 1,
};

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
    /** 视窗填满优先（视频帧等比缩放，直至视窗被视频填满，多出的视频将被截掉）*/
    VeBaseRenderModeHidden = 1,
    /** 视频帧内容全部显示优先（视频帧等比缩放，直至视频帧能够在视窗上全部显示，会有黑边）*/
    VeBaseRenderModeFit = 2,
    /** 视频帧自适应视窗（视频帧非等比缩放，直至视窗被视频填满，视频帧长宽比例可能会发生变化）*/
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
    /** 网络断开，无法使用 */
    VeBaseNetworkQualityDown = 6
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

typedef NS_ENUM(NSUInteger, VeBaseRecordingStatus) {
    /** 停止录制成功，录像文件生成成功并返回下载地址 */
    VeBaseRecordingStatusStopSuccessWithDownloadUrl = 0,
    /** 停止录制成功，录像文件生成成功 */
    VeBaseRecordingStatusStopSuccess = 1,
    /** 开始录制成功 */
    VeBaseRecordingStatusStartSuccess = 2,
    /** 失败，正在录制中时调用了开始录制 */
    VeBaseRecordingStatusStartFailure = 3,
    /** 失败，未开始录制时调用了停止录制 */
    VeBaseRecordingStatusStopFailure = 4,
    /** 存储空间不足 */
    VeBaseRecordingStatusNoSpace = 5,
    /** 录制时长到达上限，停止录制，录像文件生成成功 */
    VeBaseRecordingStatusReachLimit = 6,
    /** 设置录制时长超过上限（4小时） */
    VeBaseRecordingStatusLimitExceeded = 7,
    /** 其它原因 */
    VeBaseRecordingStatusOthers = 8
};

typedef NS_ENUM(NSUInteger, VeBaseMediaDeviceState) {
    /** 未知 */
    VeBaseMediaDeviceStateUnknown = 0,
    /** 设备已开启 */
    VeBaseMediaDeviceStateStarted = 1,
    /** 设备已停止 */
    VeBaseMediaDeviceStateStopped = 2,
    /** 设备运行时错误 */
    VeBaseMediaDeviceStateRuntimeError = 3,
    /** 设备已插入 */
    VeBaseMediaDeviceStateAdded = 4,
    /** 设备被移除 */
    VeBaseMediaDeviceStateRemoved = 5,
    /** 系统通话/锁屏/第三方应用打断了音视频通话 */
    VeBaseMediaDeviceStateInterruptionBegan = 6,
    /** 音视频通话已从系统电话/锁屏/第三方应用打断中恢复 */
    VeBaseMediaDeviceStateInterruptionEnded = 7,
    /** 本地音视频采集设备启动成功 */
    VeBaseMediaDeviceStateRecording = 8,
    /** 本地音视频采集后，首帧编码成功 */
    VeBaseMediaDeviceStateEncoding = 9,
    /** 本地音视频启动失败 */
    VeBaseMediaDeviceStateFailed = 10,
};

typedef NS_ENUM(NSInteger, VeBaseMediaDeviceError) {
    /** 未知 */
    VeBaseMediaDeviceErrorUnknown = 0,
    /** 媒体设备正常 */
    VeBaseMediaDeviceErrorOK = 1,
    /** 没有权限启动媒体设备 */
    VeBaseMediaDeviceErrorDeviceNoPermission = 2,
    /** 媒体设备已经在使用中 */
    VeBaseMediaDeviceErrorDeviceBusy = 3,
    /** 媒体设备错误 */
    VeBaseMediaDeviceErrorDeviceFailure = 4,
    /** 未找到指定的媒体设备 */
    VeBaseMediaDeviceErrorDeviceNotFound = 5,
    /** 媒体设备被移除 */
    VeBaseMediaDeviceErrorDeviceDisconnected = 6,
    /** 无采集数据 */
    VeBaseMediaDeviceErrorDeviceNoCallback = 7,
    /** 设备采样率不支持 */
    VeBaseMediaDeviceErrorUNSupportFormat = 8,
    /** iOS屏幕采集没有 GroupId 参数 */
    VeBaseMediaDeviceErrorNotFindGroupId = 9,
    /** 视频采集中断：因用户使用系统相机，应用切换到后台运行，导致采集中断 */
    VeBaseMediaDeviceErrorNotAvailableInBackground = 10,
    /** 视频采集中断：可能由于其他应用占用系统相机，导致视频设备暂时不可用，从而造成采集中断 */
    VeBaseMediaDeviceErrorVideoInUseByAnotherClient = 11,
    /** 视频采集中断：当前应用处于侧拉、分屏或者画中画模式时，导致采集中断 */
    VeBaseMediaDeviceErrorNotAvailableWithMultipleForegroundApps = 12,
    /** 视频采集中断：由于系统性能不足导致中断，比如设备过热 */
    VeBaseMediaDeviceErrorNotAvailableDueToSystemPressure = 13,
    /** 本地音视频流发布失败 */
    VeBaseMediaDeviceErrorFailure = 14,
    /** 本地音频录制失败，建议你检查录制设备是否正常工作 */
    VeBaseMediaDeviceErrorAudioRecordFailure = 15,
    /** 本地音频编码失败 */
    VeBaseMediaDeviceErrorAudioEncodeFailure = 16,
    /** 没有可用的音频录制设备 */
    VeBaseMediaDeviceErrorAudioNoRecordingDevice = 17,
    /** 本地视频采集设备不存在或已移除 */
    VeBaseMediaDeviceErrorVideoDeviceNotFound = 18,
    /** 本地视频采集失败，建议检查采集设备是否正常工作 */
    VeBaseMediaDeviceErrorVideoCaptureFailure = 19,
    /** 本地视频编码失败 */
    VeBaseMediaDeviceErrorVideoEncodeFailure = 20,
    /** 通话过程中本地视频采集设备被其他程序抢占，导致设备连接中断 */
    VeBaseMediaDeviceErrorVideoDeviceDisconnected = 21,
};

/** ------------ 定位参数 ------------- */
@interface VeBaseLocationInfo : NSObject
/**
 * 纬度
 */
@property (nonatomic, assign) double latitude;
/**
 * 经度
 */
@property (nonatomic, assign) double longitude;

@end

/** ------ MessageChannel 消息参数 ------ */
@interface VeBaseChannelMessage : NSObject
/**
 * 消息Id
 */
@property (nonatomic, copy) NSString *mid;
/**
 * 消息内容
 */
@property (nonatomic, copy) NSString *payload;
/**
 * 发送消息的时间戳，ms
 */
@property (nonatomic, copy) NSString *time;
/**
 * 发送方Uid
 */
@property (nonatomic, copy) NSString *srcUid;
/**
 * 接收方Uid
 */
@property (nonatomic, copy) NSString *destUid;

@end

/** ----------- 视频编码参数 ------------ */
@interface VeBaseVideoSolution : NSObject
/**
 * 视频宽度
 * 单位：px
 */
@property (nonatomic, assign) NSInteger width;
/**
 * 视频高度
 * 单位：px
 */
@property (nonatomic, assign) NSInteger height;
/**
 * 视频帧率
 * 单位：fps
 */
@property (nonatomic, assign) NSInteger frameRate;
/**
 * 最小编码码率
 * 单位：kbps
 */
@property (nonatomic, assign) NSInteger minBitrate;
/**
 * 最大编码码率
 * 单位：kbps
 */
@property (nonatomic, assign) NSInteger maxBitrate;
/**
 * 视频编码质量策略
 */
@property (nonatomic, assign) VeBaseVideoEncoderPreference encoderPreference;

@end

/** ------------ 本地流数据统计 ------------ */
@interface VeBaseLocalStreamStats : NSObject
/**
 * 音频往返时延
 * 单位：ms
 */
@property (nonatomic, assign) NSInteger audioRtt;
/**
 * 音频发送的码率
 * 单位：kbps
 */
@property (nonatomic, assign) CGFloat audioSentKBitrate;
/**
 * 音频丢包率
 * 范围：0 ~ 1
 */
@property (nonatomic, assign) CGFloat audioLossRate;
/**
 * 音频声道数
 */
@property (nonatomic, assign) NSInteger numChannels;
/**
 * 音频发送采样率
 * 单位：Hz
 */
@property (nonatomic, assign) NSInteger sentSampleRate;
/**
 * 音频采集采样率
 * 单位：Hz
 */
@property (nonatomic, assign) NSInteger recordSampleRate;
/**
 * 视频发送码率
 * 单位：kbps
 */
@property (nonatomic, assign) CGFloat videoSentKBitrate;
/**
 * 视频采集帧率
 * 单位：fps
 */
@property (nonatomic, assign) NSInteger inputFrameRate;
/**
 * 视频发送帧率
 * 单位：fps
 */
@property (nonatomic, assign) NSInteger sentFrameRate;
/**
 * 视频编码器输出帧率
 * 单位：fps
 */
@property (nonatomic, assign) NSInteger encoderOutputFrameRate;
/**
 * 视频本地渲染帧率
 * 单位：fps
 */
@property (nonatomic, assign) NSInteger rendererOutputFrameRate;
/**
 * 视频丢包率
 * 范围：0 ~ 1
 */
@property (nonatomic, assign) CGFloat videoLossRate;
/**
 * 视频往返时延
 * 单位：ms
 */
@property (nonatomic, assign) NSInteger videoRtt;
/**
 * 视频编码码率
 * 单位：kbps
 */
@property (nonatomic, assign) NSInteger encodedBitrate;
/**
 * 视频编码宽度
 * 单位：px
 */
@property (nonatomic, assign) NSInteger encodedFrameWidth;
/**
 * 视频编码高度
 * 单位：px
 */
@property (nonatomic, assign) NSInteger encodedFrameHeight;
/**
 * 视频帧总数
 */
@property (nonatomic, assign) NSInteger encodedFrameCount;

@end

/** ------------ 远端流数据统计 ------------- */
@interface VeBaseRemoteStreamStats : NSObject

/**
 * 用户ID 。远端音频流发布用户的用户ID
 */
@property (nonatomic, copy) NSString* uid;
/**
 * 远端视频宽度
 */
@property (nonatomic, assign) NSInteger width;
/**
 * 远端视频高度
 */
@property (nonatomic, assign) NSInteger height;
/**
 * 音频往返时延
 * 单位：ms
 */
@property (nonatomic, assign) NSInteger audioRtt;
/**
 * 视频往返时延
 * 单位：ms
 */
@property (nonatomic, assign) NSInteger videoRtt;
/**
 * 远端音频丢包率
 */
@property (nonatomic, assign) CGFloat audioLossRate;
/**
 * 远端视频丢包率
 */
@property (nonatomic, assign) CGFloat videoLossRate;
/**
 * 远端音频卡顿次数
 */
@property (nonatomic, assign) NSInteger audioStallCount;
/**
 * 远端视频卡顿次数
 */
@property (nonatomic, assign) NSInteger videoStallCount;
/**
 * 远端音频卡顿时长
 * 单位：ms
 */
@property (nonatomic, assign) NSInteger audioStallDuration;
/**
 * 远端视频卡顿时长
 * 单位：ms
 */
@property (nonatomic, assign) NSInteger videoStallDuration;
/**
 * 远端音频接收码率
 */
@property (nonatomic, assign) NSInteger receivedAudioKBitrate;
/**
 * 远端视频接收码率
 */
@property (nonatomic, assign) NSInteger receivedVideoKBitrate;
/**
 * 远端视频解码输出帧率
 */
@property (nonatomic, assign) NSInteger decoderOutputFrameRate;
/**
 * 远端视频渲染输出帧率
 */
@property (nonatomic, assign) NSInteger rendererOutputFrameRate;
/**
 * 音频声道数
 */
@property (nonatomic, assign) NSInteger numChannels;
/**
 * 播放采样率
 */
@property (nonatomic, assign) NSInteger playoutSampleRate;

@end

/** ---------- 音频采集参数 ---------- */
@interface VeBaseAudioFrame : NSObject
/**
 * PCM数据
 */
@property(nonatomic, strong) NSData *buffer;
/**
 * 采样点个数
 */
@property(nonatomic, assign) int samples;
/**
 * 音频声道
 */
@property(nonatomic, assign) VeBaseAudioChannel channel;
/**
 * 采样率
 */
@property(nonatomic, assign) VeBaseAudioSampleRate sampleRate;

@end

/** ----- 文件传输参数 ----- */
@interface VeFile : NSObject
/**
 * 文件名
 */
@property (nonatomic, copy) NSString *name;
/**
 * 文件二进制值
 */
@property (nonatomic, strong) NSData *fileData;
/**
 * 文件MD5值
 */
@property (nonatomic, copy) NSString *md5;
/**
 * 本地文件目路径
 */
@property (nonatomic, copy) NSString *localFilePath;
/**
 * pod端文件目路径
 */
@property (nonatomic, copy) NSString *podFilePath;

@end
