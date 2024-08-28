#include "quick_start_window.h"
#include "cloudrenderx_user_events.h"
#include "ve_log_lite.h"

HCURSOR global_cursor = (HCURSOR)INVALID_HANDLE_VALUE;

CloudrenderxQucikStartWindow::CloudrenderxQucikStartWindow(
	HINSTANCE instance, 
	int nCmdShow, 
	const std::wstring& window_title) : CloudRenderXWindowBase(instance, nCmdShow, window_title)
{
}

LRESULT CloudrenderxQucikStartWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto window_it = all_windows.find(hWnd);
	auto base_window_ptr = window_it == all_windows.end() ? nullptr : window_it->second;
	auto window_ptr = (CloudrenderxQucikStartWindow*)(base_window_ptr.get());

	switch (message)
	{
	case WM_COMMAND: {
		// 点击菜单
		if (window_ptr) {
			int wmId = LOWORD(wParam);
			OutputDebugPrintf("click the menu, the hWnd is %p, the menu id is %d...\n", hWnd, wmId);
			window_ptr->clickMenu(wmId);
		}
		break;
	}
	case WM_MOVE:
	case WM_SIZE: {
		if (window_ptr
			&& window_ptr->cloudrenderx_game_session) {
			window_ptr->cloudrenderx_game_session->updateValidArea();
		}
		break;
	}
	case WM_INPUT: {
		// 原始输入，包含鼠标move、鼠标左右键点击、鼠标wheel事件等
		if (window_ptr
			&& window_ptr->cloudrenderx_game_session) {
			window_ptr->cloudrenderx_game_session->captureRawInput(lParam);
		}
		break;
	}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		// 键盘输入
		if (wParam == VK_PACKET) {
			break;
		}
		if (window_ptr
			&& window_ptr->cloudrenderx_game_session) {
			window_ptr->cloudrenderx_game_session->captureWMKeyboard(wParam, lParam, true);
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		// 键盘输入
		if (wParam == VK_PROCESSKEY || wParam == VK_PACKET) {
			break;
		}
		if (window_ptr
			&& window_ptr->cloudrenderx_game_session) {
			window_ptr->cloudrenderx_game_session->captureWMKeyboard(wParam, lParam, false);
		}
		break;
	case WM_CLOSE:
		all_windows.erase(hWnd);
		if (window_ptr) {
			if (window_ptr->close_listener) {
				window_ptr->close_listener();
			}
		}
		DestroyWindow(hWnd);
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_ACTIVATE: {
		if (window_ptr) {
			bool active = LOWORD(wParam);
			window_ptr->is_client_foreground = active;
		}
		break;
	}
	case WM_USER_CURSOR_SHOW: {
		if ((bool)wParam) {
			while (ShowCursor(true) < 0);
		}
		else {
			while (ShowCursor(false) >= 0);
		}
		break;
	}
	case WM_USER_SHOW_MESSAGE_BOX: {
		CloudRenderXWindowBase::showMessageBox((const WCHAR*)lParam);
		break;
	}

	case WM_USER_CURSOR_ICON_CHANGE: {
		global_cursor = (HCURSOR)wParam;
		SetCursor(global_cursor);
		break;
	}
	case WM_USER_UPDATE_STREAM_PROFILE_ID:{
		if (window_ptr
			&& window_ptr->is_start_success
			&& window_ptr->stream_profile_wrapper
			&& window_ptr->stream_profile_wrapper->stream_profile_service) {
			window_ptr->stream_profile_wrapper->stream_profile_service->setStreamProfileIdAsync((int)lParam);
		}
		break;
	}
	case WM_SETCURSOR: {
		if (global_cursor != INVALID_HANDLE_VALUE && LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(global_cursor);
			return TRUE;
		}
		else {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void CloudrenderxQucikStartWindow::setDemoConfig(std::shared_ptr<cloudrenderx_demo::CloudrenderxDemoConfig> config)
{
	current_demo_config = config;
}

void CloudrenderxQucikStartWindow::onStartSuccess(int profile_id, const char* round_id, const char* game_id, const char* reserved_id, const char* user_extra)
{
	veLOGI("CloudrenderxQucikStartWindow::onStartSuccess : %d, msg : %s \n", profile_id, game_id);

	is_start_success = true;

	should_clear_screen_ = true;

	initStreamProfileService();


	//// 设置相同Keyboard事件的发送频率
	//auto keyboard_service = currentGameSession->getKeyboardService();
	//const auto& send_same_keyboard_duration = 16;
	//if (keyboard_service
	//    && send_same_keyboard_duration > 0) {
	//    auto result = keyboard_service->setSendSameKeyboardEventDuration(send_same_keyboard_duration);
	//    veLOGD("set the send_same_keyboard_event duration, the value is {}, the result is {}", send_same_keyboard_duration, result);
	//}
}

void CloudrenderxQucikStartWindow::onStop()
{
	veLOGI("start invoking the CloudrenderxQucikStartWindow::onStop method...");

	if (should_clear_screen_) {
		if (hWnd) {
			InvalidateRect(hWnd, nullptr, true);
		}
		should_clear_screen_ = false;
	}
}

void CloudrenderxQucikStartWindow::onFirstRemoteVideoFrame(const vecommon::VideoFrameInfo& info)
{
	veLOGI("start invoking the CloudrenderxQucikStartWindow::onFirstRemoteVideoFrame method...");
	if (cloudrenderx_game_session) {
		cloudrenderx_game_session->updateVideoFrameSize(info.width, info.height);
	}
}

void CloudrenderxQucikStartWindow::onVideoSizeChanged(const vecommon::VideoFrameInfo& info)
{
	veLOGI("start invoking the CloudrenderxQucikStartWindow::onVideoSizeChanged method...");
	if (cloudrenderx_game_session) {
		cloudrenderx_game_session->updateVideoFrameSize(info.width, info.height);
	}
}

void CloudrenderxQucikStartWindow::onError(int code, const std::string msg)
{
	veLOGE("start invoking the CloudrenderxQucikStartWindow::onError method, code : {}, error_msg: {}", code, msg);

	is_start_success = false;
	if (should_clear_screen_) {
		if (hWnd) {
			InvalidateRect(hWnd, nullptr, true);
		}
		should_clear_screen_ = false;
	}

	deinitStreamProfileService();

	std::stringstream ss;
	ss << "error: [" << code << "]" << msg;
	auto tip_str = ss.str();
	std::wstring tip(tip_str.begin(), tip_str.end());
	SendMessageW(hWnd, WM_USER_SHOW_MESSAGE_BOX, 0, (LPARAM)tip.c_str());
}


WNDPROC CloudrenderxQucikStartWindow::getWndProcMethod()
{
	return CloudrenderxQucikStartWindow::WndProc;
}

HMENU CloudrenderxQucikStartWindow::createMenu()
{
	if (hMenu) {
		return hMenu;
	}

	hMenu = CreateMenu();

	// 创建实例菜单
	auto hInstanceMenu = CreateMenu();
	// 添加初始化菜单
	::AppendMenuW(hInstanceMenu, MF_STRING, START_MENU_ID, L"&启动 ");
	::AppendMenuW(hInstanceMenu, MF_STRING, STOP_MENU_ID, L"&停止 ");

	// 将实例菜单添加到顶部菜单
	::AppendMenuW(hMenu, MF_POPUP, (UINT)hInstanceMenu, L"&云游戏Session ");

	// 将stream profile菜单添加到顶部菜单
	::AppendMenuW(hMenu, MF_POPUP, STREAM_PROFILE_MENU_ID, L"&Stream Profile");

	return hMenu;
}

void CloudrenderxQucikStartWindow::clickMenu(int wmId)
{
	if (IS_START_MENU(wmId)) {
		if (!cloudrenderx_game_session) {
			cloudrenderx_game_session = std::make_shared<cloudrenderx_demo::CloudRenderxGameSession>();
		}
		// 启动
		auto listener = std::static_pointer_cast<CloudrenderxQucikStartWindow>(shared_from_this());
		cloudrenderx_game_session->startCloudGame(hWnd, current_demo_config, windowWidth, windowHeight, listener);
	}
	else if (IS_STOP_MENU(wmId)) {
		cloudrenderx_game_session->stop();
	}
	else if(IS_STREAM_PROFILE_MENU(wmId)){
		if (!is_start_success) {
			showMessageBox(L"请先成功运行一款云游戏!");
			return;
		}

		if (!stream_profile_wrapper) {
			stream_profile_wrapper = std::make_shared<StreamProfileWrapper>();
		}

		if (stream_profile_wrapper->stream_profile_window) {
			showMessageBox(L"已经创建了stream profile测试窗口!");
			return;
		}

		stream_profile_wrapper->stream_profile_window = std::make_shared<CloudRenderXStreamProfileWindow>(
			hInstance,
			nCmdShow,
			L"stream profile",
			hWnd
		);

		stream_profile_wrapper->stream_profile_window->setWindowSize(540, 250);
		stream_profile_wrapper->stream_profile_window->createWindow();
		stream_profile_wrapper->stream_profile_window->showWindow();
		auto weak_ptr = weak_from_this();
		stream_profile_wrapper->stream_profile_window->setCloudRenderXWindowCloseListener([weak_ptr]()->void {
			auto shared_ptr = weak_ptr.lock();
			if (shared_ptr) {
				auto main_window_ptr = (CloudrenderxQucikStartWindow*)(shared_ptr.get());
				main_window_ptr->stream_profile_wrapper->stream_profile_window = nullptr;
			}
		});
	}
}

void CloudrenderxQucikStartWindow::initStreamProfileService()
{
	auto current_session = cloudrenderx_game_session->getCurrentGameSession();

	// 获取stream profile sevice
	if (!stream_profile_wrapper) {
		stream_profile_wrapper = std::make_shared<StreamProfileWrapper>();
	}

	if (!stream_profile_wrapper->stream_profile_service) {
		stream_profile_wrapper->stream_profile_service = current_session->getStreamProfileService();
		if (!(stream_profile_wrapper->stream_profile_service)) {
			CloudRenderXWindowBase::showMessageBox(L"failed to get the stream profile service...");
			veLOGE("[CloudrenderxQucikStartWindow]failed to get the stream profile service...");
			abort();
		}
	}

	stream_profile_wrapper->stream_profile_service->setStreamProfileListener(stream_profile_wrapper);
}

void CloudrenderxQucikStartWindow::deinitStreamProfileService()
{
	if (stream_profile_wrapper
		&& stream_profile_wrapper->stream_profile_service) {
		// 保险起见将监听器置空,防止内存泄漏
		stream_profile_wrapper->stream_profile_service->setStreamProfileListener(nullptr);
		stream_profile_wrapper->stream_profile_service = nullptr;
	}

	if (stream_profile_wrapper
		&& stream_profile_wrapper->stream_profile_window) {
		stream_profile_wrapper->stream_profile_window->destroyWindow();
		stream_profile_wrapper->stream_profile_window = nullptr;
	}

	stream_profile_wrapper = nullptr;
}

void CloudrenderxQucikStartWindow::StreamProfileWrapper::onStreamProfileIdChanged(int old_profile_id, int new_profile_id)
{
	if (!stream_profile_window
		|| !stream_profile_service) {
		return;
	}

	std::pair<size_t, size_t> frame_size;
	auto result = stream_profile_service->getResolution(&frame_size);
	if (result != 0) {
		CloudRenderXWindowBase::showMessageBox(L"failed to get resolution from the stream profile...");
		return;
	}

	int fps = 0;
	result = stream_profile_service->getFrameRate(&fps);
	if (result != 0) {
		CloudRenderXWindowBase::showMessageBox(L"failed to get fps from the stream profile...");
		return;
	}

	int bitrate;
	result = stream_profile_service->getBitrate(&bitrate);
	if (result != 0) {
		CloudRenderXWindowBase::showMessageBox(L"failed to get bitrate from the stream profile...");
		return;
	}

	stream_profile_window->updateProfile(new_profile_id, frame_size, fps, bitrate);
}

void CloudrenderxQucikStartWindow::StreamProfileWrapper::onError(int error_code, std::string error_msg)
{
	std::wstringstream ss;
	ss << L"receive stream profile error: [" << error_code << L"] " << std::wstring(error_msg.begin(), error_msg.end());
	CloudRenderXWindowBase::showMessageBox(ss.str().c_str());
}
