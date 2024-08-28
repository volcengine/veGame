#pragma once

namespace vegame {
	/**
	 * @type callback
	 * 
	 * @brief 上行音频监听器
	 * 
	 * @locale zh
	 */
	class CloudgameAudioUpStreamListener {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudgameAudioUpStreamListener() = default;

		/**
		 * @type callback
		 *
		 * @brief pod端请求外部音频流上传
		 *
		 */
		virtual void onReceiveAudioUpstreamStartRequestFromRemote() = 0;

		/**
		 * @type callback
		 *
		 * @brief pod端请求关闭外部音频流上传
		 *
		 */
		virtual void onReceiveAudioUpstreamStopRequestFromRemote() = 0;
	};
}