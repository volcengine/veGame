#include "cloudrenderx_game_session.h"
#include "cloudrenderx_window_base.h"
#include <time.h>
#include "ve_log_lite.h"

#include "cloudrenderx/bean/mouse_data.h"
#include "cloudrenderx/bean/keyboard_data.h"
#include "cloudrenderx/cloudgame/services/game_mouse_service.h"
#include "cloudrenderx/cloudgame/services/game_keyboard_service.h"

#include <Windows.h>

#include "cloudrenderx_user_events.h"

static const USHORT MOUSE_WHEEL = RI_MOUSE_WHEEL;
static const USHORT MOUSE_BUTTON_LEFT = RI_MOUSE_LEFT_BUTTON_DOWN | RI_MOUSE_LEFT_BUTTON_UP;
static const USHORT MOUSE_BUTTON_RIGHT = RI_MOUSE_RIGHT_BUTTON_DOWN | RI_MOUSE_RIGHT_BUTTON_UP;
static const USHORT MOUSE_BUTTON_MIDDLE = RI_MOUSE_MIDDLE_BUTTON_DOWN | RI_MOUSE_MIDDLE_BUTTON_UP;
static const USHORT MOUSE_BUTTON_DOWN = RI_MOUSE_LEFT_BUTTON_DOWN | RI_MOUSE_RIGHT_BUTTON_DOWN | RI_MOUSE_MIDDLE_BUTTON_DOWN;
static const USHORT MOUSE_BUTTON_UP = RI_MOUSE_LEFT_BUTTON_UP | RI_MOUSE_RIGHT_BUTTON_UP | RI_MOUSE_MIDDLE_BUTTON_UP;

std::unique_ptr<vecloudrenderx::VeCloudRenderX>  cloudrenderx_demo::CloudRenderxGameSession::veCloudRenderX = nullptr;

void cloudrenderx_demo::CloudRenderxGameSession::init(const std::string& account_id, HINSTANCE process_instance)
{
    if (!veCloudRenderX) {
        veCloudRenderX = std::make_unique<vecloudrenderx::VeCloudRenderX>();
    }

    vecloudrenderx::XConfig config;
    config.accountId = account_id;
    config.process_instance = process_instance;

    veCloudRenderX->prepare(config);

    // 打开调试模式，调试模式会打印更多的日志
    veCloudRenderX->setDebug(true);
}

void cloudrenderx_demo::CloudRenderxGameSession::startCloudGame(HWND wnd, std::shared_ptr<CloudrenderxDemoConfig> demo_config, int width, int height, std::shared_ptr<CloudGameSessionListener> session_listener)
{
    if (currentGameSession) {
        CloudRenderXWindowBase::showMessageBox(L"已经启动了一款云游戏!");
    }

    // 创建session配置
    CloudGameSessionConfig cloudGameSessionConfig;

    // 窗口句柄
    cloudGameSessionConfig.basicConfig.canvas = wnd;
    cloudGameSessionConfig.width = width;
    cloudGameSessionConfig.height = height;
    currentWnd = wnd;
    updateValidArea();

    // 生成user id，接入方可根据自身场景，自定义生成逻辑
    std::string userId("cloudgame_demo_user_");
    int timestamp = time(nullptr);
    userId += std::to_string(timestamp);
    cloudGameSessionConfig.basicConfig.userId = userId;

    cloudGameSessionConfig.gameId = demo_config->game_id;

    // 设置账号安全相关的配置
    cloudGameSessionConfig.basicConfig.ak = demo_config->ak;
    cloudGameSessionConfig.basicConfig.sk = demo_config->sk;
    cloudGameSessionConfig.basicConfig.token = demo_config->token;

    // 接入方可根据自身场景，自定义生成逻辑
    cloudGameSessionConfig.roundId = "round_demo_test_";

    // 设置清晰度挡位
    cloudGameSessionConfig.basicConfig.videoStreamProfileId = 19;

    // 创建session
    currentGameSession = veCloudRenderX->createCloudGameSession(cloudGameSessionConfig);

    currentGameSession->setSessionListener(session_listener);

    currentGameSession->start();

    veLOGI("the cloudgame session start...");
}

void cloudrenderx_demo::CloudRenderxGameSession::stop()
{
    if (currentGameSession) {
        currentGameSession->stop();
        currentGameSession = nullptr;
    }
}

std::shared_ptr<CloudGameSession> cloudrenderx_demo::CloudRenderxGameSession::getCurrentGameSession()
{
    return currentGameSession;
}

