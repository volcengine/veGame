#pragma once
#include "vegame_engine_event_handler.h"
#include "vegame_common.h"

namespace vegame {

class IGameEngine {
public:
    virtual ~IGameEngine() = default;

    // 初始化 SDK
    virtual void init(const EngineConfig& config, IEngineEventHandler* handler) = 0;
    // 启动云游戏并开始拉流
    virtual void start(const GameConfig& config) = 0;
    // 停止从云端拉流
    virtual void stop() = 0;
    // 销毁 SDK 实例
    virtual void destory() = 0;
    // 重启云游戏
    virtual void restart() = 0;


    // 暂停拉取云端音视频流
    virtual void pauseAVStream() = 0;
    // 恢复拉取云端音视频流
    virtual void resumeAVStream() = 0;
    // 游戏静音开关
    virtual void setAudioMute(bool mute) = 0;
    // 开始发送本地音频数据
    virtual void startSendAudioStream() = 0;
    // 停止发送本地音频数据
    virtual void stopSendAudioStream() = 0;
    // 设置云游戏视频流清晰度
    virtual void switchVideoStreamProfile(int profile_id) = 0;


    // 发送本地键盘输入按键值
    virtual void sendInputKeyboard(const KeyboardData& data) = 0;
    // 发送本地鼠标按键值，包括鼠标绝对位置
    virtual void sendInputMouseKey(const MouseKeyData& data) = 0;
    // 发送本地鼠标光标移动值，包括鼠标绝对位置
    virtual void sendInputMouseMove(const MouseMoveData& data) = 0;
    // 发送本地鼠标滚轮滚动值
    virtual void sendInputMouseWheel(const MouseWheelData& data) = 0;
    // 发送本地游戏手柄按键值
    virtual void sendInputGamepad(const GamepadInputData& data) = 0;
    // 发送本地触屏触摸事件
    virtual void sendInputTouch(const TouchArrayData& data) = 0;
    // 发送本地输入法事件
    virtual void sendInputImeComposition(const ImeCompositionData& data) = 0;

    // 是否启用用户键盘
    virtual void setKeyboardEnable(bool enable) = 0;
    // 获取用户键盘启用状态
    virtual bool getKeyboardEnable() = 0;

    // 消息发送与接收
    virtual const char* sendStringMessage(const char* str, int timeout_ms, bool need_ack) = 0;
    virtual void sendBinaryMessage(const uint8_t* data, int len) = 0;

    // 发送本地用户当前状态
    virtual void switchBackground(bool state) = 0;
    // 切换远端游戏到前台
    virtual void setRemoteGameForeground() = 0;

    // 设置断线重连时间
    virtual void setReconnectTime(int32_t seconds) = 0;
    // 设置保活时间
    virtual void setIdleTime(int32_t seconds) = 0;
    // 设置无操作回收服务时间
    virtual void setAutoRecycleTime(int32_t seconds) = 0;
    // 设置保存用户配置文件的路径
    virtual void setUserProfilePath(const char* path_list) = 0;

    // 获取无操作回收服务时间
    virtual int32_t getAutoRecycleTime() = 0;
    // 获取保存用户配置文件的路径
    virtual const char* getUserProfilePath() = 0;


    // 进入 debug 调试模式
    virtual void enableDebugMode() = 0;
    // 获取 SDK 版本号
    virtual const char* getSDKVersion() = 0;
    // 获取 SDK 生成的设备 ID
    virtual const char* getDeviceId() = 0;
};

extern "C" {
    NATIVESDK_API IGameEngine* CreateGameEngine();
    NATIVESDK_API void DestroyGameEngine(IGameEngine*& engine);
}

} // namespace vegame
