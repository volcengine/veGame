#pragma once

/**
 * @type api
 * 
 * @brief 云渲染场景基础配置 
 */
typedef struct _XSessionConfig {

    /**
     * 拉流后显示画面的画布，Windows 平台下传窗口句柄（HWND）
     */
    void* canvas{nullptr};
    /**
     * 设置用户无操作自动回收时长,单位为秒
     */
    int autoRecycleTime{0};
    /**
     * 视频流清晰度 ID
     */
    int videoStreamProfileId{0};
    /**
     * 用户ID
     */
    std::string userId = "";
    /**
     * 鉴权临时 access key
     */
    std::string ak = "";
    /**
     * 鉴权临时 secret key
     */
    std::string sk = "";
    /**
     * 鉴权临时 token
     */
    std::string token = "";
    /**
     * 用户标签
     */
    std::string userTag = "";
    /**
     * 火山侧套餐 ID
     */
    std::string configurationCode = "";
    /**
     * 机房ID
     */
    std::string dc = "";
    /**
     * 客户扩展参数，只透传不消费
     */
    std::string extra = "";
    /**
     * Debug自定义配置
     */
    std::string debugConfig = "";
    /**
     * 是否启用外部渲染功能
     */
    bool enable_external_render{false};

} XSessionConfig;