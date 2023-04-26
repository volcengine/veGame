#include "pch.h"
#include "demo_instance.h"
#include "demo_core.h"
#include "input/demo_input.h"

#define CHECK_DEMO_SHOULD_STOP                  if (should_stop_ || should_destory_) { \
                                                    return; \
                                                }

#define CHECK_DEMO_SHOULD_DESTORY               if (should_destory_) { \
                                                    return; \
                                                }

DemoInstance::DemoInstance() {
    init();
}

DemoInstance::~DemoInstance() {
    destory();
}

void DemoInstance::init() {
    if (vegame_engine_) {
        return;
    }
    vegame::EngineConfig engineConfig = {0};
    // 必须填写，火上账号信息
    engineConfig.account_id = ""; 
    engineConfig.user_id = "";
    // 自定义日志输出目录
    std::string log_dir =
        std::filesystem::current_path().append("log").string().c_str();
    engineConfig.log_file_dir = log_dir.c_str();

    // 初始化引擎
    vegame_engine_ = vegame::CreateGameEngine();
    if (!vegame_engine_) {
        veLOGE("create game engine failed");
        return;
    }
    vegame_engine_->init(engineConfig, dynamic_cast<vegame::IEngineEventHandler*>(this));

    // 可选择打开debug模式
    vegame_engine_->enableDebugMode();
    should_show_error_msg_ = true;
    should_destory_ = false;
}


void DemoInstance::start() {
    if (!vegame_engine_) {
        return;
    }
    // ak、sk、token请通过火山引擎申请ak获得，详情见https://www.volcengine.com/docs/6512/75577
    vegame::GameConfig gameConfig = { 0 };
    gameConfig.canvas = DemoCore::instance().getMainWnd();  // 必填字段
    gameConfig.ak = "";                                     // 必填字段
    gameConfig.sk = "";                                     // 必填字段
    gameConfig.token = "";                                  // 必填字段
    gameConfig.round_id = "";                               // 必填字段
    gameConfig.game_id = "";                                // 必填字段
    gameConfig.custom_game_id = "";                         // 选填字段
    gameConfig.profile_path_list = "";                      // 选填字段
    gameConfig.auto_recycle_time = 0;                       // 选填字段
    gameConfig.video_stream_profile_id = 24;                // 选填字段
    // 启动云游戏
    vegame_engine_->start(gameConfig);


    should_show_error_msg_ = true;
    should_stop_ = false;
}

void DemoInstance::stop() {
    if (!should_stop_) {
        should_stop_ = true;
        if (vegame_engine_) {
            vegame_engine_->stop();
        }
    }

    destory();
}

void DemoInstance::destory() {
    if (!need_destory_) {
        return;
    }

    if (!should_destory_) {
        should_destory_ = true;
        if (vegame_engine_) {
            vegame_engine_->destory();
            vegame::DestroyGameEngine(vegame_engine_);
            vegame_engine_ = nullptr;
        }
    }
}

void DemoInstance::restart() {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->restart();
    }
}

void DemoInstance::pauseAVStream() {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->pauseAVStream();
    }
}

void DemoInstance::resumeAVStream() {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->resumeAVStream();
    }
}

void DemoInstance::setAudioMute(bool mute) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->setAudioMute(mute);
    }
}

void DemoInstance::startSendAudioStream() {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        veLOGI("start send audio stream");
        vegame_engine_->startSendAudioStream();
    }
}

void DemoInstance::stopSendAudioStream() {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        veLOGI("stop send audio stream");
        vegame_engine_->stopSendAudioStream();
    }
}

void DemoInstance::switchVideoStreamProfile(int id) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->switchVideoStreamProfile(id);
    }
}

void DemoInstance::sendInputKeyboard(const vegame::KeyboardData& data) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->sendInputKeyboard(data);
    }
}

void DemoInstance::sendInputMouseKey(const vegame::MouseKeyData& data) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->sendInputMouseKey(data);
    }
}

void DemoInstance::sendInputMouseMove(const vegame::MouseMoveData& data) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->sendInputMouseMove(data);
    }
}

