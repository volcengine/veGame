#pragma once
#include "demo_instance.h"
#include "layer/status_layer_wnd.h"

class DemoCore : public SingletonInSameModule<DemoCore> {
    friend class SingletonInSameModule<DemoCore>;
public:
    virtual ~DemoCore();
    // demo instance
    std::shared_ptr<DemoInstance> demoIns();
    std::shared_ptr<StatusLayerWnd> demoLayer();

    // global param
    void setNotifyIconData(const NOTIFYICONDATAA& nid);
    NOTIFYICONDATAA& getNotifyIconData();

    // game config
    void setMainWnd(HWND wnd);
    HWND& getMainWnd();

    const char* getClientUserId();
    const char* getAccountId();
    const char* getAk();
    const char* getSk();
    const char* getToken();
    const char* getGameId();
    const char* getCustomGameId();
    const char* getRoundId();
    const char* getRegion();
    const char* getService();
    const char* getVersion();
    const char* getProfilePathList();
    int getAutoRecycleTime();
    int getVideoStreamProfileId();
    bool getKeyboardEnable();


    // 游戏时状态保存
    // 获取当前画面 RECT
    RECT& getPictureRect();
    // 当前是否在游戏
    bool isGaming();
    void setGameState(bool is_gaming);
    // 当前客户端是否在后台
    bool isClientForeground();
    void setClientState(bool is_background);
    // 当前是否全屏
    bool isFullScreen();
    void setFullScreenState(bool is_fullscreen);
    // 设置游戏画面宽高
    void setFrameWH(int w, int h);
    // 更新游戏画面显示大小，用于框定鼠标指针位置
    void updatePictureRect();
    // 游戏窗口全屏切换
    void switchFullScreen();
    // 根据画面显示大小 resize wnd
    void resizeWindowWithShowWH(HWND hwnd, HMONITOR hmonitor, int width, int height, bool has_menu = false);

    // from cgc config
    void setClientWH(int w, int h);
    int getClientWidth();
    int getClientHeight();
    bool canFullScreen();
    bool canResizeWindow();
    bool shouldShowVideoStats();

private:
    std::shared_ptr<DemoInstance> demo_instance_ = nullptr;
    std::shared_ptr<StatusLayerWnd> status_layer_ = nullptr;

    // global param
    NOTIFYICONDATAA notify_icon_data_;           // 托盘属性

    // game config
    HWND main_wnd_ = NULL;
    const char* client_user_id_ = nullptr;
    const char* account_id_ = nullptr;
    const char* ak_ = nullptr;
    const char* sk_ = nullptr;
    const char* token_ = nullptr;
    const char* game_id_ = nullptr;
    const char* custom_game_id_ = nullptr;
    const char* round_id_ = nullptr;
    const char* user_tag_ = nullptr;
    const char* plan_id_ = nullptr;
    const char* reserved_id_ = nullptr;
    const char* profile_path_list_ = nullptr;
    int auto_recycle_time_ = 0;
    int  video_stream_profile_id_ = 0;
    bool enable_screen_lock_ = false;
    bool enable_fullscreen_ = false;
    bool enable_keyboard_ = true;
    std::string extra_;

    // demo instance param
    // 保存客户端显示画面的位置及宽高(画面比例)，初始为屏幕中间，框住鼠标
    RECT picture_rect_ = { 0 };
    int frame_width_ = 1280;
    int frame_height_ = 720;
    // 保存当前是否游戏状态
    bool is_gaming_ = false;
    // 当前客户端是否在后台
    bool is_client_foreground_ = false;
    // 保存WINDOWPLACEMENT，游戏全屏时用到
    WINDOWPLACEMENT wnd_pm_ = { sizeof(WINDOWPLACEMENT) };
    // 保存当前是否是全屏状态
    bool is_fullscreen_ = false;

    // from cgc
    int client_width_ = 1280;
    int client_height_ = 720;
    bool can_fullscreen_ = false;
    bool can_resize_window_ = false;
    bool should_show_video_stats_ = false;
};
