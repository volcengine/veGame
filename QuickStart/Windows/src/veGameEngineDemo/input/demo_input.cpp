#include "pch.h"
#include "demo_input.h"
#include "../demo_core.h"
#include "../demo_utils.h"

static constexpr int k_mouse_edge = 65536;

DemoInput::DemoInput() {
}

DemoInput::~DemoInput() {
    uninit();
}

void DemoInput::init(HWND hwnd) {
    regRawInput(hwnd);

    himc_ = ImmAssociateContext(hwnd, NULL);
    if (himc_ == NULL) {
        veLOGW("ImmGetContext failed");
    }

    if (!RegisterTouchWindow(hwnd, 0)) {
        veLOGW("RegisterTouchWindow failed");
    }
}

void DemoInput::uninit() {
}

void DemoInput::regRawInput(HWND hwnd) {
    RAWINPUTDEVICE raw_input_device[2];
    raw_input_device[0].usUsagePage = HID_USAGE_PAGE_GENERIC;   // 设备类
    raw_input_device[0].usUsage = HID_USAGE_GENERIC_MOUSE;      // 设备类内的具体设备，鼠标
    raw_input_device[0].dwFlags = NULL;
    raw_input_device[0].hwndTarget = hwnd;

    raw_input_device[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
    raw_input_device[1].usUsage = HID_USAGE_GENERIC_GAMEPAD;
    raw_input_device[1].dwFlags = NULL;
    raw_input_device[1].hwndTarget = hwnd;

    if (!RegisterRawInputDevices(raw_input_device, 2, sizeof(RAWINPUTDEVICE))) {
        veLOGW("RegisterRawInputDevices failed.");
    }
}

void DemoInput::processHotKey(USHORT key, bool down) {
    if (!down) {
        return;
    }

    // ctrl hot key
    if (GetKeyState(VK_CONTROL) & 0x8000) {
        if (key == 'C') {

        } else if (key == 'V') {
            std::string tmp_str = getClipboardString();
            sendImeString(gbkToUtf8(tmp_str));
        } else if (GetKeyState(VK_MENU) & 0x8000) {
            if (!DemoCore::instance().isFullScreen()) {
                setClientStatus(WA_KEYBOARD_INACTIVE);
            }
        }
    } else if (GetKeyState(VK_MENU) & 0x8000) {
        if (key == VK_RETURN) {
            DemoCore::instance().switchFullScreen();
            setClientStatus(WA_GENERAL_ACTIVE);
        }
    }
}

void DemoInput::captureRawInput(LPARAM lparam) {
    UINT dw_size = 0;
    GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &dw_size, (UINT)sizeof(RAWINPUTHEADER));  // 取数据,第一次调用函数将获取需要的字节大小
    if (dw_size == 0) {
        return;
    }
    if (dw_size > dw_size_) {
        lpb_buffer_.reset(new BYTE[dw_size]);
        dw_size_ = dw_size;
    }
    GetRawInputData((HRAWINPUT)lparam, RID_INPUT, (LPVOID)lpb_buffer_.get(), (PUINT)&dw_size_, (UINT)sizeof(RAWINPUTHEADER));   // 第二次调用获取原始输入数据，读入lpbBuffer

    RAWINPUT* raw = (RAWINPUT*)lpb_buffer_.get();
    if (raw->header.dwType == RIM_TYPEMOUSE) {
        captureRawInputMouse(raw);
    } else if (raw->header.dwType == RIM_TYPEHID) {
        RID_DEVICE_INFO device_info = { sizeof(RID_DEVICE_INFO) };
        int min_sz = sizeof(RID_DEVICE_INFO);
        int ret = GetRawInputDeviceInfo(raw->header.hDevice, RIDI_DEVICEINFO, &device_info, (PUINT)&min_sz);
        if (ret <= 0) {
            veLOGE("CaptureRawInput GetRawInputDeviceInfo error.");
            return;
        }

        if (device_info.hid.usUsage == HID_USAGE_GENERIC_GAMEPAD) {
            captureRawInputGamepad();
        }
    }
}

