#pragma once
#include <memory>

namespace vecommon {
	class ExternalVideoFrame;
}

namespace vegame {
	/**
	 * @type callback
	 * 
	 * @brief 外部渲染视频帧监听器
	 * 
	 * @locale zh
	 */
	class CloudgameExternalVideoFrameListener {
	public:
	   /**
		*
		* @brief 析构函数
		*
		*/
		virtual ~CloudgameExternalVideoFrameListener() = default;

		/**
		 * @type callback
		 *
		 * @brief 收到外部视频帧的回调
		 *
		 * @param [in] result   : 外部视频帧
		 */
		virtual void onReceiveExternalVideoFrame(std::shared_ptr<const vecommon::ExternalVideoFrame> external_video_frame) = 0;
	};
}