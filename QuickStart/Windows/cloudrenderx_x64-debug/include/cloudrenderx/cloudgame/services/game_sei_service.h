#pragma once
#include "game_sei_listener.h"
#include <memory>

  /**
	* @type api
	* 
	* @brief 云游戏场景中sei服务
	* 
	 * @locale zh
	*/
namespace vegame {
	class NATIVE_SDK_API CloudGameSEIService {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudGameSEIService() = default;

		/**
		 * @type api
		 *
		 * @brief      设置sei监听器。
		 *
		 * @param [in] listener : 监听器对象，如果为空，则清空监听器。
		 *
		 * @return True表示设置成功，False表示设置失败
		 */
		virtual bool setSEIListener(std::shared_ptr<CloudGameSEIListener> listener) = 0;
	};
}