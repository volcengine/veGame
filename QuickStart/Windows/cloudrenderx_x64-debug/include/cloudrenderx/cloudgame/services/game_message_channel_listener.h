#pragma once

#include "ve_define.h"
namespace vecommon {
	class ChannelMessage;

	class ChannelBinaryMessage;
}
namespace vegame {
	/**
	 * @type api
	 * 
	 * @brief 云游戏场景消息通道监听器
	 * 
	 * @locale zh
	 */
	class NATIVE_SDK_API CloudGameMessageChannelListener {
	public:

		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudGameMessageChannelListener() = default;

		/**
		 * @type callback
		 *
		 * @brief  接收到字符串消息。
		 *
		 * @param [in] channel_message  : 接收到的消息。注意此参数为引用类型，避免在回调方法外直接使用此参数。
		 */
		virtual void onReceiveMessage(const vecommon::ChannelMessage& channel_message) {
			(void)channel_message;
		}

		/**
		 * @type callback
		 *
		 * @brief  接收到二进制消息。
		 *
		 * @param [in] channel_message  : 接收到的消息。注意此参数为引用类型，避免在回调方法外直接使用此参数。
		 */
		virtual void onReceiveBinaryMessage(const vecommon::ChannelBinaryMessage& channel_message) {
			(void)channel_message;
		}

		/**
		 * @type callback
		 *
		 * @brief  发送消息回调结果。
		 *
		 * @param [in] code  : 发送结果：0表示成功；-1表示失败
		 * @param [in] mid   : 消息ID
		 */
		virtual void onSentResult(const int code, const std::string& mid) {
			(void)code;
			(void)mid;
		}

		/**
		 * @type callback
		 *
		 * @brief  消息通道的连接状态发送改变
		 *
		 * @param [in] remote_uid  : 消息通道的远端用户ID
		 * @param [in] status      : 1表示已连接；2表示已断连
		 */
		virtual void onConnectionStatusChanged(const std::string& remote_uid, const int status) {
			(void)remote_uid;
			(void)status;
		}

		/**
		 * @type callback
		 *
		 * @brief  消息通道的错误回调
		 *
		 * @param [in] code  : 错误码
		 */
		virtual void onError(const int code) {
			(void)code;
		}
	};
}