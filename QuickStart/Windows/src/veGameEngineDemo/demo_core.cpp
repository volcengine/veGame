#include "pch.h"
#include "demo_core.h"
#include "demo_utils.h"

DemoCore::~DemoCore() {
    if (demo_instance_) {
        demo_instance_->destory();
    }
}

// deom ins
std::shared_ptr<DemoInstance> DemoCore::demoIns() {
    if (!demo_instance_) {
        demo_instance_ = std::make_shared<DemoInstance>();
    }
    return demo_instance_;
}

std::shared_ptr<StatusLayerWnd> DemoCore::demoLayer() {
    if (!status_layer_) {
        status_layer_ = std::make_shared<StatusLayerWnd>();
    }
    return status_layer_;
}

void DemoCore::setNotifyIconData(const NOTIFYICONDATAA& nid) {
    notify_icon_data_ = nid;
}

NOTIFYICONDATAA& DemoCore::getNotifyIconData() {
    return notify_icon_data_;
}

// game config
void DemoCore::setMainWnd(HWND wnd) {
    main_wnd_ = wnd;
}

HWND& DemoCore::getMainWnd() {
    return main_wnd_;
}

const char* DemoCore::getClientUserId() {
    return client_user_id_;
}

const char* DemoCore::getAccountId() {
    return account_id_;
}

const char* DemoCore::getAk() {
    return ak_;
}

const char* DemoCore::getSk() {
    return sk_;
}

const char* DemoCore::getToken() {
    return token_;
}

const char* DemoCore::getGameId() {
    return game_id_;
}

const char* DemoCore::getCustomGameId() {
    return custom_game_id_;
}

const char* DemoCore::getRoundId() {
    return round_id_;
}

const char* DemoCore::getProfilePathList() {
    return profile_path_list_;
}

int DemoCore::getAutoRecycleTime() {
    return auto_recycle_time_;
}

int DemoCore::getVideoStreamProfileId() {
    return video_stream_profile_id_;
}

bool DemoCore::getKeyboardEnable() {
    return enable_keyboard_;
}

// 游戏时状态保存
void DemoCore::setFrameWH(int w, int h) {
    frame_width_ = w;
    frame_height_ = h;
}

RECT& DemoCore::getPictureRect() {
    return picture_rect_;
}

bool DemoCore::isGaming() {
    return is_gaming_;
}

void DemoCore::setGameState(bool is_gaming) {
    is_gaming_ = is_gaming;
}

bool DemoCore::isClientForeground() {
    return is_client_foreground_;
}

void DemoCore::setClientState(bool is_foreground) {
    is_client_foreground_ = is_foreground;
}

bool DemoCore::isFullScreen() {
    return is_fullscreen_;
}

void DemoCore::setFullScreenState(bool is_fullscreen) {
    is_fullscreen_ = is_fullscreen;
}

void DemoCore::updatePictureRect() {
    if (frame_width_ == 0 || frame_height_ == 0) {
        veLOGE("frame width = {}, height = {}.", frame_width_, frame_height_);
        return;
    }
    if (main_wnd_ == nullptr) {
        return;
    }

    WINDOWINFO info = { sizeof(WINDOWINFO) };
    GetWindowInfo(main_wnd_, &info);
    int client_width = info.rcClient.right - info.rcClient.left;
    int client_height = info.rcClient.bottom - info.rcClient.top;

    int picture_width, picture_height;
    int gap_width, gap_height;
    if (client_width * frame_height_ > client_height * frame_width_) {  // 客户区宽了
        picture_rect_.top = info.rcClient.top;
        picture_rect_.bottom = info.rcClient.bottom;

        picture_height = client_height;
        picture_width = picture_height * frame_width_ / frame_height_;
        gap_width = (client_width - picture_width) / 2;

        picture_rect_.left = info.rcClient.left + gap_width;
        picture_rect_.right = info.rcClient.right - gap_width;
    } else {    // 客户区高了
        picture_rect_.left = info.rcClient.left;
        picture_rect_.right = info.rcClient.right;

        picture_width = client_width;
        picture_height = picture_width * frame_height_ / frame_width_;
        gap_height = (client_height - picture_height) / 2;

        picture_rect_.top = info.rcClient.top + gap_height;
        picture_rect_.bottom = info.rcClient.bottom - gap_height;
    }
}

