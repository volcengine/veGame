#pragma once
#include <memory>
#include "ve_define.h"

namespace vecommon {
	class GamepadInputData;
}

namespace vegame {
	/**
	 * @type api
	 * 
	 * @brief 云游戏场景中手柄操作服务
	 * 
	 * 
	 * @locale zh
	 */
	class NATIVE_SDK_API CloudGamePadService {
	public:

		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudGamePadService() = default;

		/**
		 * @type api
		 *
		 * @brief      发送游戏手柄事件。
		 *
		 * @param [in] gamepad_input_data : 手柄事件数据。
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 参数错误导致的发送失败
		 *                     -2 : 当前角色不能发送消息
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int sendGamePadEvent(const vecommon::GamepadInputData& gamepad_input_data) = 0;
	};
}