void DemoInstance::sendInputMouseWheel(const vegame::MouseWheelData& data) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->sendInputMouseWheel(data);
    }
}

void DemoInstance::sendInputGamepad(const vegame::GamepadInputData& data) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->sendInputGamepad(data);
    }
}

void DemoInstance::sendInputTouch(const vegame::TouchArrayData& data) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->sendInputTouch(data);
    }
}

void DemoInstance::sendInputImeComposition(const vegame::ImeCompositionData& data) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->sendInputImeComposition(data);
    }
}

void DemoInstance::setReconnectTime(int seconds) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        veLOGI("set reconnect time {}s", seconds);
        vegame_engine_->setReconnectTime(seconds);
    }
}

void DemoInstance::setKeyboardEnable(bool enable) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        veLOGI("set keyboard enable set: {}", enable);
        vegame_engine_->setKeyboardEnable(enable);
    }
}

void DemoInstance::getKeyboardEnable() {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        bool enable = vegame_engine_->getKeyboardEnable();
        veLOGI("get keyboard enable res: {}", enable);
    }
}

void DemoInstance::switchBackground(int state) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->switchBackground(state);
    }
}

void DemoInstance::setRemoteGameForeground() {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->setRemoteGameForeground();
    }
}

void DemoInstance::setIdleTime(int seconds) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->setIdleTime(seconds);
    }
}

void DemoInstance::setAutoRecycleTime(int seconds) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->setAutoRecycleTime(seconds);
    }
}

void DemoInstance::setUserProfilePath(const std::string& path_list) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        vegame_engine_->setUserProfilePath(path_list.c_str());
    }
}

void DemoInstance::getAutoRecycleTime() {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        auto res = vegame_engine_->getAutoRecycleTime();
        veLOGI("get auto recycle time is {}", res);
    }
}

void DemoInstance::getUserProfilePath() {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        auto res = vegame_engine_->getUserProfilePath();
        veLOGI("get user profile path is {}", res == nullptr ? "" : res);
    }
}

void DemoInstance::sendStringMessage(const std::string& str, int timeout_ms, bool need_ack) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        // warning: utf8 -> utf8 error
        auto utf8_str = gbkToUtf8(str);
        veLOGI("send string message, msg:{}", utf8_str);
        vegame_engine_->sendStringMessage(utf8_str.c_str(), timeout_ms, need_ack);
    }
}

void DemoInstance::sendBinaryMessage(const uint8_t* data, int len) {
    CHECK_DEMO_SHOULD_STOP;
    if (vegame_engine_) {
        veLOGI("send binary message, msg:0x{}", toHex(std::string((char*)data, len)));
        vegame_engine_->sendBinaryMessage(data, len);
    }
}

void DemoInstance::setNeedDestory(bool need) {
    need_destory_ = need;
}

void DemoInstance::showMonitorInfo(const std::string& msg) {
    NOTIFYICONDATAA& nid = DemoCore::instance().getNotifyIconData();
    lstrcpyA(nid.szInfo, msg.c_str());
    Shell_NotifyIconA(NIM_MODIFY, &nid);
}

void DemoInstance::onInitSuccess() {
    CHECK_DEMO_SHOULD_DESTORY;
    veLOGI("on init success");
    showMonitorInfo("on init success");
}

void DemoInstance::onPlaySuccess(int profile_id, const char* round_id, const char* game_id, const char* reserved_id) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("on play success");
    showMonitorInfo("on play success");
    should_clear_screen_ = true;
}

void DemoInstance::onPlayStop() {
    DemoCore::instance().setGameState(false);
    DemoInput::instance().setCursorLock(false);
    PostMessage(DemoCore::instance().getMainWnd(), WM_USER_CURSOR_SHOW, true, 0);
    if (should_clear_screen_) {
        should_clear_screen_ = false;
        DemoCore::instance().demoLayer()->setHasFrame(false);
        InvalidateRect(DemoCore::instance().getMainWnd(), nullptr, true);
    }
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("on play stop");
    if (should_show_error_msg_) {
        should_show_error_msg_ = false;
        showMonitorInfo("on play stop");
    }
}

