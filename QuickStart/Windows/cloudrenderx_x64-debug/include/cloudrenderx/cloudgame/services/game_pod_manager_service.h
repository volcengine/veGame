#pragma once
#include "game_pod_manager_listener.h"
#include <memory>

namespace vegame {
	/**
	 * @type api
	 * 
	 * @brief 云游戏场景中pod操作服务
	 * 
	 * @locale zh
	 */
	class NATIVE_SDK_API CloudGamePodManagerService {
	public:
		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudGamePodManagerService() = default;

		/**
		 * @type api
		 *
		 * @brief      异步更新无操作回收时长。
		 *			   更新结果可通过NoOperationRecycleTimeListener获知。
		 *
		 * @param [in] time_seconds : 无操作回收时长，单位为秒；范围[0, 36000]，即0到10小时;
		 *							 当time为0时，默认为300s。	
		 *
		 * @return              0 : 表示调用API成功
		 *                     -1 : 状态错误导致的调用失败
		 *         其他负数结果码 : 其他原因导致的调用失败
		 */
		virtual int setNoOperationRecycleTimeAsync(int time_seconds = 300) = 0;

		/**
		 * @type api
		 *
		 * @brief   异步获取当前正在生效的无操作回收时长;
		 *          具体结果通过NoOperationRecycleTimeListener获知。
		 *
		 * @return              0  : 调用API成功
		 *                     -1  : 状态错误导致的调用失败;
		 *          其他负数结果码 : 其他原因导致的调用失败;
		 */
		virtual int getNoOperationRecycleTimeAsync() = 0;

		/**
		 * @type api
		 *
		 * @brief      异步更新无操作回收倒计时。
		 *			   更新结果可通过NoOperationRecycleTimeListener获知。
		 *
		 * @param [in] time_seconds : 无操作回收时长，单位为秒；
		 *							 设置的时长不能超过无操作回收本身时长。
		 *							 如果设置为0， 不更改当前倒计时时长。
		 *							 如果设置为-1，则取消倒计时提醒功能。
		 *
		 * @return              0 : 表示调用API成功
		 *                     -1 : 状态错误导致的调用失败
		 *         其他负数结果码 : 其他原因导致的调用失败
		 */
		virtual int setIdleReminderDurationAsync(int time_seconds) = 0;

		/**
		 * @type api
		 *
		 * @brief      异步获取无操作回收倒计时时长;
		 *             具体结果通过NoOperationRecycleTimeListener获知。
		 *
		 *
	     * @return               0  : 调用API成功;
		 *                      -1  : 状态错误导致的调用失败;
		 *           其他负数结果码 : 其他原因导致的调用失败;
		 */
		virtual int getIdleReminderDurationAsync() = 0;

		/**
		 * @type api
		 *
		 * @brief      异步重置无操作回收倒计时。
		 *			   适用于倒计时开始后，重新产生了新的交互操作的场景。
		 *			   重置结果可通过NoOperationRecycleTimeListener获知。
		 *
		 * @param [in] time_seconds : 无操作回收时长，单位为秒；
		 *							 设置的时长不能超过无操作回收本身时长。
		 *							 如果设置为0， 不更改当前倒计时时长。
		 *							 如果设置为-1，则取消倒计时提醒功能。
		 *
		 * @return              0 : 表示调用API成功
		 *                     -1 : 状态错误导致的调用失败
		 *         其他负数结果码 : 其他原因导致的调用失败
		 */
		virtual int resetIdleReminderDurationAsync() = 0;

		/**
		 * @type api
		 *
		 * @brief      设置无操作回收时长监听器。
		 *
		 * @param [in] listener : 监听器对象，如果为空，则清空监听器。
		 *
		 * @return True表示设置成功，False表示设置失败
		 */
		virtual bool setNoOperationRecycleTimeListener(std::shared_ptr<CloudGameNoOperationRecycleTimeListener> listener) = 0;

		/**
		 * @type api
		 *
		 * @brief      异步更新保活(例如断网情况下)时长。
		 *			   更新结果可通过KeepAliveTimeListener获知。
		 *
		 * @param [in] time_seconds : 保活时长，单位为秒；范围[0, 7200];
		 *							 当time为0时，默认为300s。
		 *
		 * @return              0 : 表示调用API成功
		 *                     -1 : 状态错误导致的调用失败
		 *         其他负数结果码 : 其他原因导致的调用失败
		 */
		virtual int setKeepAliveTimeAsync(int time_seconds = 300) = 0;

		/**
		 * @type api
		 *
		 * @brief      设置保活时长监听器。
		 *
		 * @param [in] listener : 监听器对象，如果为空，则清空监听器。
		 *
		 * @return True表示设置成功，False表示设置失败
		 */
		virtual bool setKeepAliveTimeListener(std::shared_ptr<CloudGameKeepAliveTimeListener> listener) = 0;

		/**
		 * @type api
		 *
		 * @brief  启动保活计时
		 *
		 * @return              0 : 表示调用API成功
		 *                     -1 : 状态错误导致的调用失败
		 *         其他负数结果码 : 其他原因导致的调用失败
		 */
		virtual int startKeepAlive() = 0;

		/**
		 * @type api
		 *
		 * @brief  停止保活计时
		 *
		 * @return              0 : 表示调用API成功
		 *                     -1 : 状态错误导致的调用失败
		 *         其他负数结果码 : 其他原因导致的调用失败
		 */
		virtual int stopKeepAlive() = 0;
	};
}