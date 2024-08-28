#include "cloudrenderx_stream_profile_window.h"
#include <sstream>
#include "string_utils.h"
#include "cloudrenderx_user_events.h"

constexpr int STREAM_PROFILE_GROUP_WIDTH = 480;
constexpr int STREAM_PROFILE_GROUP_HEIGHT = 200;

constexpr int TABLE_CELL_WIDTH = 150;
constexpr int TABLE_CELL_HEIGHT = 30;

enum StreamProfileMenuHandle
{
	BaseMenu = 0,
	UpdateProfileId
};

CloudRenderXStreamProfileWindow::CloudRenderXStreamProfileWindow(HINSTANCE instance, int nCmdShow, const std::wstring& window_title, HWND _attached_window)
    : CloudRenderXWindowBase(instance, nCmdShow, window_title), attached_window(_attached_window)
{
}

LRESULT CloudRenderXStreamProfileWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto window_it = all_windows.find(hWnd);
	auto base_window_ptr = window_it == all_windows.end() ? nullptr : window_it->second;
	CloudRenderXStreamProfileWindow* stream_profile_window = nullptr;
	if (base_window_ptr) {
		stream_profile_window = (CloudRenderXStreamProfileWindow*)(base_window_ptr.get());
	}
	switch (message)
	{
	case WM_COMMAND:
		if (stream_profile_window) {
			int wmId = LOWORD(wParam);
			OutputDebugPrintf("click the menu, the menu id is %d...\n", wmId);

			if (wmId == UpdateProfileId) {
				TCHAR buff[1024];
				GetWindowText(stream_profile_window->h_update_profile_id_input, buff, 1024);

				std::string profile_id_str = buff;
				if (!cloudrenderx_demo::isDigits(profile_id_str, false)) {
					std::wstringstream ss;
					ss << L"the profile id value[" << std::wstring(profile_id_str.begin(), profile_id_str.end()) << "] is not digits...";
					showMessageBox(ss.str());
					return 0;
				}
				// 发送之
				auto profile_id = stoi(profile_id_str);
				PostMessageW(stream_profile_window->attached_window, WM_USER_UPDATE_STREAM_PROFILE_ID, 0 ,(LPARAM)profile_id);
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		break;
	case WM_CLOSE:
		all_windows.erase(hWnd);
		if (stream_profile_window) {
			if (stream_profile_window->close_listener) {
				stream_profile_window->close_listener();
			}
		}
		DestroyWindow(hWnd);
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_INPUT:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void CloudRenderXStreamProfileWindow::updateProfile(int new_profile_id, std::pair<size_t, size_t> frame_size, int fps, int bitrate)
{
	
	std::wstringstream ss;
	ss << L" " << frame_size.first << L" x " << frame_size.second << L"(宽x高)";
	SendMessageW(h_frame_size_label, WM_SETTEXT, 0, (LPARAM)ss.str().c_str());

	ss.str(L"");
	ss << " " << fps << "  ";
	SendMessageW(h_fps_label, WM_SETTEXT, 0, (LPARAM)ss.str().c_str());
	
	ss.str(L"");
	ss << " " << bitrate << "bps  ";
	SendMessageW(h_bitrate_label, WM_SETTEXT, 0, (LPARAM)ss.str().c_str());

	ss.str(L"");
	ss << " " << new_profile_id << "  ";
	SendMessageW(h_profile_id_label, WM_SETTEXT, 0, (LPARAM)ss.str().c_str());
}

WNDPROC CloudRenderXStreamProfileWindow::getWndProcMethod()
{
	return CloudRenderXStreamProfileWindow::WndProc;
}

bool CloudRenderXStreamProfileWindow::createWindowIfNeed()
{
	auto result = CloudRenderXWindowBase::createWindowIfNeed();

	if (!result) {
		return false;
	}

	createStreamProfileWindowContent();

	return true;
}

void CloudRenderXStreamProfileWindow::createStreamProfileWindowContent()
{
	// 创建窗口默认字体
	if (!h_font) {
		h_font = CreateFontW(
			-14, -7, 0, 0, 100,
			FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
			FF_DONTCARE, TEXT(L"微软雅黑")
		);
	}

	int start_left = 20;
	int start_top = 20;

	// 创建time operation group
	if (!h_group_stream_profile) {
		h_group_stream_profile = CreateWindowW(
			TEXT(L"button"),
			TEXT(L"stream profile操作区域"),
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			start_left, start_top, STREAM_PROFILE_GROUP_WIDTH, STREAM_PROFILE_GROUP_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessage(h_group_stream_profile, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	start_left += 15;
	start_top += 30;

	int row = 1;
	int col = 1;

	// 创建第1行第1列UI控件, 创建profile id 提示label
	int table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	int table_cell_top = start_top + (row - 1) * TABLE_CELL_HEIGHT;
	if (!h_profile_id_tip_label) {
		h_profile_id_tip_label = CreateWindowW(
			TEXT(L"static"),
			TEXT(L"清晰度挡位 : "),
			WS_CHILD | WS_VISIBLE | SS_RIGHT,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessage(h_profile_id_tip_label, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第1行第2列UI控件, 创建profile id 展示label
	col = 2;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	if (!h_profile_id_label) {
		h_profile_id_label = CreateWindowW(
			TEXT(L"static"),
			TEXT(L" 0 "),
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessage(h_profile_id_label, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第2行第1列UI控件, 创建frame尺寸提示label
	row = 2;
	col = 1;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	table_cell_top = start_top + (row - 1) * TABLE_CELL_HEIGHT;
	if (!h_frame_size_tip_label) {
		h_frame_size_tip_label = CreateWindowW(
			TEXT(L"static"),
			TEXT(L"  帧尺寸 : "),
			WS_CHILD | WS_VISIBLE | SS_RIGHT,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessage(h_frame_size_tip_label, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第2行第2列UI控件, 创建fps提示label
	col = 2;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	if (!h_frame_size_label) {
		h_frame_size_label = CreateWindowW(
			TEXT(L"static"),
			TEXT(L" 0 x 0( 宽  x  高 )"),
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessage(h_frame_size_label, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第3行第1列UI控件, 创建frame尺寸提示label
	row = 3;
	col = 1;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	table_cell_top = start_top + (row - 1) * TABLE_CELL_HEIGHT;
	if (!h_fps_tip_label) {
		h_fps_tip_label = CreateWindowW(
			TEXT(L"static"),
			TEXT(L"  FPS   :  "),
			WS_CHILD | WS_VISIBLE | SS_RIGHT,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessage(h_fps_tip_label, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第3行第2列UI控件, 创建frame尺寸展示label
	col = 2;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	if (!h_fps_label) {
		h_fps_label = CreateWindowW(
			TEXT(L"static"),
			TEXT(L" 0 "),
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessageW(h_fps_label, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第4行第1列UI控件, 创建bitrate提示label
	row = 4;
	col = 1;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	table_cell_top = start_top + (row - 1) * TABLE_CELL_HEIGHT;
	if (!h_bitrate_tip_label) {
		h_bitrate_tip_label = CreateWindowW(
			TEXT(L"static"),
			TEXT(L"  码率 : "),
			WS_CHILD | WS_VISIBLE | SS_RIGHT,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessageW(h_bitrate_tip_label, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第4行第2列UI控件, 创建frame尺寸展示label
	col = 2;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	if (!h_bitrate_label) {
		h_bitrate_label = CreateWindowW(
			TEXT(L"static"),
			TEXT(L" 0bps"),
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessageW(h_bitrate_label, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第5行第1列UI控件, 创建更新profile id提示label
	row = 5;
	col = 1;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	table_cell_top = start_top + (row - 1) * TABLE_CELL_HEIGHT;
	if (!h_update_profile_id_tip_label) {
		h_update_profile_id_tip_label = CreateWindowW(
			TEXT(L"static"),
			TEXT(L"更新清晰度挡位   :  "),
			WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessageW(h_update_profile_id_tip_label, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第5行第2列UI控件, 创建更新profile id输入框
	col = 2;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	if (!h_update_profile_id_input) {
		h_update_profile_id_input = CreateWindowW(
			TEXT(L"edit"),
			NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH * 0.5, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			NULL,
			NULL
		);
		SendMessageW(h_update_profile_id_input, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第5行第3列UI控件, 创建更新profile id按钮
	col = 3;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH - TABLE_CELL_WIDTH * 0.45;
	if (!h_update_profile_id_button) {
		h_update_profile_id_button = CreateWindowW(
			TEXT(L"button"), TEXT(L"发送"),
			WS_CHILD | WS_VISIBLE | BS_CENTER | BS_DEFPUSHBUTTON,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH * 0.5, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)UpdateProfileId,
			hInstance,
			NULL
		);
		SendMessageW(h_update_profile_id_button, WM_SETFONT, (WPARAM)h_font, NULL);
	}

}
