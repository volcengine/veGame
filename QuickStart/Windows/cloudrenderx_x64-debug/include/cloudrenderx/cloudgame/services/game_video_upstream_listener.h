#pragma once

namespace vegame {
	/**
	 * @type callback
	 * 
	 * @brief 上行视频监听器
	 * 
	 * @locale zh
	 */
	class CloudgameVideoUpStreamListener {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudgameVideoUpStreamListener() = default;

		/**
		 * @type callback
		 *
		 * @brief pod端请求开始外部视频流上传
		 *
		 */
		virtual void onReceiveVideoUpstreamStartRequestFromRemote() = 0;

		/**
		 * @type callback
		 *
		 * @brief pod端请求关闭外部视频流上传
		 *
		 */
		virtual void onReceiveVideoUpstreamStopRequestFromRemote() = 0;
	};
}