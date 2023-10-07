#pragma once
#include "veGame/vegame_engine_interface.h"

class DemoInstance : public vegame::IEngineEventHandler {
public:
    DemoInstance();
    virtual ~DemoInstance();

    void init();
    void start();
    void stop();
    void destory();
    void restart();

    void pauseAVStream();
    void resumeAVStream();
    void setAudioMute(bool mute);
    void startSendAudioStream();
    void stopSendAudioStream();
    void switchVideoStreamProfile(int id);

    void sendInputKeyboard(const vegame::KeyboardData& data);
    void sendInputMouseKey(const vegame::MouseKeyData& data);
    void sendInputMouseMove(const vegame::MouseMoveData& data);
    void sendInputMouseWheel(const vegame::MouseWheelData& data);
    void sendInputGamepad(const vegame::GamepadInputData& data);
    void sendInputTouch(const vegame::TouchArrayData& data);
    void sendInputImeComposition(const vegame::ImeCompositionData& data);

    void sendStringMessage(const std::string& str, int timeout_ms, bool need_ack);
    void sendBinaryMessage(const uint8_t* data, int len);

    void setReconnectTime(int seconds);
    void setKeyboardEnable(bool enable);
    void getKeyboardEnable();
    void switchBackground(int state);
    void setRemoteGameForeground();

    void setIdleTime(int seconds);
    void setAutoRecycleTime(int seconds);
    void setUserProfilePath(const std::string& path_list);
    void getAutoRecycleTime();
    void getUserProfilePath();

    // 
    void setNeedDestory(bool need);

protected:
    // tool func
    void showMonitorInfo(const std::string& msg);

    virtual void onInitSuccess() override;
    virtual void onPlaySuccess(int profile_id, const char* round_id, const char* game_id, const char* reserved_id) override;
    virtual void onPlayStop() override;
    virtual void onFirstAudioFrame() override;
    virtual void onFirstVideoFrame(const vegame::VideoFrameInfo& info) override;
    virtual void onStreamConnectionStateChanged(vegame::StreamConnectionState state) override;
    virtual void onRemoteAudioStartRequest() override;
    virtual void onRemoteAudioStopRequest() override;
    virtual void onAudioStats(const vegame::AudioStats& stats) override;
    virtual void onVideoStats(const vegame::VideoStats& stats) override;
    virtual void onNetworkQuality(vegame::NetworkQualityState state) override;
    virtual void onMessageChannelStateChanged(const char* channel_id, vegame::MessageChannelState state) override;
    virtual void onVideoSizeChanged(const vegame::VideoFrameInfo& info) override;
    virtual void onVideoStreamProfileChange(bool result, int from, int to) override;
    virtual void onDelayDetected(int milliseconds) override;
    virtual void onSendMessageResult(bool success, const char* mid) override;
    virtual void onStringMessageReceived(const char* str) override;
    virtual void onBinaryMessageReceived(const uint8_t* data, int len) override;
    virtual void onError(int code, const char* msg) override;
    virtual void onWarning(int code, const char* msg) override;
    virtual void onKeyboardEnable(bool enable) override;
    virtual void onBackgroundSwitched(bool state) override;
    virtual void onRemoteGameSwitchedBackground(vegame::RemoteGameSwitchType type) override;
    virtual void onRemoteGameSwitchedForeground(vegame::RemoteGameSwitchType type) override;
    virtual void onOutputCursorPos(const vegame::MouseCursorPos& data) override;
    virtual void onOutputCursorVisibility(bool visible) override;
    virtual void onOutputGamepadVibration(const vegame::GamepadVibrationData& data) override;
    virtual void onOutputImeState(const vegame::ImeStateData& data) override;

private:
    std::atomic<bool> should_stop_ = true;
    std::atomic<bool> should_destory_ = true;
    vegame::IGameEngine* vegame_engine_ = nullptr;
    bool should_show_error_msg_ = false;
    bool should_clear_screen_ = false;

    // 测试用，测试离线场景
    bool need_destory_ = true;
};
