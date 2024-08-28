#pragma once
#include <string>
namespace vecommon {
    class DelayDetectResult;
}
namespace vegame {
    /**
     * @type callback
     * 
     * @brief 延迟探测监听回调
     * 
	 * @locale zh
     */
    class CloudGameDelayDetectListener {
    public:
        /**
         *
         * @brief 析构函数
         *
         */
        virtual ~CloudGameDelayDetectListener() = default;

        /**
         * @type callback
         *
         * @brief 收到延迟探测结果的回调
         *
         * @param [in] result   : 延迟探测结果
         */
        virtual void onReceiveDelayDetectResult(const vecommon::DelayDetectResult& result) {
            (void)result;
        };
    };
}