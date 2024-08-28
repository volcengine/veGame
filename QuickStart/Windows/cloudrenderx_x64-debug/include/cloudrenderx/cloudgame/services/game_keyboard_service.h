#pragma once
#include "ve_define.h"
#include <stdint.h>
namespace vecommon {
	class KeyboardData;
}
namespace vegame {
	/**
	 * @type api
	 * 
	 * @brief 云游戏场景中键盘操作服务
	 * 
	 * @locale zh
	 */
	class NATIVE_SDK_API CloudGameKeyboardService {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudGameKeyboardService() = default;

		/**
		 * @type api
		 *
		 * @brief      发送键盘按键事件。
		 *
		 * @param [in] keyboard_data : 键盘按键。
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 参数错误导致的发送失败
		 *                     -2 : 当前角色不能发送消息
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int sendKeyboardvent(const vecommon::KeyboardData& keyboard_data) = 0;

		/**
		 * @type api
		 *
		 * @brief      设置相同Keyboard事件的发送频率。
		 *
		 * @param [in] duration   : 每隔多少毫秒发送1次相同的Keyboard事件。
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 参数错误导致的发送失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int setSendSameKeyboardEventDuration(int64_t duration) = 0;
	};
}