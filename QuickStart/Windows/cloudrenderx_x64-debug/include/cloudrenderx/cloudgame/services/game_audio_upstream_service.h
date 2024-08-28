#pragma once
#include <memory>
#include "game_audio_upstream_listener.h"

namespace vecommon {
	class ExternalAudioFrame;
}

namespace vegame {
	/**
	 * @type api
	 * 
	 * @brief 云游戏场景中上行音频服务
	 * 
	 * @locale zh
	 */
	class CloudgameAudioUpstreamService {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudgameAudioUpstreamService() = default;

		/**
		 * @type api
		 *
		 * @brief     设置上行音频监听器。
		 *
		 * @param [in] listener       : 监听器对象，如果为空，则清空监听器。
		 *
		 * @return True表示设置成功，False表示设置失败;
		 */

		virtual bool setAudioUpStreamListener(std::shared_ptr<CloudgameAudioUpStreamListener> listener) = 0;

		/**
		 * @type api
		 *
		 * @brief     开启外部音频上传。
		 *			  建议在CloudgameAudioUpStreamListener::onReceiveAudioUpstreamStartRequestFromRemote回调中调用此方法;
		 *
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 运行状态不对导致的失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int startExternalAudioUpstream() = 0;


		/**
		 * @type api
		 *
		 * @brief     推送外部音频帧。
		 *            建议每10ms推送1次;
		 *            调用前必须确保CloudgameAudioUpstreamService::startExternalAudioUpstream方法调用成功
		 *
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 运行状态不对导致的失败
		 *                     -2 : 未调用CloudgameAudioUpstreamService::startExternalAudioUpstream方法，开启外部音频上传
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int pushExternalAudioFrame(std::shared_ptr<vecommon::ExternalAudioFrame> extenral_audio_frame) = 0;

		/**
		 * @type api
		 *
		 * @brief     停止外部音频上传。
		 *			  建议在CloudgameAudioUpStreamListener::onReceiveAudioUpstreamStopRequestFromRemote回调中调用此方法;
		 *
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 运行状态不对导致的失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int stopExternalAudioUpstream() = 0;
	};
}