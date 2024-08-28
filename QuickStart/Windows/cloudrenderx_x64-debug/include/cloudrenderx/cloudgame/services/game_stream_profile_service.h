#pragma once

#include "game_stream_profile_listener.h"

namespace vegame {
	/**
	 * @type api
	 * 
	 * @brief 云游戏场景中串流简况服务
	 * 
	 * @locale zh
	 */
	class CloudgameStreamProfileService {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudgameStreamProfileService() = default;

		/**
		 * @type api
		 *
		 * @brief     异步切换当前视频流的清晰度挡位，结果可在CloudgameStreamProfileListener::onStreamProfileIdChanged中获取。
		 *
		 * @param [in] stream_profile_id   : 清晰度挡位，详见：
		 *									 手游清晰度档位说明：https://www.volcengine.com/docs/6512/1256324
		 *									 端游清晰度档位说明：https://www.volcengine.com/docs/6512/1316450
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 运行状态不对导致的失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int setStreamProfileIdAsync(int stream_profile_id) = 0;

		/**
		 * @type api
		 *
		 * @brief     异步获取当前视频流的清晰度挡位，结果可在CloudgameStreamProfileListener::onStreamProfileIdChanged中获取。
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 运行状态不对导致的失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int getStreamProfileIdAsync() = 0;

		/**
		 * @type api
		 *
		 * @brief     同步获取当前视频流的分辨率。
		 * 
		 * @param [out] out_resolution   : 将结果分辨率填充到此指针中
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 运行状态不对导致的失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int getResolution(std::pair<size_t, size_t>* out_resolution) = 0;

		/**
		 * @type api
		 *
		 * @brief     同步获取当前视频流的帧率。
		 *
		 * @param [out] out_frame_rate   : 将结果帧率填充到此引用中
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 运行状态不对导致的失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int getFrameRate(int* out_frame_rate) = 0;

		/**
		 * @type api
		 *
		 * @brief     同步获取当前视频流的码率。
		 *
		 * @param [out] out_bitrate   : 将结果码率填充到此引用中
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 运行状态不对导致的失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int getBitrate(int* out_bitrate) = 0;

		/**
		 * @type api
		 *
		 * @brief     设置stream简况监听器。
		 *
		 * @param [in] listener       : 监听器对象，如果为空，则清空监听器。
		 *
		 * @return True表示设置成功，False表示设置失败;
		 */

		virtual bool setStreamProfileListener(std::shared_ptr<CloudgameStreamProfileListener> listener) = 0;
	};
}