#pragma once
#include "cloudrenderx_window_base.h"
#include "cloudrenderx_stream_profile_window.h"
#include "session/cloudrenderx_game_session.h"
#include "cloudrenderx/cloudgame/services/game_stream_profile_listener.h"
#include "cloudrenderx/cloudgame/services/game_stream_profile_service.h"

#define START_MENU_BASE_ID 100
#define START_MENU_ID START_MENU_BASE_ID##+CloudRenderXWindowBase::window_num
#define IS_START_MENU(x) \
		199>=##x&&START_MENU_BASE_ID<=##x

#define STOP_MENU_BASE_ID 200
#define STOP_MENU_ID STOP_MENU_BASE_ID##+CloudRenderXWindowBase::window_num
#define IS_STOP_MENU(x) \
		299>=##x&&STOP_MENU_BASE_ID<=##x

#define STREAM_PROFILE_MENU_BASE_ID 1100
#define STREAM_PROFILE_MENU_ID STREAM_PROFILE_MENU_BASE_ID##+CloudRenderXWindowBase::window_num
#define IS_STREAM_PROFILE_MENU(x) \
		1199>=##x&&STREAM_PROFILE_MENU_BASE_ID<=##x

// demo主窗口，包含配置信息及demo列表
class CloudrenderxQucikStartWindow : 
	public CloudRenderXWindowBase,
	public CloudGameSessionListener {
public:

	CloudrenderxQucikStartWindow(
		HINSTANCE instance,
		int nCmdShow,
		const std::wstring& window_title);

	~CloudrenderxQucikStartWindow() = default;

	// windows系统API回调
	static LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void setDemoConfig(std::shared_ptr<cloudrenderx_demo::CloudrenderxDemoConfig> config);

public: // CloudGameSessionListener中需要override的方法

	void onStartSuccess(int profile_id, const char* round_id, const char* game_id, const char* reserved_id, const char* user_extra) override;

	void onStop() override;

	void onFirstRemoteVideoFrame(const vecommon::VideoFrameInfo& info) override;

	void onVideoSizeChanged(const vecommon::VideoFrameInfo& info) override;

	void onError(int code, const std::string msg) override;

protected:

	WNDPROC getWndProcMethod() override;

	HMENU createMenu() override;

private:

	void clickMenu(int wmId);

	void initStreamProfileService();

	void deinitStreamProfileService();

private:

	// stream profile相关参数
	// stream profile监听器
	class StreamProfileWrapper : public vegame::CloudgameStreamProfileListener {
	public:

		void onStreamProfileIdChanged(int old_profile_id, int new_profile_id) override;

		void onError(int error_code, std::string error_msg) override;

		std::shared_ptr<vegame::CloudgameStreamProfileService> stream_profile_service = nullptr;

		std::shared_ptr<CloudRenderXStreamProfileWindow> stream_profile_window = nullptr;
	};
	std::shared_ptr<StreamProfileWrapper> stream_profile_wrapper = nullptr;


	// 主菜单
	HMENU hMenu = nullptr;

	std::shared_ptr<cloudrenderx_demo::CloudrenderxDemoConfig> current_demo_config = nullptr;

	std::shared_ptr<cloudrenderx_demo::CloudRenderxGameSession> cloudrenderx_game_session = nullptr;

	bool is_start_success = false;
	bool should_clear_screen_ = false;
};