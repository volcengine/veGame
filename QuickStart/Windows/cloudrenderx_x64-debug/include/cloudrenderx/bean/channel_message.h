#pragma once

#include "ve_define.h"
#include <string>
namespace vecommon {
    /**
    * @type api
    * 
	* @locale zh
    *
    * @brief 消息通道字符串类型的消息
    *
    */
    class ChannelMessage {
    public:
        /**
         * @type api
         * @brief 获取消息ID
         */
        NATIVE_SDK_API virtual const std::string getMid() const = 0;

        /**
         * @type api
         * @brief 获取消息发送方的uid
         */
        NATIVE_SDK_API virtual const std::string getSourceChannelUid() const = 0;


        /**
         * @type api
         * @brief 获取消息接收方的uid
         */
        NATIVE_SDK_API virtual const std::string geDestinationChannelUid() const = 0;

        /**
         * @type api
         * @brief 获取携带的消息内容
         */
        NATIVE_SDK_API virtual const std::string& getPayload() const = 0;

        /**
         * @type api
         * @brief 获取消息发送时间戳,单位为毫秒
         */
        NATIVE_SDK_API virtual const time_t getTimeMillis() const = 0;
    };

    /**
    * @type api
    * 
	* @locale zh
    *
    * @brief 消息通道二进制类型的消息
    *
    */
    class ChannelBinaryMessage {
    public:
        /**
         * @type api
         * @brief 获取消息ID
         */
        NATIVE_SDK_API virtual const std::string getMid() const = 0;

        /**
         * @type api
         * @brief 获取消息发送方的uid
         */
        NATIVE_SDK_API virtual const std::string getSourceChannelUid() const = 0;


        /**
         * @type api
         * @brief 获取消息接收方的uid
         */
        NATIVE_SDK_API virtual const std::string geDestinationChannelUid() const = 0;

        /**
         * @type api
         * @brief 获取携带的消息内容
         */
        NATIVE_SDK_API virtual const uint8_t* getPayload() const = 0;

        /**
         * @type api
         * @brief 获取携带的消息内容长度
         */
        NATIVE_SDK_API virtual const size_t getPayloadLen() const = 0;

        /**
         * @type api
         * @brief 获取消息发送时间戳,单位为毫秒
         */
        NATIVE_SDK_API virtual const time_t getTimeMillis() const = 0;

        /**
         * @type api
         * @brief 获取二进制消息对应的key，可能为空;
         */
        NATIVE_SDK_API virtual const std::string getKey() const = 0;
    };
}