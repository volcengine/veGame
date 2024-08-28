#pragma once
#include "game_external_video_frame_listener.h"
#include "game_external_audio_frame_listener.h"

namespace vegame {
	/**
	 * @type api
	 * 
	 * @brief 云游戏场景中外部渲染服务
	 * 
	 * @locale zh
	 */
	class CloudgameExternalRenderService {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudgameExternalRenderService() = default;

		/**
		 * @type api
		 *
		 * @brief      设置外部视频帧监听器。
		 *
		 * @param [in] listener       : 监听器对象，如果为空，则清空监听器。
		 *
		 * @return True表示设置成功，False表示设置失败;
		 */

		virtual bool setCloudgameExternalVideoFrameListener(std::shared_ptr<CloudgameExternalVideoFrameListener> listener) = 0;

		/**
		 * @type api
		 *
		 * @brief      设置外部音频帧监听器。
		 *
		 * @param [in] listener       : 监听器对象，如果为空，则清空监听器。
		 *
		 * @return True表示设置成功，False表示设置失败;
		 */

		virtual bool setCloudgameExternalAudioFrameListener(std::shared_ptr<CloudgameExternalAudioFrameListener> listener) = 0;
	};
}