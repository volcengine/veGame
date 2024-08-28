#pragma once

#include "ve_define.h"
#include <string>

namespace vegame {
	/**
	 * @type api
	 * 
	 * @brief 云游戏场倒计时状态
	 * 
	 * @locale zh
	 */
	enum class NATIVE_SDK_API CloudGameCountdownState {
		Start,
		Unknown
	};

	/**
	 * @type api
	 * 
	 * @brief 云游戏保活事件监听器
	 */
	class NATIVE_SDK_API CloudGameKeepAliveTimeListener {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudGameKeepAliveTimeListener() = default;

		/**
		 * @type callback
		 *
		 * @brief  更新保活时长失败回调。
		 *
		 * @param [in] code  : 错误码
		 * @param [in] msg   : 错误信息
		 */
		virtual void onErrorForKeepAliveTime(int code, const std::string& msg) = 0;

		/**
		 * @type callback
		 *
		 * @brief  KeepAliveTime保活开始。
		 */
		virtual void onKeepAliveStarted() = 0;

		/**
		 * @type callback
		 *
		 * @brief  KeepAliveTime保活停止。
		 */
		virtual void onKeepAliveStopped() = 0;
	};

	/**
	 * @type api
	 *
	 * @brief 云游戏无操作回调时间相关操作的监听器
	 */
	class NATIVE_SDK_API CloudGameNoOperationRecycleTimeListener {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudGameNoOperationRecycleTimeListener() = default;

		/**
		 * @type callback
		 *
		 * @brief  更新保活时长或倒计时时长失败回调。
		 *
		 * @param [in] code  : 错误码
		 * @param [in] msg   : 错误信息
		 */
		virtual void onErrorForNoOperationRecycleTime(int code, const std::string& msg) = 0;

		/**
		 * @type callback
		 *
		 * @brief  pod无操作回收时长变更回调。
		 *
		 * @param [in] time_seconds  : 单位: s
		 * 
		 */
		virtual void onNoOperationRecycleTimeUpdated(int time_seconds) = 0;

		/**
		 * @type callback
		 *
		 * @brief  pod无操作回收倒计时变更回调。
		 *
		 * @param [in] time_seconds  : 单位: s
		 *
		 */
		virtual void onCountdownUpdated(int time_seconds) = 0;

		/**
		 * @type callback
		 *
		 * @brief  pod侧触发/取消无操作回收倒计时。
		 *
		 * @param [in] status        : 当前倒计时状态，详见CloudGameCountdownState
		 * @param [in] time_seconds  : 具体倒计时时长，单位: s
		 *
		 */
		virtual void onCountdown(CloudGameCountdownState state, int time_seconds) = 0;

		/**
		 * @type callback
		 *
		 * @brief  重置空闲提醒倒计时的结果的回调。
		 *
		 * @param [in] status  : 1：成功， 0：失败。
		 *
		 */
		virtual void onResetIdleReminderDuration(int status) = 0;
	};
}