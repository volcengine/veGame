#pragma once
#include "ve_define.h"
/**
 * @type interface
 * 
 * @brief 云渲染会话接口
 * 
 * @locale zh
 */
class NATIVE_SDK_API XSession {

public:
    virtual ~XSession() = default;

    /**
     * @type api
     * @brief 启动会话
     */
    virtual void start() = 0;

    /**
     * @type api
     * @brief 停止会话
     */
    virtual void stop() = 0;

    /**
     * @type api
     * @brief 获取会话 ID
     */
    virtual const char* getSessionId() = 0;

};
