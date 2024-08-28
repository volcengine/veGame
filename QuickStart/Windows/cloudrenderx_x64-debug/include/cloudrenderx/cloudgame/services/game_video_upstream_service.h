#pragma once

#include <memory>
#include "game_video_upstream_listener.h"

namespace vecommon {
	class ExternalVideoFrame;
}

namespace vegame {

	/**
	 * @type api
	 * 
	 * @brief 云游戏场景中上行视频服务
	 * 
	 * @locale zh
	 * 
	 */
	class CloudgameVideoUpstreamService {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudgameVideoUpstreamService() = default;

		/**
		 * @type api
		 *
		 * @brief     设置上行视频监听器。
		 *
		 * @param [in] listener       : 监听器对象，如果为空，则清空监听器。
		 *
		 * @return True表示设置成功，False表示设置失败;
		 */

		virtual bool setVideoUpStreamListener(std::shared_ptr<CloudgameVideoUpStreamListener> listener) = 0;

		/**
		 * @type api
		 *
		 * @brief     开始通过内部采集的方式推送camera视频流。
		 *			  建议在CloudgameVideoUpStreamListener::onReceiveVideoUpstreamStartRequestFromRemote回调中调用此方法;
		 *
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 运行状态不对导致的失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int startCameraUpstream() = 0;

		/**
		 * @type api
		 *
		 * @brief     停止通过内部采集的方式推送camera视频流。
		 *			  建议在CloudgameVideoUpStreamListener::onReceiveVideoUpstreamStartRequestFromRemote回调中调用此方法;
		 *
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 运行状态不对导致的失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int stopCameraUpstream() = 0;

		/**
		 * @type api
		 *
		 * @brief     开始上传指定的视频流。
		 *			  建议在CloudgameVideoUpStreamListener::onReceiveVideoUpstreamStartRequestFromRemote回调中调用此方法;
		 *
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 运行状态不对导致的失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int startExternalVideoUpstream() = 0;

		/**
		 * @type api
		 *
		 * @brief     推送外部视频帧。
		 *            调用前必须确保CloudgameVideoUpstreamService::startExternalVideoUpstream方法调用成功，且视频流类型必须是VideoStreamType::External
		 *
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 运行状态不对导致的失败
		 *                     -2 : 未调用CloudgameVideoUpstreamService::startExternalVideoUpstream方法
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int pushExternalVideoFrame(std::shared_ptr<vecommon::ExternalVideoFrame> extenral_video_frame) = 0;

		/**
		 * @type api
		 *
		 * @brief     停止上传指定的视频流。
		 *			  建议在CloudgameVideoUpStreamListener::onReceiveVideoUpstreamStopRequestFromRemote回调中调用此方法;
		 *
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 运行状态不对导致的失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int stopExternalVideoUpstream() = 0;
	};
}
