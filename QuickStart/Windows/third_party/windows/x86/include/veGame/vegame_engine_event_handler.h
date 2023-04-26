#include "vegame_type_defines.h"

namespace vegame {

class IEngineEventHandler {
public:
    virtual ~IEngineEventHandler() = default;

    // SDK 初始化成功后回调
    virtual void onInitSuccess() {
    }
    // 启动云游戏后，播放成功回调
    virtual void onPlaySuccess(int profile_id, const char* round_id, const char* game_id, const char* reserved_id) {
        (void)profile_id;
        (void)round_id;
        (void)game_id;
        (void)reserved_id;
    }
    // 云游戏正常停止
    virtual void onPlayStop() {
    }
    // 订阅到音频流后的音频首帧回调
    virtual void onFirstAudioFrame() {
    }
    // 订阅到视频流后的视频首帧回调
    virtual void onFirstVideoFrame(const VideoFrameInfo& info) {
        (void)info;
    }
    // 音视频流连接状态切换回调
    virtual void onStreamConnectionStateChanged(StreamConnectionState state) {
        (void)state;
    }
    // 收到云端实例请求开始发送音频数据事件
    virtual void onRemoteAudioStartRequest() {
    }
    // 收到云端实例请求停止发送音频数据事件
    virtual void onRemoteAudioStopRequest() {
    }
    // 音频流统计信息回调，2 秒回调一次
    virtual void onAudioStats(const AudioStats& stats) {
        (void)stats;
    }
    // 视频流统计信息回调，2 秒回调一次
    virtual void onVideoStats(const VideoStats& stats) {
        (void)stats;
    }
    // 视频帧大小改变回调
    virtual void onVideoSizeChanged(const VideoFrameInfo& info) {
        (void)info;
    }
    // 清晰度切换结果回调
    virtual void onVideoStreamProfileChange(bool result, int from, int to) {
        (void)result;
        (void)from;
        (void)to;
    }
    // 当时网络状态回调，2 秒回调一次
    virtual void onNetworkQuality(NetworkQualityState state) {
        (void)state;
    }
    // 当前操作时延回调，5 秒回调一次
    virtual void onDelayDetected(int milliseconds) {
        (void)milliseconds;
    }
    // 远端消息通道状态改变回调
    virtual void onMessageChannelStateChanged(const char* channel_id, MessageChannelState state) {
        (void)channel_id;
        (void)state;
    }
    // 发送消息给游戏的发送结果回调
    virtual void onSendMessageResult(bool success, const char* mid) {
        (void)success;
        (void)mid;
    }
    // 收到游戏内字符串消息回调
    virtual void onStringMessageReceived(const char* str) {
        (void)str;
    }
    // 收到游戏内二进制数据消息回调
    virtual void onBinaryMessageReceived( const uint8_t* data, int len) {
        (void)data;
        (void)len;
    }
    // SDK 内部产生的错误回调，产生错误会调用 stop() 停止从云端拉流
    virtual void onError(int code, const char* msg) {
        (void)code;
        (void)msg;
    }
    // SDK 内部产生的警告回调
    virtual void onWarning(int code, const char* msg) {
        (void)code;
        (void)msg;
    }
    // 是否启用本地键盘回调
    virtual void onKeyboardEnable(bool enable) {
        (void)enable;
    }
    // 云端收到本地用户状态后的回调
    virtual void onBackgroundSwitched(bool state) {
        (void)state;
    }
    // 云游戏应用切换到后台回调
    virtual void onRemoteGameSwitchedBackground(RemoteGameSwitchType type) {
        (void)type;
    }
    // 云游戏应用切换到前台回调
    virtual void onRemoteGameSwitchedForeground(RemoteGameSwitchType type) {
        (void)type;
    }

    // 输出游戏中鼠标位置
    virtual void onOutputCursorPos(const MouseCursorPos& data) {
        (void)data;
    }
    // 输出游戏中鼠标是否显示，客户端需要同步修改鼠标隐藏状态
    virtual void onOutputCursorVisibility(bool visible) {
        (void)visible;
    }
    // 输出游戏中手柄震动事件
    virtual void onOutputGamepadVibration(const GamepadVibrationData& data) {
        (void)data;
    }
    // 输出游戏中输入法状态
    virtual void onOutputImeState(const ImeStateData& data) {
        (void)data;
    }
};

}