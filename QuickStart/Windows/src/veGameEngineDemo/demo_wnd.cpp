#include "pch.h"
#include "res/resource.h"
#include "demo_core.h"
#include "input/demo_input.h"
#include "demo_utils.h"

#define MAX_LOADSTRING 100

// 全局变量:
static HINSTANCE g_inst;                               // 当前实例
static WCHAR g_wnd_title[MAX_LOADSTRING];              // 标题栏文本
static WCHAR g_wnd_class[MAX_LOADSTRING];              // 主窗口类名
static WCHAR g_edit_value[MAX_LOADSTRING];             // 对话框文本内容
static constexpr int g_binary_data_size = 1024 * 60;        // 二进制数据大小，rtc 的限制
static char g_binary_data_buf[g_binary_data_size] = { 0 };  // 二进制数据 buffer

// 此代码模块中包含的函数的前向声明:
ATOM                myRegisterClass(HINSTANCE inst);
BOOL                initInstance(HINSTANCE, int);
LRESULT CALLBACK    wndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    dialogGetValue(HWND, UINT, WPARAM, LPARAM);
void processWMCommand(HWND wnd, WPARAM wparam, LPARAM lparam);
void processMenuItemInfo(HWND wnd, UINT command, std::function<void (bool)> func);

int APIENTRY wWinMain(_In_ HINSTANCE inst,
    _In_opt_ HINSTANCE prev_inst,
    _In_ LPWSTR    cmd_line,
    _In_ int       cmd_show) {
    UNREFERENCED_PARAMETER(prev_inst);
    UNREFERENCED_PARAMETER(cmd_line);

    // TODO: 在此处放置代码。
    // 初始化CrashRecorder
    InitUnhandledExceptionFilter();

    // 初始化全局字符串
    LoadStringW(inst, IDS_APP_TITLE, g_wnd_title, MAX_LOADSTRING);
    LoadStringW(inst, IDC_VEGAMEENGINEDEMO, g_wnd_class, MAX_LOADSTRING);

    // 设置日志文件名
    setUpLocalSpdLog(".\\log\\vegame_demo." + std::to_string(GetCurrentProcessId()) + ".log", "vegame_demo");

    myRegisterClass(inst);

    // 执行应用程序初始化:
    if (!initInstance(inst, cmd_show)) {
        return FALSE;
    }

    MSG msg;
    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// 实例化托盘
static const int WM_TRAY = WM_USER + 100;
static char app_name[MAX_LOADSTRING] = { 0 };
void initTray(HINSTANCE inst, HWND hwnd) {
    LoadStringA(inst, IDS_APP_TITLE, app_name, MAX_LOADSTRING);
    NOTIFYICONDATAA nid;

    nid.cbSize = sizeof(NOTIFYICONDATAA);
    nid.hWnd = hwnd;
    nid.uID = IDI_VEGAMEENGINEDEMO;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
    nid.uCallbackMessage = WM_TRAY;
    nid.hIcon = LoadIcon(inst, MAKEINTRESOURCE(IDI_VEGAMEENGINEDEMO));
    lstrcpyA(nid.szTip, app_name);
    lstrcpyA(nid.szInfoTitle, app_name);
    lstrcpyA(nid.szInfo, "start veGameEngineDemo");

    // 显示托盘
    Shell_NotifyIconA(NIM_ADD, &nid);

    DemoCore::instance().setNotifyIconData(nid);
}

//
//  函数: MyRegisterClass()
//  目标: 注册窗口类。
//
ATOM myRegisterClass(HINSTANCE inst) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = wndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = inst;
    wcex.hIcon = LoadIcon(inst, MAKEINTRESOURCE(IDI_VEGAMEENGINEDEMO));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VEGAMEENGINEDEMO);
    wcex.lpszClassName = g_wnd_class;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//   目标: 保存实例句柄并创建主窗口
