#pragma once

namespace vecommon {
	/**
	 * @type api
	 * @locale zh
	 * @brief 延迟探测结果数据结构
	 */
	struct DelayDetectResult {
		/**
		 *  云端一共检测了多少帧
		 */
		int check_frames{0};

		/**
		 *  发送延迟探测事件到本次结果被回调之间时间间隔，单位为毫秒
		 */
		int64_t elapse{0};

		/**
		 *  云端检测的帧索引
		 */
		int frame_index{0};

		/**
		 *  云端检测的时间间隔，单位为毫秒
		 */
		int intervals{0};
	};
}