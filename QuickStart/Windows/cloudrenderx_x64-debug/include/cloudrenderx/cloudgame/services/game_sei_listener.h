#pragma once

#include "ve_define.h"
#include <string>

namespace vegame {
	/**
	 * @type api
	 *
	 * @brief 云游戏场景中SEI监听器
	 * 
	 * @locale zh
	 */
	class NATIVE_SDK_API CloudGameSEIListener {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudGameSEIListener() = default;

		/**
		 * @type callback
		 *
		 * @brief  接收coustom sei信息。
		 *
		 * @param [in] custom_sei_data : 自定义sei数据
		 */
		virtual void onReceiveCustomSEI(const std::string& custom_sei_data) = 0;
	};
}