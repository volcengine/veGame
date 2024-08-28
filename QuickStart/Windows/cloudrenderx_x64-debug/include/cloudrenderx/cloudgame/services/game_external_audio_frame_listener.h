#pragma once

#include <memory>

namespace vecommon {
	class ExternalAudioFrame;
}

namespace vegame {
	/**
	 * @type callback
	 * 
	 * @brief 外部渲染音频帧监听器
	 * 
	 * @locale zh
	 */
	class CloudgameExternalAudioFrameListener {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudgameExternalAudioFrameListener() = default;

		/**
		 * @type callback
		 *
		 * @brief 收到外部音频帧的回调，每隔10毫秒回调1次
		 *
		 * @param [in] result   : 外部音频帧
		 */
		virtual void onReceiveExternalAudioFrame(std::shared_ptr<const vecommon::ExternalAudioFrame> external_audio_frame) = 0;
	};
}