void DemoInstance::onFirstAudioFrame() {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("on first audio frame");
}

void DemoInstance::onFirstVideoFrame(const vegame::VideoFrameInfo& info) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("on first video frame, width {}, height {}, rotation {}", info.width, info.height, static_cast<int>(info.rotation));
    if (DemoCore::instance().canResizeWindow()) {
        HMONITOR hmonitor = MonitorFromWindow(DemoCore::instance().getMainWnd(), MONITOR_DEFAULTTOPRIMARY);
        DemoCore::instance().resizeWindowWithShowWH(DemoCore::instance().getMainWnd(), hmonitor, info.width, info.height, true);
    }
    DemoCore::instance().setGameState(true);
    DemoCore::instance().setFrameWH(info.width, info.height);
    DemoCore::instance().updatePictureRect();
    DemoCore::instance().demoLayer()->setHasFrame(true);
    DemoCore::instance().demoLayer()->setFrameWH(info.width, info.height);
    SendMessage(DemoCore::instance().getMainWnd(), WM_ACTIVATE, WA_CLICKACTIVE, 0);
    SetForegroundWindow(DemoCore::instance().getMainWnd());
}

void DemoInstance::onStreamConnectionStateChanged(vegame::StreamConnectionState state) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("on stream connection state changed, state {}", static_cast<int>(state));
}

void DemoInstance::onRemoteAudioStartRequest() {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("remote audio start request");
    if (vegame_engine_) {
        vegame_engine_->startSendAudioStream();
    }
}

void DemoInstance::onRemoteAudioStopRequest() {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("remote audio stop request");
    if (vegame_engine_) {
        vegame_engine_->stopSendAudioStream();
    }
}

void DemoInstance::onAudioStats(const vegame::AudioStats& stats) {
    CHECK_DEMO_SHOULD_STOP;
    // 
}

void DemoInstance::onVideoStats(const vegame::VideoStats& stats) {
    CHECK_DEMO_SHOULD_STOP;
    // 
    DemoCore::instance().demoLayer()->setVideoStats(
        stats.video_loss_rate,
        static_cast<long>(stats.e2e_delay),
        stats.rtt,
        stats.renderer_output_frame_rate,
        stats.received_kbitrate);
    DemoCore::instance().demoLayer()->onRender();
}

void DemoInstance::onNetworkQuality(vegame::NetworkQualityState state) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("onNetworkQuality, quality: {}", static_cast<int>(state));
}

void DemoInstance::onMessageChannelStateChanged(const char* channel_id, vegame::MessageChannelState state) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("onMessageChannelStateChanged, state: {}", static_cast<int>(state));
}

void DemoInstance::onVideoSizeChanged(const vegame::VideoFrameInfo& info) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("on video size changed, width {}, height {}, rotation {}", info.width, info.height, static_cast<int>(info.rotation));
    if (DemoCore::instance().canResizeWindow()) {
        HMONITOR hmonitor = MonitorFromWindow(DemoCore::instance().getMainWnd(), MONITOR_DEFAULTTOPRIMARY);
        DemoCore::instance().resizeWindowWithShowWH(DemoCore::instance().getMainWnd(), hmonitor, info.width, info.height, true);
    }
    DemoCore::instance().setFrameWH(info.width, info.height);
    DemoCore::instance().updatePictureRect();
    DemoCore::instance().demoLayer()->setFrameWH(info.width, info.height);
}

void DemoInstance::onVideoStreamProfileChange(bool result, int from, int to) {
    CHECK_DEMO_SHOULD_STOP;
    if (result) {
        showMonitorInfo(fmt::format("change profile id from {} to {}", from, to));
    }
}

void DemoInstance::onDelayDetected(int milliseconds) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("on delay detected {}", milliseconds);
    DemoCore::instance().demoLayer()->setDetectDelay(milliseconds);
}

void DemoInstance::onSendMessageResult(bool success, const char* mid) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("on send message result, success {}, mid {}", success, mid == nullptr ? "" : mid);
}

