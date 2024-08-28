#pragma once
#include <vector>
#include <string>

namespace vegame {
    /**
     * @type callback
     * 
     * @brief 云端粘贴板事件回调
     * 
	 * @locale zh
     */
    class ClipboardServiceListener {
    public:
        /**
         *
         * @brief 析构函数
         *
         */
        virtual ~ClipboardServiceListener() = default;

        /**
         * @type callback
         *
         * @brief 接收到云端粘贴板内容事件
         *
         * @param [in] clipboard_content   : 云端粘贴板内容
         */
        virtual void onReceivedClipData(const std::vector<std::string>& clipbord_content) {
            (void)clipbord_content;
        }
    };
}