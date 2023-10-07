#pragma once
#include "veGame/vegame_type_defines.h"

class DemoInput : public SingletonInSameModule<DemoInput> {
    friend class SingletonInSameModule<DemoInput>;
public:
    DemoInput();
    virtual ~DemoInput();

    void init(HWND hwnd);
    void uninit();

    // 捕捉 wm_input 消息
    void captureRawInput(LPARAM lparam);
    // 处理触屏输入
    void captureWMTouch(WPARAM wparam, LPARAM lparam);

    // 客户端激活状态变化，0:WA_INACTIVE, 1:WA_ACTIVE, 2:WA_CLICKACTIVE, // 3:KEY(INACTIVE) 4:MOUSE(ACTIVE) 5:GENERAL(ACTIVE)
    void setClientStatus(DWORD state);

    // 框定鼠标位置，框在游戏画面内
    void setCursorLock(bool lock);
    // 改变游戏时的鼠标状态，由 pod 端同步
    void setCursorState(bool show);
    // 不受是否 is gaming 影响，改变本地鼠标状态
    void setLocalCursorShow(bool show);

    // 输入法相关
    void setImeState(const vegame::ImeStateData& data);
    void setImeAssociateState(bool active);
    void captureCompositionString(vegame::ImeCompositionType type, LPARAM lparam = 0);
    void sendImeString(const std::string& str);
    void captureWMKeyboard(WPARAM wparam, LPARAM lparam, bool down);
    
protected:
    // 注册 raw_input 设备
    void regRawInput(HWND hwnd);
    // process hot key
    void processHotKey(USHORT key, bool down);
    // 用Xinput捕捉手柄消息
    void captureRawInputGamepad();
    // 处理 raw input 键盘输入
    void captureRawInputKeyboard(RAWINPUT* raw);
    // 左右键处理，wm_input and wm_keydown
    USHORT processExtendsCode(USHORT key, USHORT make_code, USHORT flags);
    USHORT processExtendsCode(WPARAM wparam, LPARAM lparam);
    // 处理鼠标输入
    void captureRawInputMouse(RAWINPUT* raw);

    // 保存鼠标光标位置
    void saveCursorPos();

    // 按键切换游戏状态时，清除现有按键down状态及游戏状态
    void clearDownKeys();
    void clearDownKeyboard(USHORT key);

private:
    // 接收raw input数据
    std::shared_ptr<BYTE> lpb_buffer_ = nullptr;
    // 保存lpb_buffer大小
    UINT dw_size_ = 0;

    // 保存游戏时鼠标指针是否隐藏
    bool cursor_show_ = true;
    // 保存光标位置
    int cursor_pos_x_ = 0;
    int cursor_pos_y_ = 0;

    // 保存手柄上一个状态，用于判断是否按键抬起
    XINPUT_STATE xinput_state_[XUSER_MAX_COUNT] = { 0 };

    // 保存功能键按下时，同时按下的其他键
    std::vector<bool> key_down_ = std::vector<bool>(256, false);

    // 保存禁用输入法时的himc
    HIMC himc_;
    vegame::ImeStateData ime_state_ = { 0 };
};

// 鼠标隐藏消息
static const USHORT WM_USER_CURSOR_SHOW = WM_USER + 2;
// 输入法状态消息
static const USHORT WM_USER_IME_ACTIVE = WM_USER + 4;

// CursorLock消息
static const DWORD WA_KEYBOARD_INACTIVE = 4;
static const DWORD WA_MOUSE_ACTIVE = 5;
static const DWORD WA_GENERAL_ACTIVE = 6;


//鼠标按键值合并
static const USHORT MOUSE_WHEEL = RI_MOUSE_WHEEL;
static const USHORT MOUSE_BUTTON_LEFT = RI_MOUSE_LEFT_BUTTON_DOWN | RI_MOUSE_LEFT_BUTTON_UP;
static const USHORT MOUSE_BUTTON_RIGHT = RI_MOUSE_RIGHT_BUTTON_DOWN | RI_MOUSE_RIGHT_BUTTON_UP;
static const USHORT MOUSE_BUTTON_MIDDLE = RI_MOUSE_MIDDLE_BUTTON_DOWN | RI_MOUSE_MIDDLE_BUTTON_UP;
static const USHORT MOUSE_BUTTON_DOWN = RI_MOUSE_LEFT_BUTTON_DOWN | RI_MOUSE_RIGHT_BUTTON_DOWN | RI_MOUSE_MIDDLE_BUTTON_DOWN;
static const USHORT MOUSE_BUTTON_UP = RI_MOUSE_LEFT_BUTTON_UP | RI_MOUSE_RIGHT_BUTTON_UP | RI_MOUSE_MIDDLE_BUTTON_UP;