void DemoInstance::onStringMessageReceived(const char* str) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("on string message received, msg:{}", str);
}

void DemoInstance::onBinaryMessageReceived(const uint8_t* data, int len) {
    CHECK_DEMO_SHOULD_STOP;
    std::string bin_data(reinterpret_cast<const char*>(data), len);
    veLOGI("on binary message received, data 0x{}", toHex(bin_data));
}

void DemoInstance::onError(int code, const char* msg) {
    veLOGE("code {}, msg {}", code, msg);
    DemoCore::instance().setGameState(false);
    DemoInput::instance().setCursorLock(false);
    PostMessage(DemoCore::instance().getMainWnd(), WM_USER_CURSOR_SHOW, true, 0);
    if (should_clear_screen_) {
        should_clear_screen_ = false;
        DemoCore::instance().demoLayer()->setHasFrame(false);
        InvalidateRect(DemoCore::instance().getMainWnd(), nullptr, true);
    }

    CHECK_DEMO_SHOULD_DESTORY;
    if (should_show_error_msg_) {
        should_show_error_msg_ = false;
        showMonitorInfo(fmt::format("on error code {}, msg {}", code, msg).c_str());
    }
}

void DemoInstance::onWarning(int code, const char* msg) {
    veLOGW("code {}, msg {}", code, msg);

    CHECK_DEMO_SHOULD_DESTORY;
    // showMonitorInfo(fmt::format("on warning code {}, msg {}", code, msg).c_str());
}

void DemoInstance::onKeyboardEnable(bool enable) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("onKeyboardEnable, enable: {}", enable);
}

void DemoInstance::onBackgroundSwitched(bool state) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("onBackgroundSwitched, state: {}", static_cast<int>(state));
}

void DemoInstance::onRemoteGameSwitchedBackground(vegame::RemoteGameSwitchType type) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("onRemoteGameSwitchedBackground, type: {}", static_cast<int>(type));
}

void DemoInstance::onRemoteGameSwitchedForeground(vegame::RemoteGameSwitchType type) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("onRemoteGameSwitchedForeground, type: {}", static_cast<int>(type));
}

void DemoInstance::onOutputCursorPos(const vegame::MouseCursorPos& data) {
    CHECK_DEMO_SHOULD_STOP;
    if (!DemoCore::instance().isClientForeground()) {
        return;
    }

    veLOGI("output cursor pos x {}, y {}", data.abs_x, data.abs_y);
    WINDOWINFO info = { sizeof(WINDOWINFO) };
    GetWindowInfo(DemoCore::instance().getMainWnd(), &info);
    int left = info.rcClient.left;
    int top = info.rcClient.top;

    RECT picture_rect = DemoCore::instance().getPictureRect();
    int picture_width = picture_rect.right - picture_rect.left;
    int picture_height = picture_rect.bottom - picture_rect.top;

    left += picture_width * data.abs_x / 65535;
    top += picture_height * data.abs_y / 65535;
    if ((GetKeyState(VK_LBUTTON) & 0x8000) == 0) {
        SetCursorPos(left, top);
    }
}

void DemoInstance::onOutputCursorVisibility(bool visible) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("output cursor show: {}", visible);
    PostMessage(DemoCore::instance().getMainWnd(), WM_USER_CURSOR_SHOW, visible, 0);
}

void DemoInstance::onOutputGamepadVibration(const vegame::GamepadVibrationData& data) {
    CHECK_DEMO_SHOULD_STOP;
    if (!DemoCore::instance().isClientForeground()) {
        return;
    }

    XINPUT_VIBRATION xv = { static_cast<WORD>(data.lm), static_cast<WORD>(data.rm) };
    XInputSetState(data.index, &xv);
}

void DemoInstance::onOutputImeState(const vegame::ImeStateData& data) {
    CHECK_DEMO_SHOULD_STOP;
    veLOGI("output ime state {}", data.enable);
    DemoInput::instance().setImeState(data);
    PostMessage(DemoCore::instance().getMainWnd(), WM_USER_IME_ACTIVE, data.enable, 0);
}