void cloudrenderx_demo::CloudRenderxGameSession::captureRawInput(LPARAM lparam)
{
    if (!currentGameSession) {
        return;
    }

    auto mouse_service = currentGameSession->getMouseService();
    if (!mouse_service) {
        return;
    }

    // 读取原始输入数据
    UINT dw_size = 0;
    GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &dw_size, (UINT)sizeof(RAWINPUTHEADER));
    if (dw_size == 0) {
        return;
    }
    if (dw_size > dwSize) {
        lpbBuffer.reset(new BYTE[dw_size]);
        dwSize = dw_size;
    }
    GetRawInputData((HRAWINPUT)lparam, RID_INPUT, (LPVOID)lpbBuffer.get(), (PUINT)&dwSize, (UINT)sizeof(RAWINPUTHEADER));   // 第二次调用获取原始输入数据，读入lpbBuffer

    RAWINPUT* raw = (RAWINPUT*)lpbBuffer.get();
    if (raw->header.dwType == RIM_TYPEMOUSE) {
        USHORT mouse_button = raw->data.mouse.usButtonFlags;

        // 获取鼠标位置
        POINT point;
        GetCursorPos(&point);
        RECT mouse_rect = valid_area;
        mouse_rect.right += 1;
        mouse_rect.bottom += 1;

        int valid_area_width = valid_area.right - valid_area.left;
        int valid_area_height = valid_area.bottom - valid_area.top;

        if (!PtInRect(&mouse_rect, point)) {
            if (current_pressed_mouse_button != 0) {
                // 鼠标移除有效区域时，自动发送up事件，避免事件中断，而造成的游戏角色自动运行的问题
                vecommon::MouseKeyData key;
                key.key = current_pressed_mouse_button;
                key.down = false;
                key.absX = (point.x - valid_area.left) * 65535 / valid_area_width;
                key.absY = (point.y - valid_area.top) * 65535 / valid_area_height;
                mouse_service->sendButtonEvent(key);

                current_pressed_mouse_button = 0;
            }
            return;
        }

        // mouse move
        if (raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) {
            vecommon::MouseMoveData move;
            static INT nLastX = 0;
            static INT nLastY = 0;
            move.absX = raw->data.mouse.lLastX;
            move.absY = raw->data.mouse.lLastY;
            move.deltaX = raw->data.mouse.lLastX - nLastX;
            move.deltaY = raw->data.mouse.lLastY - nLastY;
            nLastX = raw->data.mouse.lLastX;
            nLastY = raw->data.mouse.lLastY;
            if (currentGameSession) {
                auto mouse_service = currentGameSession->getMouseService();
                if (mouse_service) {
                    mouse_service->sendMoveEvent(move);
                }
            }
        }
        else if (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0) {
            vecommon::MouseMoveData move;
            move.absX = (point.x - valid_area.left) * 65535 / valid_area_width;
            move.absY = (point.y - valid_area.top) * 65535 / valid_area_height;
            move.deltaX = raw->data.mouse.lLastX;
            move.deltaY = raw->data.mouse.lLastY;

            auto mouse_service = currentGameSession->getMouseService();
            if (mouse_service) {
                mouse_service->sendMoveEvent(move);
            }
        }

        // mouse key
        uint8_t button = 0;
        if (mouse_button & MOUSE_BUTTON_LEFT) {
            button = VK_LBUTTON;
        }
        else if (mouse_button & MOUSE_BUTTON_RIGHT) {
            button = VK_RBUTTON;
        }
        else if (mouse_button & MOUSE_BUTTON_MIDDLE) {
            button = VK_MBUTTON;
        }
        if (button != 0) {
            bool is_down = false;
            if (mouse_button & MOUSE_BUTTON_DOWN) {
                is_down = true;
            }

            if (is_down
                && current_pressed_mouse_button != button) {
                current_pressed_mouse_button = button;
            }
            else if (!is_down
                && current_pressed_mouse_button != 0) {
                current_pressed_mouse_button = 0;
            }

            vecommon::MouseKeyData key;
            key.key = button;
            key.down = is_down;
            key.absX = (point.x - valid_area.left) * 65535 / valid_area_width;
            key.absY = (point.y - valid_area.top) * 65535 / valid_area_height;

            mouse_service->sendButtonEvent(key);
        }

        // mouse wheel
        if (mouse_button & MOUSE_WHEEL) {
            vecommon::MouseWheelData wheel;
            wheel.wheel = (short)LOWORD(raw->data.mouse.usButtonData);
            wheel.hwheel = 0;
            mouse_service->sendWheelEvent(wheel);
        }
    }
}

