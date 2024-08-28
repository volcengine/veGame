
#pragma once
#include <memory>
#include <string>
#include "ve_define.h"
#include "x_config.h"

#include "cloudgame/cloud_game_session.h"
#include "cloudgame/cloud_game_session_config.h"


namespace vecloudrenderx {
/**
 * @type api
 *
 * @brief 云渲染引擎
 * @locale zh
 *
 */
class VeCloudRenderX {

public:

    NATIVE_SDK_API VeCloudRenderX() = default;

    NATIVE_SDK_API ~VeCloudRenderX() = default;

    /**
     * @type api
     * @brief 准备VeCloudRenderX SDK
     * @note 使用VeCloudRenderX能力之前，必须调用此api进行初始化.
     */
    NATIVE_SDK_API bool prepare(XConfig& config);

    
    /**
     * @type api
     * @brief      创建cloud game session.
     * @param [in] config 创建cloud game session的配置参数.
     */
    NATIVE_SDK_API std::shared_ptr<CloudGameSession> createCloudGameSession(const CloudGameSessionConfig& config);
    
            

    /**
     * @type api
     * @brief 是否设置调试模式
     */
    NATIVE_SDK_API void setDebug(bool debug);

    /**
     * @type api
     * @brief 获取设备ID
     */
    NATIVE_SDK_API const std::string getDeviceId();

    /**
     * @type api
     * @brief 获取SDK版本号
     */
    NATIVE_SDK_API const std::string getSDKVersion();

};

} // namespace vecommon
