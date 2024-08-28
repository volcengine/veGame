#pragma once
#include <string>

namespace vegame {
	/**
	 * @type callback
	 * 
	 * @brief stream简况监听器
	 * 
	 * @locale zh
	 */
	class CloudgameStreamProfileListener {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudgameStreamProfileListener() = default;

		/**
		 * @type callback
		 * 
		 * @brief 当前视频流的清晰度挡位发生变化时，会触发该回调
		 *
		 * @param [in] old_profile_id   : 变化前的清晰度ID
		 * @param [in] new_profile_id   : 变化后的清晰度ID
		 *
		 */
		virtual void onStreamProfileIdChanged(int old_profile_id, int new_profile_id) = 0;

		/**
		 * @type callback
		 *
		 * @brief 切换当前视频流的清晰度失败时，会触发该回调
		 *
		 * @param [in] error_code   : 错误码
		 * @param [in] error_msg    : 错误信息
		 * 
		 *
		 */
		virtual void onError(int error_code, std::string error_msg) = 0;
	};
}