//   注释:
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL initInstance(HINSTANCE inst, int cmd_show) {
    g_inst = inst; // 将实例句柄存储在全局变量中

    // Create window
    HWND hwnd = CreateWindowW(g_wnd_class, g_wnd_title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, inst, nullptr);
    if (!hwnd) {
        return false;
    }
    // resize window
    POINT pt;
    if (!GetCursorPos(&pt)) {
        veLOGW("GetCursorPos failed, error: {}", GetLastError());
    }
    HMONITOR hmonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
    DemoCore::instance().resizeWindowWithShowWH(hwnd, hmonitor, 1280, 720, true);

    // 设置 HWND
    DemoCore::instance().setMainWnd(hwnd);
    DemoCore::instance().updatePictureRect();
    DemoInput::instance().init(hwnd);
    // 
    DemoCore::instance().demoLayer()->open(inst, hwnd);

    ShowWindow(hwnd, cmd_show);
    UpdateWindow(hwnd);

    // 实例化托盘
    initTray(inst, hwnd);

    return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//  目标: 处理主窗口的消息。
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
LRESULT CALLBACK wndProc(HWND wnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_TOUCH: {
            DemoInput::instance().captureWMTouch(wparam, lparam);
            break;
        }
        case WM_INPUT: {
            DemoInput::instance().captureRawInput(lparam);
            break;
        }
        case WM_CHAR: {
            // dbgLOG->info("char 0x{:x}, lp 0x{:x}", wparam, lparam);
            // 过滤有效按键
            if ((lparam >> 16) & 0xff) {
                 break;
            }
            DemoInput::instance().sendImeString(unicodeToUtf8(std::wstring(1, (WCHAR)wparam)));
            break;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN: {
            // dbgLOG->info("key 0x{:x} down", wparam);
            if (wparam == VK_PROCESSKEY || wparam == VK_PACKET) {
                break;
            }
            DemoInput::instance().captureWMKeyboard(wparam, lparam, true);
            break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            if (wparam == VK_PROCESSKEY || wparam == VK_PACKET) {
                break;
            }
            DemoInput::instance().captureWMKeyboard(wparam, lparam, false);
            break;
        }
        case WM_IME_STARTCOMPOSITION: {
            DemoInput::instance().captureCompositionString(vegame::ImeCompositionType::Begin);
            break;
        }
        case WM_IME_ENDCOMPOSITION: {
            DemoInput::instance().captureCompositionString(vegame::ImeCompositionType::End);
            break;
        }
        case WM_IME_COMPOSITION: {
            DemoInput::instance().captureCompositionString(vegame::ImeCompositionType::Input, lparam);
            break;
        }
        case WM_COMMAND: {
            processWMCommand(wnd, wparam, lparam);
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(wnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(wnd, &ps);
            break;
        }
        case WM_MOVE:
        case WM_SIZE: {
            DemoCore::instance().updatePictureRect();
            DemoCore::instance().demoLayer()->onPoseSizeChanged();
            break;
        }
        case WM_USER_CURSOR_SHOW: {
            DemoInput::instance().setCursorState((bool)wparam);
            if (DemoCore::instance().isClientForeground()) {
                DemoInput::instance().setLocalCursorShow((bool)wparam);
            }
            if (!DemoCore::instance().isGaming()) {
                DemoInput::instance().setLocalCursorShow(true);
            }
            break;
        }
        case WM_USER_IME_ACTIVE: {
            DemoInput::instance().setImeAssociateState((bool)wparam);
            break;
        }
        case WM_ACTIVATE: {
            bool active = LOWORD(wparam);
            DemoCore::instance().demoLayer()->onParentActive(active);
            DemoInput::instance().setClientStatus(active);
            break;
        }
        case WM_DESTROY: {
            DemoInput::instance().uninit();
            NOTIFYICONDATAA& nid = DemoCore::instance().getNotifyIconData();
            Shell_NotifyIconA(NIM_DELETE, &nid);
            PostQuitMessage(0);
            break;
        }
        default: {
            break;
        }
    }
    return DefWindowProc(wnd, message, wparam, lparam);
}

void processWMCommand(HWND wnd, WPARAM wparam, LPARAM lparam) {
    DemoInput::instance().setClientStatus(WA_INACTIVE);
    int wmid = LOWORD(wparam);
    // 分析菜单选择:
    switch (wmid) {
        case ID_VEGAME_START: {
            DemoCore::instance().demoIns()->start();
            break;
        }
        case ID_VEGAME_STOP: {
            DemoCore::instance().demoIns()->stop();
            break;
        }
        case ID_VEGAME_RESTART: {
            DemoCore::instance().demoIns()->restart();
            break;
        }
        case ID_PLAY_PAUSEAVSTREAM: {
            DemoCore::instance().demoIns()->pauseAVStream();
            break;
        }
        case ID_PLAY_RESUMEAVSTREAM: {
            DemoCore::instance().demoIns()->resumeAVStream();
            break;
        }
        case ID_PLAY_SETAUDIOMUTE: {
            processMenuItemInfo(wnd, ID_PLAY_SETAUDIOMUTE, std::function<void (bool)>(std::bind(&DemoInstance::setAudioMute, DemoCore::instance().demoIns(), std::placeholders::_1)));
            break;
        }
        case ID_PLAY_STARTSENDAUDIOSTREAM: {
            DemoCore::instance().demoIns()->startSendAudioStream();
            break;
        }
        case ID_PLAY_STOPSENDAUDIOSTREAM: {
            DemoCore::instance().demoIns()->stopSendAudioStream();
            break;
        }
        case ID_TEST_SENDSTRINGMESSAGE: {
            DemoCore::instance().demoIns()->sendStringMessage(getClipboardString(), 0, false);
            break;
        }
        case ID_TEST_SENDSTRINGMESSAGEACK: {
            DemoCore::instance().demoIns()->sendStringMessage(getClipboardString(), 0, true);
            break;
        }
        case ID_TEST_SENDSTRINGMESSAGETIMEOUT: {
            if (IDCANCEL == DialogBox(g_inst, MAKEINTRESOURCE(IDD_DIALOG_GETVALUE), wnd, dialogGetValue)) {
                break;
            }
            try {
                DemoCore::instance().demoIns()->sendStringMessage(
                    getClipboardString(), std::stoi(g_edit_value), true);
            } catch (...) {
            }
            break;
        }
        case ID_TEST_SENDBINARYMESSAGE: {
            // open file
            WCHAR buff[MAX_PATH + 1] = { 0 };
            OPENFILENAME file_name = { 0 };
            file_name.lStructSize = sizeof(OPENFILENAME);
            file_name.hwndOwner = wnd;
            file_name.lpstrTitle = L"select a binary file";
            file_name.Flags = OFN_DONTADDTORECENT | OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
            file_name.lpstrFile = buff;
            file_name.nMaxFile = MAX_PATH;
            bool ret = GetOpenFileName(&file_name);
            if (!ret) {
                auto err = CommDlgExtendedError();
                MessageBoxA(NULL, fmt::format("打开文件失败 {}", err).c_str(), "caption", MB_OK);
                break;
            }
            // read file
            HANDLE h_file = CreateFile(file_name.lpstrFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
            DWORD read_bytes = 0;
            ZeroMemory(g_binary_data_buf, g_binary_data_size);
            if (!ReadFile(h_file, g_binary_data_buf, g_binary_data_size, &read_bytes, nullptr)) {
                veLOGI("read zip file failed, error: {}", GetLastError());
                return;
            }
            if (read_bytes <= 0) {
                break;
            }
            // send
            veLOGI("send file size: {} Bytes", read_bytes);
            DemoCore::instance().demoIns()->sendBinaryMessage(
                reinterpret_cast<const uint8_t*>(g_binary_data_buf), static_cast<int>(read_bytes));
            break;
        }
        case ID_TEST_SETIDLETIME: {
            if (IDCANCEL == DialogBox(g_inst, MAKEINTRESOURCE(IDD_DIALOG_GETVALUE), wnd, dialogGetValue)) {
                break;
            }
            try {
                DemoCore::instance().demoIns()->setIdleTime(std::stoi(g_edit_value));
            }
            catch (...) {
            }
            break;
        }
        case ID_TEST_SETAUTORECYCLETIME: {
            if (IDCANCEL == DialogBox(g_inst, MAKEINTRESOURCE(IDD_DIALOG_GETVALUE), wnd, dialogGetValue)) {
                break;
            }
            try {
                DemoCore::instance().demoIns()->setAutoRecycleTime(std::stoi(g_edit_value));
            }
            catch (...) {
            }
            break;
        }
        case ID_TEST_GETAUTORECYCLETIME: {
            DemoCore::instance().demoIns()->getAutoRecycleTime();
            break;
        }
        case ID_TEST_SETUSERPROFILEPATH: {
            DemoCore::instance().demoIns()->setUserProfilePath(getClipboardString());
            break;
        }
        case ID_TEST_GETUSERPROFILEPATH: {
            DemoCore::instance().demoIns()->getUserProfilePath();
            break;
        }
        case ID_TEST_SWITCHBACKGROUND: {
            processMenuItemInfo(wnd, ID_TEST_SWITCHBACKGROUND,
                std::function<void(bool)>(std::bind(&DemoInstance::switchBackground, DemoCore::instance().demoIns(), std::placeholders::_1)));
            break;
        }
        case ID_TEST_SETREMOTEGAMEFOREGROUND: {
            DemoCore::instance().demoIns()->setRemoteGameForeground();
            break;
        }
        case ID_TEST_SETKEYBOARDENABLE: {
            processMenuItemInfo(wnd, ID_TEST_SETKEYBOARDENABLE,
                std::function<void(bool)>(std::bind(&DemoInstance::setAudioMute, DemoCore::instance().demoIns(), std::placeholders::_1)));
            break;
        }
        case ID_TEST_GETKEYBOARDENABLE: {
            DemoCore::instance().demoIns()->getKeyboardEnable();
            break;
        }
        case ID_TEST_SETRECONNECTTIME: {
            if (IDCANCEL == DialogBox(g_inst, MAKEINTRESOURCE(IDD_DIALOG_GETVALUE), wnd, dialogGetValue)) {
                break;
            }
            try {
                DemoCore::instance().demoIns()->setReconnectTime(std::stoi(g_edit_value));
            } catch (...) {
            }
            break;
        }
        case ID_SWITCHVIDEOSTREAMPROFILE_640X360x30x3: {
            DemoCore::instance().demoIns()->switchVideoStreamProfile(1);
            break;
        }
        case ID_SWITCHVIDEOSTREAMPROFILE_848X480x30x4: {
            DemoCore::instance().demoIns()->switchVideoStreamProfile(2);
            break;
        }
        case ID_SWITCHVIDEOSTREAMPROFILE_960X540x30x4: {
            DemoCore::instance().demoIns()->switchVideoStreamProfile(3);
            break;
        }
        case ID_SWITCHVIDEOSTREAMPROFILE_1024X576x30x4: {
            DemoCore::instance().demoIns()->switchVideoStreamProfile(4);
            break;
        }
        case ID_SWITCHVIDEOSTREAMPROFILE_1024X576x60x4: {
            DemoCore::instance().demoIns()->switchVideoStreamProfile(5);
            break;
        }
        case ID_SWITCHVIDEOSTREAMPROFILE_1280X720x30x6: {
            DemoCore::instance().demoIns()->switchVideoStreamProfile(10);
            break;
        }
        case ID_SWITCHVIDEOSTREAMPROFILE_1280X720x60x6: {
            DemoCore::instance().demoIns()->switchVideoStreamProfile(11);
            break;
        }
        case ID_SWITCHVIDEOSTREAMPROFILE_1920X1080x30x12: {
            DemoCore::instance().demoIns()->switchVideoStreamProfile(24);
            break;
        }
        case ID_SWITCHVIDEOSTREAMPROFILE_1920X1080x60x12: {
            DemoCore::instance().demoIns()->switchVideoStreamProfile(25);
            break;
        }
        case ID_SWITCHVIDEOSTREAMPROFILE_2560X1440x60x16: {
            DemoCore::instance().demoIns()->switchVideoStreamProfile(35);
            break;
        }
        case ID_FILE_EXITWITHOUTSTOP: {
            DemoCore::instance().demoIns()->setNeedDestory(false);
            DestroyWindow(wnd);
            break;
        }
        default: {
            break;
        }
    }
}

void processMenuItemInfo(HWND wnd, UINT command, std::function<void (bool)> func) {
    HMENU hmenu = GetMenu(wnd);
    MENUITEMINFO menu_info = { sizeof(MENUITEMINFO) };
    menu_info.fMask = MIIM_STATE;
    GetMenuItemInfo(hmenu, command, false, &menu_info);
    if (menu_info.fState == MFS_CHECKED) {
        menu_info.fState = MFS_UNCHECKED;
        func(false);
    } else {
        menu_info.fState = MFS_CHECKED;
        func(true);
    }
    SetMenuItemInfo(hmenu, command, false, &menu_info);
}

// 弹出框的消息处理程序。
INT_PTR CALLBACK dialogGetValue(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam) {
    UNREFERENCED_PARAMETER(lparam);
    switch (message) {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;
        case WM_COMMAND:
            if (LOWORD(wparam) == IDOK || LOWORD(wparam) == IDCANCEL) {
                GetDlgItemText(dialog, IDC_EDIT_GETVALUE, g_edit_value, sizeof(g_edit_value) / sizeof(WCHAR));
                EndDialog(dialog, LOWORD(wparam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}
