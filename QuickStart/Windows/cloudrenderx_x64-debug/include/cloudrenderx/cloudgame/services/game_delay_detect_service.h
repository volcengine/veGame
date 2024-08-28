#pragma once
#include "game_delay_detect_listener.h"
#include <memory>
namespace vegame {
    /**
     * @type api
     * 
     * @brief 延迟探测服务
     * 
	 * @locale zh
     */
    class CloudGameDelayDetectService {
    public:
        /**
         *
         * @brief 析构函数
         *
         */
        virtual ~CloudGameDelayDetectService() = default;

        /**
         * @type api
         *
         * @brief      设置延迟探测监听器。
         *
         * @param [in] listener       : 监听器对象，如果为空，则清空监听器。
         *
         * @return True表示设置成功，False表示设置失败
         */
        virtual bool setCloudGameDelayDetectListener(std::shared_ptr<CloudGameDelayDetectListener> listener) = 0;

        /**
         * @type api
         * 
         * @deprecated ，谨慎调用此API，后续会删除;
         *
         * @brief      是否使用json格式发送延迟探测事件。
         *
         * @param [in] listener       : 监听器对象，如果为空，则清空监听器。
         *
         * @return True表示设置成功，False表示设置失败
         * @descri
         */
        virtual void useJson(bool use) = 0;
    };
}