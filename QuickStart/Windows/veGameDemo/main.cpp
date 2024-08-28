#include <Windows.h>
#include <hidusage.h>
#include "cloudrenderx_mian_window.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // 创建并打开主窗口
    auto main_window = std::make_shared<CloudRenderXMainWindow>(
        hInstance,
        nCmdShow,
        L"main_window"
    );

    main_window->setWindowSize(450, 800);
    main_window->createWindow();
    main_window->showWindow();

    // 【重要】将Demo注册为raw input，确保可以接收原始鼠标事件
    RAWINPUTDEVICE raw_input_devices[1];
    raw_input_devices[0].usUsagePage = HID_USAGE_PAGE_GENERIC;   // 设备类
    raw_input_devices[0].usUsage = 0;
    raw_input_devices[0].dwFlags = RIDEV_PAGEONLY;
    raw_input_devices[0].hwndTarget = NULL; // 窗口句柄为空，则将raw input event路由到焦点窗口
    if (RegisterRawInputDevices(raw_input_devices, 1, sizeof(RAWINPUTDEVICE))) {
        OutputDebugPrintf("RegisterRawInputDevices successed...\n");
    }
    else {
        auto last_error = GetLastError();
        OutputDebugPrintf("RegisterRawInputDevices failed, the error code is %d...\n", last_error);
        abort();
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}