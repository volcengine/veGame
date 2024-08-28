#pragma once
#include <string>

namespace vecloudrenderx {
	/**
	 * @type api
	 * @brief 云游戏场景中进程维度的配置项
	 * @locale zh
	 *
	 */
	struct XConfig
	{	
	   /**
		* @locale zh
		* @brief  账号ID
		*/
		std::string accountId;

	    /**
		 * @locale zh
		 * @brief 进程级别的instance handler， windows平台下为HINSTANCE
		 */
		void* process_instance;
	};
}