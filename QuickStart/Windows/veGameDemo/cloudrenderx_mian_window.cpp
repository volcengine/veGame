#include "cloudrenderx_mian_window.h"
#include "cloudrenderx_user_events.h"
#include "configs/cloudrenderx_demo_config.h"
#include "session/cloudrenderx_game_session.h"
#include "demos/quick_demo/quick_start_window.h"

constexpr int DEMO_CONFIG_GROUP_WIDTH = 400;
constexpr int DEMO_CONFIG_GROUP_HEIGHT = 120;
constexpr int DEMOS_GROUP_WIDTH = 400;
constexpr int DEMOS_GROUP_HEIGHT = 640;
constexpr int TABLE_CELL_WIDTH = 180;
constexpr int TABLE_CELL_HEIGHT = 40;

enum class DemoEnterType
{
	BaseType = 0,
	QuickStartDemo
};

CloudRenderXMainWindow::CloudRenderXMainWindow(
	HINSTANCE instance, 
	int nCmdShow, 
	const std::wstring& window_title) : CloudRenderXWindowBase(instance, nCmdShow, window_title)
{
}

LRESULT CloudRenderXMainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto window_it = all_windows.find(hWnd);
	auto base_window_ptr = window_it == all_windows.end() ? nullptr : window_it->second;
	CloudRenderXMainWindow* main_window = nullptr;
	if (base_window_ptr) {
		main_window = (CloudRenderXMainWindow*)(base_window_ptr.get());
	}
	switch (message)
	{
	case WM_COMMAND:
		if (main_window) {
			int wmId = LOWORD(wParam);
			OutputDebugPrintf("click the menu, the menu id is %d...\n", wmId);
			if (wmId == (int)DemoEnterType::QuickStartDemo) {
				main_window->startQuickStartDemo();
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		break;
	case WM_CLOSE:
		all_windows.erase(hWnd);
		if (main_window) {
			if (main_window->close_listener) {
				main_window->close_listener();
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
	case WM_DESTROY:
		// 关闭窗口后自动退出进程
		PostQuitMessage(0);
		break;
	case WM_USER_DEMO_CONFIG_CHANGED:
		if (base_window_ptr) {
			auto config = (cloudrenderx_demo::CloudrenderxDemoConfig*)lParam;
			cloudrenderx_demo::CloudRenderxGameSession::init(config->account_id, base_window_ptr->hInstance);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

WNDPROC CloudRenderXMainWindow::getWndProcMethod()
{
	return CloudRenderXMainWindow::WndProc;
}

bool CloudRenderXMainWindow::createWindowIfNeed()
{
	auto result = CloudRenderXWindowBase::createWindowIfNeed();

	if (!result) {
		return false;
	}

	int start_left = 20;
	int start_top = 20;
	createDemoConfigContent(start_left, start_top);

	start_top += DEMO_CONFIG_GROUP_HEIGHT + 20;
	createDemosContent(start_left, start_top);

	return true;
}

void CloudRenderXMainWindow::createDemoConfigContent(int start_left, int start_top)
{
	// 创建demo config group
	if (!h_group_demo_config) {
		h_group_demo_config = CreateWindowW(
			TEXT(L"button"),
			TEXT(L"Demo配置区域"),
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			start_left, start_top, DEMO_CONFIG_GROUP_WIDTH, DEMO_CONFIG_GROUP_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessage(h_group_demo_config, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	start_left += 15;
	start_top += 30;

	int row = 1;
	int col = 1;

	// 创建第1行第1列UI控件: demo配置提示label
	int table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	int table_cell_top = start_top + (row - 1) * TABLE_CELL_HEIGHT;
	if (!h_demo_config_tip_label) {
		h_demo_config_tip_label = CreateWindowW(
			TEXT(L"static"),
			TEXT(L"请选择配置 : "),
			WS_CHILD | WS_VISIBLE | SS_CENTER,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessage(h_demo_config_tip_label, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第1行第2列UI控件: demo配置下拉框
	col = 2;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	if (!h_demo_config_select_box) {
		h_demo_config_select_box = CreateWindow(
			TEXT("COMBOBOX"), TEXT(""),
			CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			NULL,
			hInstance,
			NULL);
		auto all_config_names = cloudrenderx_demo::getAllConfigNames();
		for (auto config_name : all_config_names) {
			SendMessage(h_demo_config_select_box, CB_ADDSTRING, 0, (LPARAM)TEXT(config_name.c_str()));
		}
		SendMessage(h_demo_config_select_box, CB_SETCURSEL, 0, 0);

		SendMessage(h_demo_config_select_box, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第2行第1列UI控件: 当前游戏id提示label
	row = 2;
	col = 1;
	table_cell_top = start_top + (row - 1) * TABLE_CELL_HEIGHT;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	if (!h_current_game_id_tip_label) {
		h_current_game_id_tip_label = CreateWindowW(
			TEXT(L"static"),
			TEXT(L"当前游戏id : "),
			WS_CHILD | WS_VISIBLE | SS_CENTER,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessage(h_current_game_id_tip_label, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第2行第2列UI控件 : 当前游戏id label
	col = 2;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	if (!h_current_game_id) {
		h_current_game_id = CreateWindowW(
			TEXT(L"static"),
			TEXT(L""),
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessage(h_current_game_id, WM_SETFONT, (WPARAM)h_font, NULL);

		updateDemoConfigBySeleceted();

	}
}

void CloudRenderXMainWindow::createDemosContent(int start_left, int start_top)
{
	// 创建demo config group
	if (!h_group_demos) {
		h_group_demos = CreateWindowW(
			TEXT(L"button"),
			TEXT(L"Demo启动区域"),
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			start_left, start_top, DEMOS_GROUP_WIDTH, DEMOS_GROUP_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessage(h_group_demos, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	start_left += 15;
	start_top += 30;

	int row = 1;
	int col = 1;

	// 创建第1行第1列UI控件: 快速开始demo
	int table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	int table_cell_top = start_top + (row - 1) * TABLE_CELL_HEIGHT;
	if (!h_quick_start_demo) {
		h_quick_start_demo = CreateWindowW(
			TEXT(L"button"), TEXT(L"快速开始demo"),
			WS_CHILD | WS_VISIBLE | BS_CENTER,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)DemoEnterType::QuickStartDemo,
			hInstance,
			NULL
		);
		SendMessage(h_quick_start_demo, WM_SETFONT, (WPARAM)h_font, NULL);
	}

	// 创建第1行第2列UI控件: 排队demo
	col = 2;
	table_cell_left = start_left + (col - 1) * TABLE_CELL_WIDTH;
	if (!h_queue_demo) {
		h_queue_demo = CreateWindowW(
			TEXT(L"button"), TEXT(L"排队demo"),
			WS_CHILD | WS_VISIBLE | BS_CENTER,
			table_cell_left, table_cell_top, TABLE_CELL_WIDTH, TABLE_CELL_HEIGHT,
			hWnd,
			(HMENU)0,
			hInstance,
			NULL
		);
		SendMessage(h_queue_demo, WM_SETFONT, (WPARAM)h_font, NULL);
	}

}

void CloudRenderXMainWindow::updateDemoConfigBySeleceted()
{
	if (!h_demo_config_select_box) {
		return;
	}

	int selected_item_index = SendMessage(
		h_demo_config_select_box,
		(UINT)CB_GETCURSEL,
		(WPARAM)0, (LPARAM)0);

	char selected_config_name[256];
	(TCHAR)SendMessage(
		h_demo_config_select_box,
		(UINT)CB_GETLBTEXT,
		(WPARAM)selected_item_index,
		(LPARAM)selected_config_name);

	current_demo_config = cloudrenderx_demo::getDemoConfigByName(selected_config_name);
	if (!current_demo_config) {
		std::stringstream ss;
		ss << "cloud not find the config by name:" << selected_config_name;
		std::wstring tip(ss.str().begin(), ss.str().end());
		showMessageBox(tip);
		return;
	}

	std::wstring game_id(current_demo_config->game_id.begin(), current_demo_config->game_id.end());
	SendMessageW(h_current_game_id, WM_SETTEXT, 0, (LPARAM)game_id.c_str());

	SendMessageW(hWnd, WM_USER_DEMO_CONFIG_CHANGED, 0, (LPARAM)current_demo_config.get());
}

void CloudRenderXMainWindow::startQuickStartDemo()
{
	if (quick_start_demo) {
		showMessageBox(L"已经打开快速启动demo...");
		return;
	}

	quick_start_demo = std::make_shared<CloudrenderxQucikStartWindow>
		(hInstance,
		nCmdShow,
		L"QuickStart Demo");
	quick_start_demo->setDemoConfig(current_demo_config);
	quick_start_demo->setWindowSize(960, 540);
	quick_start_demo->createWindow();
	quick_start_demo->showWindow();
	auto weak_ptr = weak_from_this();
	quick_start_demo->setCloudRenderXWindowCloseListener([weak_ptr]()->void {
		auto shared_ptr = weak_ptr.lock();
		if (shared_ptr) {
			auto main_window_ptr = (CloudRenderXMainWindow*)(shared_ptr.get());
			main_window_ptr->quick_start_demo = nullptr;
		}
		});

}
