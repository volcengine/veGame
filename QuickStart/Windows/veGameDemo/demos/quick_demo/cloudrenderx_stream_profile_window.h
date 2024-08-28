#pragma once
#include "cloudrenderx_window_base.h"
#include <memory>

// stream profile相关信息展示窗口，不会调用任何云游戏的API
class CloudRenderXStreamProfileWindow :
	public CloudRenderXWindowBase {
public:

	CloudRenderXStreamProfileWindow(
		HINSTANCE instance,
		int nCmdShow,
		const std::wstring& window_title,
		HWND _attached_window);

	~CloudRenderXStreamProfileWindow() = default;

	// windows系统API回调
	static LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);

	void updateProfile(int new_profile_id, std::pair<size_t, size_t> frame_size, int fps, int bitrate);

protected:

	WNDPROC getWndProcMethod() override;

	bool createWindowIfNeed() override;

private:

	void createStreamProfileWindowContent();

private:

	HWND attached_window = nullptr;

	// UI相关的属性
	HFONT h_font = nullptr;  //逻辑字体

	HWND h_group_stream_profile = nullptr;         // stream profile group
	HWND h_profile_id_tip_label = nullptr;         // profile id 提示label
	HWND h_profile_id_label = nullptr;             // profile id 展示label
	HWND h_frame_size_tip_label = nullptr;         // frame尺寸提示label
	HWND h_frame_size_label = nullptr;             // frame尺寸展示label
	HWND h_fps_tip_label = nullptr;                // fps提示label
	HWND h_fps_label = nullptr;                    // fps展示label
	HWND h_bitrate_tip_label = nullptr;            // bitrate提示label
	HWND h_bitrate_label = nullptr;                // bitrate展示label
	HWND h_update_profile_id_tip_label = nullptr;  // 更新profile id提示label
	HWND h_update_profile_id_input = nullptr;      // 更新profile id输入框
	HWND h_update_profile_id_button = nullptr;     // 更新profile id按钮
};