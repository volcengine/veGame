#pragma once
#include <stdint.h>
#include <memory>
#include "ve_define.h"
namespace vecommon {
    /**
    * @type api
	* @locale zh
    *
    * @brief 音频采样率挡位
    *
    */
    enum class AudioSampleRate {
        UNKNWON = -2,

        /**
         * @brief 8000Hz
         */
        Hz8000 = 8000,

        /**
         * @brief 16000Hz
         */
        Hz16000 = 16000,

        /**
         * @brief 32000Hz
         */
        Hz32000 = 32000,

        /**
         * @brief 44100Hz
         */
        Hz44100 = 44100,

        /**
         * @brief 48000Hz
         */
        Hz48000 = 48000
    };

    /**
    * @type api
    * 
	* @locale zh
    * 
    * @brief 音频通道类型
    *
    */
    enum class AudioChannel {
        UNKNWON = -2,

        /**
         * @brief 默认设置。
         */
        Auto = -1,
        /**
         * @brief 单声道
         */
        Mono = 1,
        /**
         * @brief 双声道
         */
        Stereo = 2
    };

    /**
    * @type api
    * 
	* @locale zh
    *
    * @brief 音频帧类型
    *
    */
    enum class AudioFrameType {
        UNKNWON = -1,

        /**
        * @brief PCM 16bit
        */
        PCM16 = 0
    };

    /**
    * @type api
    * 
	* @locale zh
    *
    * @brief 外部音频帧
    *
    */
	class ExternalAudioFrame {
	public:
        /**
         *
         * @brief 析构函数
         *
         */
        virtual ~ExternalAudioFrame() = default;

        /**
         * @brief 获取音频帧时间戳，单位：微秒
         */
        virtual int64_t timestampUs() const = 0;

        /**
         * @brief 获取音频采样率
         */
        virtual AudioSampleRate sampleRate() const = 0;

        /**
         * @brief 获取音频通道
         */
        virtual AudioChannel channel() const = 0;

        /**
         * @brief 获取音频数据
         */
        virtual uint8_t* data() const = 0;

        /**
         * @brief 获取音频数据大小
         */
        virtual int dataSize() const = 0;

        /**
         * @brief 获取音频格式
         */
        virtual AudioFrameType frameType() const = 0;

        /**
         * @brief 释放音频帧
         */
        virtual void release() = 0;

        /**
         * @brief 是否是静默音频
         */
        virtual bool isMutedData() const = 0;
	};

   /**
    * @type api
    * 
	* @locale zh
    *
    * @brief     创建外部音频帧。
    *
    * @param [in] sameple_rate     : 音频采样率。
    * @param [in] audio_channel    : 音频数据通道。
    * @param [in] audio_frame_type : 音频数据类型，目前只支持PCM。
    * @param [in] extra_params     : 额外的创建参数。
    *
    * @return nullptr表示设置失败;
    */
    NATIVE_SDK_API std::shared_ptr<ExternalAudioFrame> createExternalAudioFrame(
        vecommon::AudioSampleRate sameple_rate,
        vecommon::AudioChannel audio_channel,
        vecommon::AudioFrameType audio_frame_type = vecommon::AudioFrameType::PCM16,
        void* extra_params = nullptr
    );

   /**
    * @type api
    * 
	* @locale zh
    *
    * @brief   更新外部音频帧
    *
    *
    * @return  发送结果；具体结果码说明：
    *						0  : 成功；
    *                     -1 : 无效参数导致的更新失败
    *          其他负数结果码: 其他原因导致的更新失败
    *
    */
    NATIVE_SDK_API int updateExternalAudioFrame(
        std::shared_ptr<ExternalAudioFrame> external_frame,
        const uint8_t* audio_frame_data, 
        const size_t& audio_frame_data_len);
}