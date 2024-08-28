#pragma once

#include <Windows.h>
#include <memory>
#include "configs/cloudrenderx_demo_config.h"
#include "cloudrenderx/veCloudRenderX.h"
#include "cloudrenderx/cloudgame/cloud_game_session.h"
#include "cloudrenderx/cloudgame/game_session_listener.h"

namespace cloudrenderx_demo {
	// 云游戏session
	class CloudRenderxGameSession : 
		public std::enable_shared_from_this<CloudRenderxGameSession>
	{

	public: // cloudrenderx game session生命周期相关

		// account_id来源于config.ini文件中的配置
		static void init(const std::string& account_id, HINSTANCE process_instance);

		// width与height是必要的，云端需要根据此两参数决定游戏帧流尺寸
		void startCloudGame(HWND wnd, std::shared_ptr<CloudrenderxDemoConfig> demo_config, int width, int height, std::shared_ptr<CloudGameSessionListener> session_listener);

		void stop();

		std::shared_ptr<CloudGameSession> getCurrentGameSession();

	public: // 键鼠事件相关

		// 根据原始输入，发送鼠标事件
		void captureRawInput(LPARAM lparam);

		// 发送键盘事件
		void captureWMKeyboard(WPARAM wparam, LPARAM lparam, bool down);

	public: // 窗口尺寸改变

		void updateValidArea();

		// 从云端游戏画面获取最新的最新的帧尺寸
		void updateVideoFrameSize(int width, int height);

	private:

		// ctrl、shift等按键进行左右键处理
		USHORT processExtendsCode(USHORT key, USHORT make_code, USHORT flags);
		USHORT processExtendsCode(WPARAM wparam, LPARAM lparam);

	private:
		// 云游戏相关的对象
		static std::unique_ptr<vecloudrenderx::VeCloudRenderX> veCloudRenderX;
		std::shared_ptr<CloudGameSession> currentGameSession = nullptr;

		HWND currentWnd = nullptr;

		// windows raw inputs 相关参数
		std::shared_ptr<BYTE> lpbBuffer = nullptr;
		UINT dwSize = 0;

		// 窗口尺寸相关参数
		RECT valid_area;
		int video_frame_width = 0;
		int video_frame_height = 0;

		// 当前正在被按下的鼠标事件
		uint8_t current_pressed_mouse_button = 0;
	};
}