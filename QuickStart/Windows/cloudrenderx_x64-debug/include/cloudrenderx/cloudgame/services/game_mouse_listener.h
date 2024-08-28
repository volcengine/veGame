#pragma once

#include "ve_define.h"

namespace vegame {
	/**
	 * @type api
	 * 
	 * @brief 云游戏场景鼠标监听器
	 * 
	 * @locale zh
	 */
	class NATIVE_SDK_API CloudGameMouseListener {
	public:

		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudGameMouseListener() = default;

		/**
		 * @type callback
		 *
		 * @brief  云端鼠标可见状态回调。
		 *
		 * @param [in] visible  : 1: 可见， 0: 不可见。
		 */
		virtual void onRemoteMouseVisibilityChanged(const int visible) {
			(void)visible;
		};
	};
}