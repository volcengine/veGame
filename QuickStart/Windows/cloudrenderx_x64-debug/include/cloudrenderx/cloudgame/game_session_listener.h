#pragma once

#include <string>
#include "ve_cloud_defines.h"
/**
 * @type api
 * 
 * @locale zh
 * 
 * @brief 云游戏场景生命周期监听器
 */
class CloudGameSessionListener {
public:
    virtual ~CloudGameSessionListener() = default;

    /**
     * @type callback
	 * @locale zh
     * @brief 启动成功
     * @param [in] video_stream_profile, 清晰度档位  <br>
     * @param [in] round_id   <br>
     * @param [in] target_id  云手机pod_id或云游戏game_id <br>
     * @param [in] reserved_id   <br>
     * @param [in] plan_id，套餐id    <br>
     *
     */
    virtual void onStartSuccess(int video_stream_profile, const char* round_id, const char* target_id, const char* reserved_id, const char* plan_id) {
        (void)video_stream_profile;
        (void)round_id;
        (void)target_id;
        (void)reserved_id;
        (void)plan_id;
    }

    /**
     * @type callback
     * @brief 云游戏/云手机停止回调
	 * @locale zh
     */
    virtual void onStop() {
    }

    /**
     * @type callback
     * @brief 收到音频首帧回调
	 * @locale zh
     */
    virtual void onFirstRemoteAudioFrame() {
    }

    /**
     * @type callback
     * @brief 收到视频首帧回调
	 * @locale zh
     * @param [in] info， 参考{@link#VideoFrameInfo}
     */
    virtual void onFirstRemoteVideoFrame(const vecommon::VideoFrameInfo& info) {
        (void)info;
    };

    /**
     * @type callback
     * @brief 拉流连接状态改变回调
	 * @locale zh
     * @param [in] state， 参考{@link#StreamConnectionState}
     */
    virtual void onStreamConnectionStateChanged(vecommon::StreamConnectionState state) {
        (void)state;
    }

    /**
     * @type callback
	 * @locale zh
     * @brief 视频帧大小改变回调
     */
    virtual void onVideoSizeChanged(const vecommon::VideoFrameInfo& info) {
        (void)info;
    }

    /**
     * @type callback
	 * @locale zh
     * @brief 网络类型改变回调
     * 
     * • -1： 网络连接类型未知。
     * •  0： 网络连接已断开。
     * •  1： 网络类型为 LAN 。
     * •  2： 网络类型为 Wi-Fi（包含热点）。
     * •  3： 网络类型为 2G 移动网络。
     * •  4： 网络类型为 3G 移动网络。
     * •  5： 网络类型为 4G 移动网络。
     *
     */
    virtual void onNetworkTypeChanged(int type) {
        (void)type;
    }

    /**
     * @type callback
     * @brief 网络质量回调，2 秒回调一次
	 * @locale zh
     */
    virtual void onStreamNetworkQualityUpdated(vecommon::NetworkQualityState state) {
        (void)state;
    }

    /**
     * @type callback
     * @brief 音频流统计信息回调，2 秒回调一次
	 * @locale zh
     */
    virtual void onRemoteAudioStreamStats(const vecommon::AudioStats& stats) {
        (void)stats;
    }

    /**
     * @type callback
     * @brief 视频流统计信息回调，2 秒回调一次
	 * @locale zh
     */
    virtual void onRemoteVideoStreamStats(const vecommon::VideoStats& stats) {
        (void)stats;
    }

    /**
     * @type callback
     * @brief SDK内部产生的错误回调
	 * @locale zh
     * @param [in] code，参考开发者文档
     */
    virtual void onError(int code, const std::string msg) {
        (void)code;
        (void)msg;
    }
    
    /**
     * @type callback
     * @brief SDK内部产生的警告回调
	 * @locale zh
     * @param [in] code，参考开发者文档
     */
    virtual void onWarning(int code) {
        (void)code;
    }
};