void DemoCore::switchFullScreen() {
    if (main_wnd_ == nullptr) {
        return;
    }
    // get menu size
    MENUBARINFO menu_info = { sizeof(menu_info) };
    int menu_height = 0;
    if (GetMenuBarInfo(main_wnd_, OBJID_MENU, 0, &menu_info)) {
        menu_height = menu_info.rcBar.bottom - menu_info.rcBar.top;
    }
    // get window_style
    LONG_PTR window_style = GetWindowLongPtr(main_wnd_, GWL_STYLE);
    if (window_style == 0) {
        return;
    }
    if (!DemoCore::instance().isFullScreen()) {
        // 全屏
        MONITORINFO mi = { sizeof(MONITORINFO) };
        if (GetWindowPlacement(main_wnd_, &wnd_pm_)
            && GetMonitorInfo(MonitorFromWindow(main_wnd_, MONITOR_DEFAULTTOPRIMARY), &mi)) {
            SetWindowLongPtr(main_wnd_, GWL_STYLE, window_style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(main_wnd_, HWND_TOP,
                mi.rcMonitor.left, mi.rcMonitor.top - menu_height,
                mi.rcMonitor.right - mi.rcMonitor.left,
                mi.rcMonitor.bottom - mi.rcMonitor.top + menu_height,
                SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    } else {
        SetWindowLongPtr(main_wnd_, GWL_STYLE, window_style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(main_wnd_, &wnd_pm_);
        SetWindowPos(main_wnd_, NULL, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
    is_fullscreen_ = !is_fullscreen_;
    updatePictureRect();
}

void DemoCore::resizeWindowWithShowWH(HWND hwnd, HMONITOR hmonitor, int width, int height, bool has_menu) {
    if (width == 0 || height == 0) {
        return;
    }
    // get monitor size
    MONITORINFO monitor_info = { sizeof(MONITORINFO) };
    if (!GetMonitorInfo(hmonitor, &monitor_info)) {
        return;
    }
    int monitor_width = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
    int monitor_height = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;
    // 是否全屏
    if (monitor_width == width && monitor_height == height) {
        switchFullScreen();
        return;
    }
    // 
    WINDOWINFO window_info = { sizeof(WINDOWINFO) };
    if (!GetWindowInfo(hwnd, &window_info)) {
        return;
    }
    // get wnd size
    RECT wnd_rect{ 0, 0, width, height };
    if (!AdjustWindowRect(&wnd_rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, has_menu)) {
        veLOGI("adjust window rect failed, error: {}", GetLastError());
    }
    // video show size
    int wnd_width = wnd_rect.right - wnd_rect.left;
    int wnd_height = wnd_rect.bottom - wnd_rect.top;
    // 居中显示
    int start_x = monitor_info.rcMonitor.left + (monitor_width - wnd_width) / 2;
    int start_y = monitor_info.rcMonitor.top + (monitor_height - wnd_height) / 2;
    if (wnd_width > monitor_width || wnd_height > monitor_height) {
        int gap_width = wnd_width - width;
        int gap_height = wnd_height - height;
        if (monitor_width * wnd_height > monitor_height * wnd_width) {  // 显示器宽了
            int client_height = monitor_height - gap_height;
            wnd_width = client_height * width / height + gap_width;
            wnd_height = monitor_height;

            start_x = monitor_info.rcMonitor.left + (monitor_width - wnd_width) / 2;
            start_y = monitor_info.rcMonitor.top;
        } else {    // 显示器高了
            int client_width = monitor_width - gap_width;
            wnd_width = monitor_width;
            wnd_height = client_width * height / width + gap_height;

            start_x = monitor_info.rcMonitor.left;
            start_y = monitor_info.rcMonitor.top + (monitor_height - wnd_height) / 2;
        }
    }

    SetWindowPos(hwnd, HWND_TOP, start_x, start_y, wnd_width, wnd_height, SWP_SHOWWINDOW);
}

// from cgc config
void DemoCore::setClientWH(int w, int h) {
    client_width_ = w; client_height_ = h;
}

int DemoCore::getClientWidth() {
    return client_width_;
}

int DemoCore::getClientHeight() {
    return client_height_;
}

bool DemoCore::canFullScreen() {
    return is_fullscreen_;
}

bool DemoCore::canResizeWindow() {
    return can_resize_window_;
}

bool DemoCore::shouldShowVideoStats() {
    return should_show_video_stats_;
}