void cloudrenderx_demo::CloudRenderxGameSession::captureWMKeyboard(WPARAM wparam, LPARAM lparam, bool down)
{
    USHORT curr_key = static_cast<USHORT>(wparam);
    curr_key = processExtendsCode(wparam, lparam);
    //veLOGE("send key code : {}, the action is {}", curr_key, down);

    if (currentGameSession) {
        auto keyboard_service = currentGameSession->getKeyboardService();
        if (keyboard_service) {
            // todo VK_PROCESSKEY的处理
            vecommon::KeyboardData key;
            key.key = curr_key;
            key.down = down ? 1 : 0;
            key.processed = false;
            keyboard_service->sendKeyboardvent(key);
        }
    }
}

void cloudrenderx_demo::CloudRenderxGameSession::updateValidArea()
{
    if (video_frame_width == 0 || video_frame_height == 0) {
        veLOGE("frame size from pod is invalid : width = {}, height = {}.", video_frame_width, video_frame_height);
        return;
    }

    if (!currentWnd) {
        return;
    }

    WINDOWINFO info = { sizeof(WINDOWINFO) };
    GetWindowInfo(currentWnd, &info);
    int client_width = info.rcClient.right - info.rcClient.left;
    int client_height = info.rcClient.bottom - info.rcClient.top;

    int picture_width, picture_height;
    int gap_width, gap_height;
    if (client_width * video_frame_height > client_height * video_frame_width) {  // 客户区宽了
        valid_area.top = info.rcClient.top;
        valid_area.bottom = info.rcClient.bottom;

        picture_height = client_height;
        picture_width = picture_height * video_frame_width / video_frame_height;
        gap_width = (client_width - picture_width) / 2;

        valid_area.left = info.rcClient.left + gap_width;
        valid_area.right = info.rcClient.right - gap_width;
    }
    else {    // 客户区高了
        valid_area.left = info.rcClient.left;
        valid_area.right = info.rcClient.right;

        picture_width = client_width;
        picture_height = picture_width * video_frame_height / video_frame_width;
        gap_height = (client_height - picture_height) / 2;

        valid_area.top = info.rcClient.top + gap_height;
        valid_area.bottom = info.rcClient.bottom - gap_height;
    }
}

void cloudrenderx_demo::CloudRenderxGameSession::updateVideoFrameSize(int width, int height)
{
    bool hasChange = false;
    if (video_frame_width != width) {
        video_frame_width = width;
        hasChange = true;
    }

    if (video_frame_height != height) {
        video_frame_height = height;
        hasChange = true;
    }

    if (hasChange) {
        updateValidArea();
    }
}

USHORT cloudrenderx_demo::CloudRenderxGameSession::processExtendsCode(USHORT key, USHORT make_code, USHORT flags)
{
    USHORT ret_key = key;
    switch (ret_key) {
    case VK_SHIFT: {
        if (make_code == 0x2a) {
            ret_key = VK_LSHIFT;
        }
        else if (make_code == 0x36) {
            ret_key = VK_RSHIFT;
        }
        break;
    } case VK_CONTROL: {
        ret_key = (flags & 0x2) ? VK_RCONTROL : VK_LCONTROL;
        break;
    } case VK_MENU: {
        ret_key = (flags & 0x2) ? VK_RMENU : VK_LMENU;
        break;
    } default: {
        break;
    }
    }
    return ret_key;
}

USHORT cloudrenderx_demo::CloudRenderxGameSession::processExtendsCode(WPARAM wparam, LPARAM lparam)
{
    USHORT ret_key = static_cast<USHORT>(wparam);
    switch (ret_key) {
    case VK_SHIFT: {
        if ((lparam >> 16 & 0xff) == 0x2a) {
            ret_key = VK_LSHIFT;
        }
        else if ((lparam >> 16 & 0xff) == 0x36) {
            ret_key = VK_RSHIFT;
        }
        break;
    } case VK_CONTROL: {
        ret_key = (lparam >> 24 & 0x1) ? VK_RCONTROL : VK_LCONTROL;
        break;
    } case VK_MENU: {
        ret_key = (lparam >> 24 & 0x1) ? VK_RMENU : VK_LMENU;
        break;
    } default: {
        break;
    }
    }
    return ret_key;
}