void DemoInput::captureRawInputGamepad() {
    if (!DemoCore::instance().isGaming()
            || !DemoCore::instance().isClientForeground()) {
        return;
    }

    XINPUT_STATE state_new = { 0 };
    for (int id = 0; id < XUSER_MAX_COUNT; ++id) {
        DWORD dw_result = XInputGetState(id, &state_new);
        // 有些手柄会不停的改 dwPacketNumber，一直发消息，直接判断 gamepad 内容是否有变化
        if (dw_result == ERROR_SUCCESS && memcmp(&state_new.Gamepad, &xinput_state_[id].Gamepad, sizeof(state_new.Gamepad))) {
            xinput_state_[id] = state_new;

            vegame::GamepadInputData data;
            data.index = id;
            data.buttons = state_new.Gamepad.wButtons;
            data.lt = state_new.Gamepad.bLeftTrigger;
            data.rt = state_new.Gamepad.bRightTrigger;
            data.lx = state_new.Gamepad.sThumbLX;
            data.ly = state_new.Gamepad.sThumbLY;
            data.rx = state_new.Gamepad.sThumbRX;
            data.ry = state_new.Gamepad.sThumbRY;
            DemoCore::instance().demoIns()->sendInputGamepad(data);
            //dbgLOG->info("gamepad index {}, buttons 0x{:x}, lt {}, rt {}, lx {}, ly {}, rx {}, ry {}",
            //    data.index, data.buttons, data.lt, data.rt, data.lx, data.ly, data.rx, data.ry);
        }
    }
}

void DemoInput::captureRawInputKeyboard(RAWINPUT* raw) {
    USHORT curr_key = raw->data.keyboard.VKey;
    curr_key = processExtendsCode(curr_key, raw->data.keyboard.MakeCode, raw->data.keyboard.Flags);

    // 过滤某些键盘
    if (curr_key == 0xff) {
        return;
    }

    int action = 1;     // key_down
    if (raw->data.keyboard.Message == WM_KEYUP || raw->data.keyboard.Message == WM_SYSKEYUP) {
        action = 0;
    }
    // 按了某些快捷键游戏失活后，消除按键状态
    key_down_[curr_key] = action;

    processHotKey(curr_key, action);

    // dbgLOG->info("send binary msg: key{} state{}", curr_key, action);
    vegame::KeyboardData data;
    data.key = static_cast<uint8_t>(curr_key);
    data.down = action;
    DemoCore::instance().demoIns()->sendInputKeyboard(data);
}

