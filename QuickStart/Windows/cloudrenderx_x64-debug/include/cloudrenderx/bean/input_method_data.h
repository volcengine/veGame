#pragma once
namespace vecommon {
    /**
    * @type api
    * 
	* @locale zh
    *
    * @brief 发送输入法内容的模式
    *
    */
    enum IMESendTextMode {
        /**
         * 内容追加输入
         */
        APPEND = 0,
        /**
         * 内容覆盖输入
         */
        REPLACE = 1
    };
}