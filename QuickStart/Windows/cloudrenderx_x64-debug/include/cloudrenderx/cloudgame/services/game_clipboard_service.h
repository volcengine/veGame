#pragma once
#include <vector>
#include <string>
#include <memory>
#include "game_clipboard_listener.h"

namespace vegame {
    /**
     * @type api
     * 
     * @brief 云游戏场景中粘贴板操作服务
     * 
	 * @locale zh
     */
	class CloudGameClipboardService {
	public:
        /**
         *
         * @brief 析构函数
         *
         */
        virtual ~CloudGameClipboardService() = default;

        /**
         * @type api
         * @brief 向Pod端发送粘贴板的数据
         * 
         * @param [in] clipbord_content 粘贴板内容
         * 
         * @return  发送结果；具体结果码说明：
         *						0 : 成功；
         *                     -1 : 参数错误导致的发送失败
         *                     -2 : 当前角色不能发送消息
         *          其他负数结果码：其他原因导致的发送失败
         */
        virtual int sendClipData(const std::vector<std::string>& clipbord_content) = 0;


        /**
         * @type api
         * @brief 设置云端粘贴板监听
         */
        virtual void setClipboardListener(std::shared_ptr<ClipboardServiceListener> listener) = 0;
	};
}