USHORT DemoInput::processExtendsCode(USHORT key, USHORT make_code, USHORT flags) {
    USHORT ret_key = key;
    switch (ret_key) {
        case VK_SHIFT: {
            if (make_code == 0x2a) {
                ret_key = VK_LSHIFT;
            } else if (make_code == 0x36) {
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

USHORT DemoInput::processExtendsCode(WPARAM wparam, LPARAM lparam) {
    USHORT ret_key = static_cast<USHORT>(wparam);
    switch (ret_key) {
        case VK_SHIFT: {
            if ((lparam >> 16 & 0xff) == 0x2a) {
                ret_key = VK_LSHIFT;
            } else if ((lparam >> 16 & 0xff) == 0x36) {
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

void DemoInput::captureRawInputMouse(RAWINPUT* raw) {
    USHORT mouse_button = raw->data.mouse.usButtonFlags;
    if (mouse_button & MOUSE_BUTTON_DOWN) {
        setClientStatus(WA_MOUSE_ACTIVE);
    }

    if (!DemoCore::instance().isGaming()
            || !DemoCore::instance().isClientForeground()) {
        return;
    }

    // 确定鼠标当前在游戏画面中的相对位置 0.00 - 1.00;
    POINT point;
    GetCursorPos(&point);
    // 鼠标不在游戏画面中，则不传输该消息，游戏画面内，鼠标判断为一个左闭右开区间，加上一个像素，在边界时还能传递delta
    RECT picture_rect = DemoCore::instance().getPictureRect();
    RECT mouse_rect = picture_rect;
    mouse_rect.right += 1;
    mouse_rect.bottom += 1;
    if (!PtInRect(&mouse_rect, point)) {
        return;
    }
    int picture_width = picture_rect.right - picture_rect.left;
    int picture_height = picture_rect.bottom - picture_rect.top;

    // mouse move
    if (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0) {
        vegame::MouseMoveData move;
        if (raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) {
            move.abs_x = raw->data.mouse.lLastX;
            move.abs_y = raw->data.mouse.lLastY;
            DemoCore::instance().demoIns()->sendInputMouseMove(move);
        } else {
            move.abs_x = (point.x - picture_rect.left) * k_mouse_edge / picture_width;
            move.abs_y = (point.y - picture_rect.top) * k_mouse_edge / picture_height;
            move.delta_x = raw->data.mouse.lLastX;
            move.delta_y = raw->data.mouse.lLastY;
            DemoCore::instance().demoIns()->sendInputMouseMove(move);
        }
    }

    // mouse key
    uint8_t button = 0;
    if (mouse_button & MOUSE_BUTTON_LEFT) {
        button = VK_LBUTTON;
    } else if (mouse_button & MOUSE_BUTTON_RIGHT) {
        button = VK_RBUTTON;
    } else if (mouse_button & MOUSE_BUTTON_MIDDLE) {
        button = VK_MBUTTON;
    }
    if (button != 0) {
        bool is_down = false;
        if (mouse_button & MOUSE_BUTTON_DOWN) {
            is_down = true;
        }

        vegame::MouseKeyData key;
        key.key = button;
        key.down = is_down;
        key.abs_x = (point.x - picture_rect.left) * k_mouse_edge / picture_width;
        key.abs_y = (point.y - picture_rect.top) * k_mouse_edge / picture_height;
        DemoCore::instance().demoIns()->sendInputMouseKey(key);
    }

    // mouse wheel
    if (mouse_button & MOUSE_WHEEL) {
        vegame::MouseWheelData wheel;
        wheel.wheel = (short)LOWORD(raw->data.mouse.usButtonData);
        wheel.hwheel = 0;
        DemoCore::instance().demoIns()->sendInputMouseWheel(wheel);
    }
}

void DemoInput::captureWMTouch(WPARAM wparam, LPARAM lparam) {
    if (!DemoCore::instance().isGaming()
            || !DemoCore::instance().isClientForeground()) {
        return;
    }

    UINT num_inputs = (UINT)wparam;
    TOUCHINPUT* ti = new TOUCHINPUT[num_inputs];
    if (ti == NULL) {
        veLOGE("CaptureWMTouch error.");
        return;
    }

    if (!GetTouchInputInfo((HTOUCHINPUT)lparam, num_inputs, ti, sizeof(TOUCHINPUT))) {
        veLOGE("GetTouchInputInfo error.");
        delete[] ti;
        return;
    }

    RECT picture_rect = DemoCore::instance().getPictureRect();
    int picture_width = picture_rect.right - picture_rect.left;
    int picture_height = picture_rect.bottom - picture_rect.top;
    if (picture_width == 0 || picture_height == 0) {
        veLOGE("m_picture_rect width or height is zero");
        CloseTouchInputHandle((HTOUCHINPUT)lparam);
        delete[] ti;
        return;
    }

    std::vector<uint8_t> tmp(sizeof(vegame::TouchArrayData) + num_inputs * sizeof(vegame::TouchData), 0);
    vegame::TouchArrayData* touch_array = reinterpret_cast<vegame::TouchArrayData*>(tmp.data());
    touch_array->count = num_inputs;
    for (UINT pointer_id = 0; pointer_id < num_inputs; ++pointer_id) {
        vegame::TouchType action = vegame::TouchType::None;
        if (ti[pointer_id].dwFlags & TOUCHEVENTF_DOWN) {
            action = vegame::TouchType::Down;
        } else if (ti[pointer_id].dwFlags & TOUCHEVENTF_UP) {
            action = vegame::TouchType::Up;
        } else if (ti[pointer_id].dwFlags & TOUCHEVENTF_MOVE) {
            action = vegame::TouchType::Move;
        }
        vegame::TouchData touch;
        touch.index = (int16_t)ti[pointer_id].dwID;
        touch.type = action;
        touch.abs_x = (ti[pointer_id].x / 100 - picture_rect.left) * k_mouse_edge / picture_width;
        touch.abs_y = (ti[pointer_id].y / 100 - picture_rect.top) * k_mouse_edge / picture_height;
        touch_array->touch[pointer_id] = std::move(touch);
    }
    DemoCore::instance().demoIns()->sendInputTouch(*touch_array);

    CloseTouchInputHandle((HTOUCHINPUT)lparam);
    delete[] ti;
}

void DemoInput::setClientStatus(DWORD active_state) {
    if (!DemoCore::instance().isGaming()) {
        return;
    }

    // 获取 player 窗体样式
    LONG_PTR player_style = GetWindowLongPtr(DemoCore::instance().getMainWnd(), GWL_STYLE);
    switch (active_state) {
        case WA_INACTIVE:
        case WA_KEYBOARD_INACTIVE: {
            clearDownKeys();
            setLocalCursorShow(true);
            setCursorLock(false);
            saveCursorPos();
            DemoCore::instance().setClientState(false);
            if (player_style && !DemoCore::instance().isFullScreen()) {
                SetWindowLongPtr(DemoCore::instance().getMainWnd(), GWL_STYLE, player_style | WS_SIZEBOX);
            }
            break;
        }
        case WA_ACTIVE:
        case WA_GENERAL_ACTIVE:
        case WA_CLICKACTIVE:
        case WA_MOUSE_ACTIVE: {
            CURSORINFO info = { sizeof(CURSORINFO) };
            GetCursorInfo(&info);
            bool ret = PtInRect(&DemoCore::instance().getPictureRect(), info.ptScreenPos);
            // 鼠标不在客户区时，不认为激活
            if (!ret) {
                return;
            }

            setCursorLock(true);
            setLocalCursorShow(cursor_show_);
            DemoCore::instance().setClientState(true);
            if (active_state == WA_ACTIVE || active_state == WA_GENERAL_ACTIVE) {
                // SetCursorPos(cursor_pos_x_, cursor_pos_y_);
            }
            if (player_style != 0) {
                SetWindowLongPtr(DemoCore::instance().getMainWnd(), GWL_STYLE, player_style & ~WS_SIZEBOX);
            }
            break;
        }
        default: {
            break;
        }
    }
}

void DemoInput::clearDownKeys() {
    for (short key = 0; key < 256; ++key) {
        if (key_down_[key] == true) {
            key_down_[key] = false;
            clearDownKeyboard(key);
        }
    }
}

void DemoInput::clearDownKeyboard(USHORT key_code) {
    vegame::KeyboardData data;
    data.key = static_cast<uint8_t>(key_code);
    data.down = false;
    DemoCore::instance().demoIns()->sendInputKeyboard(data);
}

void DemoInput::setCursorLock(bool lock) {
    if (lock) {
        ClipCursor(&DemoCore::instance().getPictureRect());
    } else {
        ClipCursor(nullptr);
    }
}

void DemoInput::setCursorState(bool show) {
    cursor_show_ = show;
}

void DemoInput::setLocalCursorShow(bool show) {
    if (show) {
        while (ShowCursor(true) < 0);
    } else {
        while (ShowCursor(false) >= 0);
    }
}

void DemoInput::setImeState(const vegame::ImeStateData& data) {
    if (ime_state_.enable == data.enable) {
        return;
    }

    veLOGI("set ime state: {}", data.enable);
    ime_state_ = data;
}

void DemoInput::setImeAssociateState(bool active) {
    if (active) {
        ImmAssociateContext(DemoCore::instance().getMainWnd(), himc_);
    } else {
        ImmAssociateContext(DemoCore::instance().getMainWnd(), nullptr);
    }
}

void DemoInput::captureCompositionString(vegame::ImeCompositionType type, LPARAM lparam) {
    if (!DemoCore::instance().isGaming()
            || !DemoCore::instance().isClientForeground()) {
        return;
    }

    if (!ime_state_.enable) {
        return;
    }

    static std::string tmp_comp_res_str;
    vegame::ImeCompositionData ime;
    ime.type = type;
    ime.str = nullptr;
    std::string tmp_comp_str;
    if (type == vegame::ImeCompositionType::Begin) {
        tmp_comp_res_str.clear();
    } else if (type == vegame::ImeCompositionType::End) {
        ime.str = tmp_comp_res_str.c_str();
    } else if (type == vegame::ImeCompositionType::Input) {
        // ImmSetCompositionWindow
        if (ime_state_.w == 0 || ime_state_.h == 0) {
            veLOGI("ime state width or height is zero");
            return;
        }
        auto& picture_rect = DemoCore::instance().getPictureRect();
        COMPOSITIONFORM cf = { CFS_POINT };
        int client_width = picture_rect.right - picture_rect.left;
        int client_height = picture_rect.bottom - picture_rect.top;
        int left = ime_state_.x * client_width / ime_state_.w;
        int top = ime_state_.y * client_height / ime_state_.h;

        cf.ptCurrentPos = POINT{ left, top + ime_state_.fh + 2 };
        if (false == ImmSetCompositionWindow(himc_, &cf)) {
            veLOGW("Set Composition Window failed");
        }

        // ImmSetCompositionFontA
        LOGFONTA lf;
        bool b_ret = ImmGetCompositionFontA(himc_, &lf);
        lf.lfHeight = ime_state_.fh * client_height / ime_state_.h;
        lf.lfWidth = ime_state_.fw * client_width / ime_state_.w;
        lf.lfQuality = DEFAULT_QUALITY;
        b_ret = ImmSetCompositionFontA(himc_, &lf);

        // ImmGetCompositionStringA
        LONG len = 0;
        if (lparam & GCS_COMPSTR) {
            len = ImmGetCompositionStringA(himc_, GCS_COMPSTR, NULL, 0);
            tmp_comp_str.resize(len);
            len = ImmGetCompositionStringA(himc_, GCS_COMPSTR, tmp_comp_str.data(), len);
        } else if (lparam & GCS_RESULTSTR) {
            len = ImmGetCompositionStringA(himc_, GCS_RESULTSTR, NULL, 0);
            tmp_comp_str.resize(len);
            len = ImmGetCompositionStringA(himc_, GCS_RESULTSTR, tmp_comp_str.data(), len);
        }
        if (len == IMM_ERROR_NODATA) {
            veLOGW("ImmGetCompositionString failed, error: IMM_ERROR_NODATA");
            return;
        } else if (len == IMM_ERROR_GENERAL) {
            veLOGW("ImmGetCompositionString failed, error: IMM_ERROR_GENERAL");
            return;
        } else if (len <= 0) {
            veLOGI("ImmGetCompositionString get string len is zero");
            return;
        }

        if (lparam & GCS_RESULTSTR) {
            tmp_comp_str = gbkToUtf8(tmp_comp_str);
            tmp_comp_res_str = tmp_comp_str;
            ime.str = tmp_comp_str.c_str();
        }
    }
    // dbgLOG->info("ime type {}, str {}", ime.type, cg::utf8ToGbk(tmp_comp_str));

    DemoCore::instance().demoIns()->sendInputImeComposition(ime);
}

void DemoInput::sendImeString(const std::string& str) {
    if (!DemoCore::instance().isGaming()
            || !DemoCore::instance().isClientForeground()) {
        return;
    }

    vegame::ImeCompositionData ime;
    ime.type = vegame::ImeCompositionType::Begin;
    ime.str = nullptr;
    DemoCore::instance().demoIns()->sendInputImeComposition(ime);

    ime.type = vegame::ImeCompositionType::Input;
    ime.str =  str.c_str();
    DemoCore::instance().demoIns()->sendInputImeComposition(ime);

    ime.type = vegame::ImeCompositionType::End;
    ime.str = nullptr;
    DemoCore::instance().demoIns()->sendInputImeComposition(ime);
}

void DemoInput::captureWMKeyboard(WPARAM wparam, LPARAM lparam, bool down) {
    if (!DemoCore::instance().isGaming()
            || !DemoCore::instance().isClientForeground()) {
        return;
    }

    USHORT curr_key = static_cast<USHORT>(wparam);
    // 过滤某些键盘
    if (curr_key == 0xff) {
        return;
    }
    // 处理快捷键
    processHotKey(curr_key, down);
    curr_key = processExtendsCode(wparam, lparam);

    // warning: 临时屏蔽 ctrl+v
    if (GetKeyState(VK_CONTROL) & 0x8000) {
        if (curr_key == 'V') {
            return;
        }
    }

    // 按了某些快捷键游戏失活后，消除按键状态
    key_down_[curr_key] = down;

    vegame::KeyboardData data;
    data.key = static_cast<uint8_t>(curr_key);
    data.down = !!down;
    DemoCore::instance().demoIns()->sendInputKeyboard(data);
}

void DemoInput::saveCursorPos() {
    CURSORINFO info = { sizeof(CURSORINFO) };
    GetCursorInfo(&info);
    cursor_pos_x_ = static_cast<int>(info.ptScreenPos.x);
    cursor_pos_y_ = static_cast<int>(info.ptScreenPos.y);
}
