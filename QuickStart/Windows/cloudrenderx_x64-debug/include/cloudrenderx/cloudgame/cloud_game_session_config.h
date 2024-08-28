#pragma once

#include "ve_xsession_config.h"
#include "ve_cloud_defines.h"
struct CloudGameStreamConfig;
/**
 * @type api
 * 
 * @brief 云游戏场景配置
 * 
 * @locale zh
 */
typedef struct _CloudGameSessionConfig {

    /**
     * session 基础配置
     */
    XSessionConfig basicConfig;

    /**
    * 游戏窗口的宽度
    */
    int width;

    /**
    * 游戏窗口的高度
    */
    int height;
    /**
     * 游戏 ID
     */
    std::string gameId = "";
    /**
     * 自定义游戏 ID
     */
    std::string customGameId = "";
    /**
     * 本次生命周期标识符
     */
    std::string roundId = "";
    /**
     * 资源预锁定 ID
     */
    std::string reservedId = "";
    /**
     * 保存用户游戏配置文件的路径列表
     */
    std::string profilePathList = "";
    /**
     * 是否锁定屏幕横竖屏显示
     */
    bool enableScreenLock{false};
    /**
     * 是否开启本地键盘输入
     */
    bool enableLocalKeyboard{true};

    /**
     * 无操作下线倒计时时长，单位s，0则表示使用默认倒计时时长，-1表示取消倒计时
     */
    int32_t idle_reminder_duration;

    /**
     * 会话模式：
     * 0 : 常规模式
     */
    int sessionMode = 0;

    /**
     * 自动订阅的流类型
     */
    vecommon::StreamType streamType = vecommon::StreamType::Both;

    /**
     * 启动模式：
     * 0 : 正常模式
     * 1 : 直接启动模式，跳过paas调度，直接启动流引擎，适合融合平台场景
     */
    int startMode = 0;

    /**
     * 直接启动流引擎的配置;
     * 
     * 只有当startMode等于1时，设置此参数才有效
     */
    std::shared_ptr<CloudGameStreamConfig> streamConfig = nullptr;

} CloudGameSessionConfig;

/**
 * @type api
 * @brief 直接启动流引擎的配置
 */
struct CloudGameStreamConfig {
    /**
     * 流APP ID
     */
    const char* streamAppId{ nullptr };

    /**
     * 流房间ID
     */
    const char* streamRoomId{ nullptr };

    /**
     * 加房时token
     */
    const char* streamToken{ nullptr };

    /**
     * 额外的流引擎启动参数；
     * cloudrenderx会直接透传
     */
    const char* extraStreamStartParams{ nullptr };

    /**
     * 自定义的编码配置，可为空
     * 
     */
    std::shared_ptr<const vecommon::VideoEncodeConfig> video_encode_config{nullptr};


    /**
     *  外部session id, 如果不为空，则需要将之替换成此次session的id
     */
    std::string outter_session_id = "";

    /**
     *  外部account id, 目前不为空时，只用作埋点
     */
    std::string outter_account_id = "";

    /**
     *  rtc libra实验所需的business_id
     */
    std::string business_id = "";
};