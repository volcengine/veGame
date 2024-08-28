#pragma once
#include "game_input_method_listener.h"
#include <memory>
namespace vecommon {
    enum IMESendTextMode;
}
namespace vegame {
    /**
     * @type api
     * 
     * @brief 输入法相关接口
     * 
	 * @locale zh
     */
    class CloudGameInputMethodService {

    public:

        /**
         *
         * @brief 析构函数
         *
         */
        virtual ~CloudGameInputMethodService() = default;

        /**
         * @type api
         * @brief 向Pod端输入框发送文本
         * @param [in] text 文本内容
         * @param [in] mode 输入模式，参考InputMethodService::Mode定义
         * @return  发送结果；具体结果码说明：
	     *						0 : 成功；
	     *                     -1 : 参数错误导致的发送失败
	     *                     -2 : 当前角色不能发送消息
	     *          其他负数结果码：其他原因导致的发送失败
         */
        virtual int sendText(const std::string& text, vecommon::IMESendTextMode mode) = 0;

        /**
         * @type api
         * @brief 设置输入法是否生效
         * @param [in] enable 是否开启
         * @return  发送结果；具体结果码说明：
	     *						0 : 成功；
	     *                     -1 : 参数错误导致的发送失败
	     *                     -2 : 当前角色不能发送消息
	     *          其他负数结果码：其他原因导致的发送失败
         */
        virtual int enablePodInputMethod(bool enable) = 0;

        /**
         * @type api
         * @brief 获取输入法是否可用
         * @param [in] enable 是否开启
         * @return  云端输入法是否启用
         */
        virtual bool isInputMethodEnabled() = 0;

        /**
         * @type api
         * @brief 设置输入法监听
         */
        virtual void setInputMethodListener(std::shared_ptr<InputMethodListener> listener) = 0;
    };
}