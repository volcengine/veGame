#pragma once
#include <string>
namespace vegame {
    /**
     * @type callback
     * 
     * @brief 输入法监听回调
     * 
	 * @locale zh
     */
    class InputMethodListener {
    public:

        virtual ~InputMethodListener() = default;

        /**
         * @type callback
         * 
         * @brief pod获取到输入法焦点
         * 
         * @param [in] hitText   : 输入框提示字符
         * @param [in] inputType : 输入框类型
         */
        virtual void onCommondPrepare(const std::string& hintText, int inputType) {
            (void)hintText;
            (void)inputType;
        };

        /**
         * @type callback
         * @brief 输入法弹出，可能会反复触发
         */
        virtual void onCommondShow() {
        };

        /**
         * @type callback
         * @brief 输入法隐藏
         */
        virtual void onCommondHide() {
        };

        /**
         * @type callback
         * @brief 文本内容发生改变
         * @param [in] text   : 发送改变的文字
         */
        virtual void onTextChange(const std::string& text) {
            (void)text;
        };

        /**
         * @type callback
         * @brief pod输入法可用
         */
        virtual void onPodInputMethodEnabled() {
        };

        /**
         * @type callback
         * @brief pod输入法不可用
         */
        virtual void onPodInputMethodDisabled() {
        };
    };
}