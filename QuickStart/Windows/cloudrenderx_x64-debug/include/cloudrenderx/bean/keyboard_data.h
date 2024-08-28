#pragma once
namespace vecommon {
    /**
    * @type api
    * 
	* @locale zh
    *
    * @brief 键盘事件数据结构
    *
    */
    struct KeyboardData {
        /**
         * 键盘按键 key，非 VK_PROCESSKEY 等处理后的值，需要填入正在输入的按键值，参考  <br>
         * https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
         */
        uint8_t key{ 0 };
        /**
         *  键盘按键状态   0 up, 1 down
         */
        uint8_t down{ 0 };
        /**
         * key 的值是否经过处理，如果 key 的值为 VK_PROCESSKEY，需要还原 key 的原始键值，并设置 process 为 ture
         */
        uint8_t processed{ 0 };
    };
}