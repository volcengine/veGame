#include "cloudrenderx_window_base.h"
#include <sstream>
#include <Uxtheme.h>

uint16_t CloudRenderXWindowBase::window_num = 0;

// todo 关闭窗口是从此map中移除
std::map<HWND, std::shared_ptr<CloudRenderXWindowBase>> CloudRenderXWindowBase::all_windows;

void OutputDebugPrintf(const char* strOutputString, ...)
{
	char strBuffer[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	_vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
	va_end(vlArgs);
	OutputDebugString(strBuffer);
}

CloudRenderXWindowBase::CloudRenderXWindowBase(
	HINSTANCE instance,
	int nCmdShow,
	const std::wstring& window_title) :
	hInstance(instance), 
	nCmdShow(nCmdShow), 
	windowTitle(window_title)
{
	window_num++;
	WINDOW_CLASS_NAME(window_num);
	windowClass = wss.str();
}

CloudRenderXWindowBase::~CloudRenderXWindowBase()
{

}

bool CloudRenderXWindowBase::setWindowSize(const int& width, const int& height)
{
	if (hWnd) {
		// todo 动态修改窗口大小
		return false;
	}

	windowWidth = width;
	windowHeight = height;

	return true;
}

void CloudRenderXWindowBase::createWindow()
{
	WNDPROC _WndProc = getWndProcMethod();

	if (!registerWindowClass(_WndProc)) {
		return;
	}

	if (!createWindowIfNeed()) {
		return;
	}
}

void CloudRenderXWindowBase::showWindow()
{
	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);
}

void CloudRenderXWindowBase::destroyWindow()
{
	if (!hWnd) {
		return;
	}

	SendMessage(hWnd, WM_CLOSE, 0, 0);
}

void CloudRenderXWindowBase::showMessageBox(const std::wstring& message)
{
	MessageBoxW(NULL, message.c_str(), L"提示", MB_OK);
}

HWND CloudRenderXWindowBase::getHWnd()
{
	return hWnd;
}

int CloudRenderXWindowBase::getWindowWidth()
{
	return windowWidth;
}

int CloudRenderXWindowBase::getWindowHeight()
{
	return windowHeight;
}

void CloudRenderXWindowBase::setCloudRenderXWindowCloseListener(CloudRenderXWindowCloseListener listener)
{
	close_listener = listener;
}

bool CloudRenderXWindowBase::registerWindowClass(WNDPROC _WndProc)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = _WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);  //窗口背景画刷（灰色） //(HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = windowClass.c_str();
	auto result = RegisterClassExW(&wcex);
	if (result) {
		OutputDebugPrintf("success to register class, the hWnd is %p...\n", hWnd);
	}
	else {
		auto last_error = GetLastError();
		OutputDebugPrintf("failed to Register class[%s], the error code is %d...\n", windowClass.c_str(), last_error);
	}
	return result;
}

bool CloudRenderXWindowBase::createWindowIfNeed()
{
	if (hWnd) {
		return true;
	}

	// 创建窗口默认字体
	if (!h_font) {
		h_font = CreateFont(
			-14, -7, 0, 0, 100,
			FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
			FF_DONTCARE, TEXT("微软雅黑")
		);
	}

	auto menu_handle = createMenu();

	auto title_height = GetThemeSysSize(NULL, SM_CYSIZE) + GetThemeSysSize(NULL, SM_CYMENUSIZE) + GetThemeSysSize(NULL, SM_CXPADDEDBORDER) * 2;

	// 此方法内部，在return之前会回调WndProc方法
	hWnd = CreateWindowW(windowClass.c_str(),
		windowTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		computeWindowStartX(),
		computeWindowStartY(),
		windowWidth,
		windowHeight + title_height,
		nullptr,
		menu_handle,
		hInstance,
		nullptr);
	if (hWnd) {
		all_windows[hWnd] = shared_from_this();
		return true;
	}
	else {
		auto last_error = GetLastError();
		OutputDebugPrintf("failed to create window, the error code is %d...\n", last_error);
		return false;
	}
}

int CloudRenderXWindowBase::computeWindowStartX()
{
	return 100 + (window_num - 1) * DEFAULT_WINDOW_WIDTH / 20;
}

int CloudRenderXWindowBase::computeWindowStartY()
{
	return 100 + (window_num - 1) * DEFAULT_WINDOW_HEIGHT / 20;
}
