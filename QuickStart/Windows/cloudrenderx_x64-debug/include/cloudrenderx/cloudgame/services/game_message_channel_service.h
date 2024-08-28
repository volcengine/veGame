#pragma once

#include <memory>
#include <string>
#include "ve_define.h"
#include "game_message_channel_listener.h"
namespace vecommon {
	class ChannelMessage;

	class ChannelBinaryMessage;
}

#define DEFAULT_MCC_TIMEOUT_FOR_CLOUDGAME 30 * 1000L

namespace vegame {
	/**
	 * @type api
	 * 
	 * @brief 云游戏场景中消息通道服务
	 * 
	 * @locale zh
	 */
	class NATIVE_SDK_API CloudGameMessageChannelService {
	public:

		/**
		*
		* @brief 析构函数
		*
		*/
		virtual ~CloudGameMessageChannelService() = default;

		/**
		 * @type api
		 *
		 * @brief      通过消息通道，发送普通字符串数据。
		 *
		 * @param [in] payload       : 待发送消息。
		 * @param [in] needAck       : 是否需要接收方发送ack确认消息。
		 * @param [in] destUid       : 接收方的用户ID。
		 * @param [in] timeoutMillis : 等待发送结果的超时时间，单位为毫秒。
		 *
		 * @return  发送的消息信息对象; 发送失败，则返回nullptr
		 */
		virtual std::shared_ptr<const vecommon::ChannelMessage> sendMessage(
			const std::string& payload,
			bool needAck = false,
			const std::string& destUid = "",
			uint64_t timeoutMillis = DEFAULT_MCC_TIMEOUT_FOR_CLOUDGAME) = 0;

		/**
		 * @type api
		 *
		 * @brief      通过消息通道，发送二进制数据。
		 *
		 * @param [in] payload       : 待发送消息。
		 * @param [in] payload_len   : 待发送消息长度。
		 * @param [in] needAck       : 是否需要接收方发送ack确认消息。
		 * @param [in] destUid       : 接收方的用户ID。
		 * @param [in] timeoutMillis : 等待发送结果的超时时间，单位为毫秒。
		 *
		 * @return  发送的消息信息对象; 发送失败，则返回nullptr
		 */
		virtual std::shared_ptr<const vecommon::ChannelBinaryMessage> sendBinaryMessage(
			const uint8_t* payload,
			const size_t payload_len,
			bool needAck = false,
			const std::string& destUid = "",
			uint64_t timeoutMillis = DEFAULT_MCC_TIMEOUT_FOR_CLOUDGAME) = 0;

		/**
		 * @type api
		 *
		 * @brief      通过消息通道，发送带key的二进制数据。
		 *
		 * @param [in] key           : 待发送消息对应的key，禁止为空，需确保远端侧可识别此key。
		 * @param [in] payload       : 待发送消息。
		 * @param [in] payload_len   : 待发送消息长度。
		 * @param [in] needAck       : 是否需要接收方发送ack确认消息。
		 * @param [in] destUid       : 接收方的用户ID。
		 * @param [in] timeoutMillis : 等待发送结果的超时时间，单位为毫秒。
		 *
		 * @return  发送的消息信息对象; 发送失败，则返回nullptr
		 */
		virtual std::shared_ptr<const vecommon::ChannelBinaryMessage> sendBinaryMessageWithKey(
			const std::string& key,
			const uint8_t* payload,
			const size_t payload_len,
			bool needAck = false,
			const std::string& destUid = "",
			uint64_t timeoutMillis = DEFAULT_MCC_TIMEOUT_FOR_CLOUDGAME) = 0;

		/**
		 * @type api
		 *
		 * @brief      设置消息通道监听器。
		 *
		 * @param [in] listener       : 监听器对象，如果为空，则清空监听器。
		 *
		 * @return True表示设置成功，False表示设置失败
		 */
		virtual bool setMessageChannelListener(std::shared_ptr<CloudGameMessageChannelListener> listener) = 0;
	};
}