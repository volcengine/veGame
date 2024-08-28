#pragma once
#include "cloudrenderx_window_base.h"
#include "configs/cloudrenderx_demo_config.h"

class CloudrenderxQucikStartWindow;
class CloudrenderxDemoConfig;

// demo主窗口，包含配置信息及demo列表
class CloudRenderXMainWindow : public CloudRenderXWindowBase {
public:

	CloudRenderXMainWindow(
		HINSTANCE instance,
		int nCmdShow,
		const std::wstring& window_title);

	~CloudRenderXMainWindow() = default;

	// windows系统API回调
	static LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:

	WNDPROC getWndProcMethod() override;

	bool createWindowIfNeed() override;

private:

	void createDemoConfigContent(int start_left, int start_top);

	void createDemosContent(int start_left, int start_top);

	void updateDemoConfigBySeleceted();

	void startQuickStartDemo();

private:

	std::shared_ptr<cloudrenderx_demo::CloudrenderxDemoConfig> current_demo_config = nullptr;

	// 快速开始demo窗口
	std::shared_ptr<CloudrenderxQucikStartWindow> quick_start_demo = nullptr;

	// demo config group
	HWND h_group_demo_config = nullptr;         // 游戏demo配置group
	HWND h_demo_config_tip_label = nullptr;     // demo配置提示label
	HWND h_demo_config_select_box = nullptr;    // demo配置下拉框
	HWND h_current_game_id_tip_label = nullptr; // 当前游戏id提示label
	HWND h_current_game_id = nullptr;           // 当前游戏id label

	// demo group
	HWND h_group_demos = nullptr;       // 当前所有demo的group
	HWND h_quick_start_demo = nullptr;  // 快速开始demo
	HWND h_queue_demo = nullptr;        // 排队demo
};