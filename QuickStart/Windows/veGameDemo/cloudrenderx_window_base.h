#pragma once
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <string>
#include <map>
#include <memory>
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <functional>

#define DEFAULT_WINDOW_WIDTH 540
#define DEFAULT_WINDOW_HEIGHT 960

#define WINDOW_CLASS_NAME(X)                  \
    std::wstringstream wss;                   \
    wss << "CLOUD_X_RENDER_SDK_QUICKSTART_";  \
    wss << ##X

void OutputDebugPrintf(const char* strOutputString, ...);

enum CloudXRenderType {
	CLOUD_GAME,
	UNKNOWN
};

using CloudRenderXWindowCloseListener = std::function<void()>;

// 窗口基类
class CloudRenderXWindowBase : public std::enable_shared_from_this<CloudRenderXWindowBase> {
public:
	CloudRenderXWindowBase(
		HINSTANCE instance,
		int nCmdShow,
		const std::wstring& window_title);

	~CloudRenderXWindowBase();

	// 必须在createWindow调用前，执行此方法
	// 否则，将无效
	bool setWindowSize(const int& width, const int& height);

	void createWindow();

	void showWindow();

	void destroyWindow();

	static void showMessageBox(const std::wstring& message);

public: // 属性setting or getting

	HWND getHWnd();

	int getWindowWidth();

	int getWindowHeight();

	void setCloudRenderXWindowCloseListener(CloudRenderXWindowCloseListener listener);

protected:

	virtual bool createWindowIfNeed();

	virtual HMENU createMenu() {
		return nullptr;
	}

	// 提供窗口事件响应方法
	virtual WNDPROC getWndProcMethod(){
		return nullptr;
	};

private: // 创建窗口相关的方法

	bool registerWindowClass(WNDPROC _WndProc);

	static int computeWindowStartX();

	static int computeWindowStartY();

public:

	// 可执行文件的实例句柄或者模块句柄
	HINSTANCE hInstance = nullptr;

protected:

	// 创建了多少个窗口了
	static uint16_t window_num;

	static std::map<HWND, std::shared_ptr<CloudRenderXWindowBase>> all_windows;

	// 当前窗口句柄
	HWND hWnd = nullptr;

	int windowWidth = DEFAULT_WINDOW_WIDTH;

	int windowHeight = DEFAULT_WINDOW_HEIGHT;

	int nCmdShow;

	std::wstring windowTitle;

	CloudRenderXWindowCloseListener close_listener = nullptr;

	bool is_client_foreground = true;

	// UI相关字体
	HFONT h_font = nullptr;  //逻辑字体

private:

	std::wstring windowClass;
};