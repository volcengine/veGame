#pragma once

namespace vecommon {
   /**
    * @type api
    * 
	* @locale zh
    *
    * @brief 鼠标样式模式
    *
    */
    enum class CursorStyleMode {
        Local, // 本地模式
        Remote // pod远程模式
    };

   /**
    * @type api
    * 
	* @locale zh
    *
    * @brief 鼠标光标位置事件
    *
    */
    struct MouseCursorPos {
        /**
         * 画面宽度
         */
        int32_t width{ 0 };
        /**
         * 画面高度
         */
        int32_t height{ 0 };
        /**
         * 相对于当前画面宽度，光标的x坐标
         */
        int32_t x{ 0 };
        /**
         * 相对于当前画面高度，光标的y坐标
         */
        int32_t y{ 0 };

    };

   /**
    * @type api
    * 
	* @locale zh
    *
    * @brief 鼠标移动事件
    *
    */
    struct MouseMoveData {
        /**
         * 鼠标在画面的 x 轴坐标，左-->右 映射到 [0,65535]
         */
        int32_t absX{ 0 };
        /**
         * 鼠标在画面的 y 轴坐标，上-->下 映射到 [0,65535]
         */
        int32_t absY{ 0 };
        /**
         * 相对上次鼠标位置 x 方向移动值，左负，右正
         */
        int32_t deltaX{ 0 };
        /**
         * 相对上次鼠标位置 y 方向移动值，上负，下正
         */
        int32_t deltaY{ 0 };

    };

   /**
    * @type api
    * 
	* @locale zh
    *
    * @brief 鼠标按键事件
    *
    */
    struct MouseKeyData {
        /**
         * 鼠标按键 key code，参考 https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
         */
        uint8_t key{ 0 };
        /**
         * 鼠标按键状态   0 up, 1 down
         */
        uint8_t down{ 0 };
        /**
         * 鼠标在画面的 x 轴坐标，左-->右 映射到 [0,65535]
         */
        int32_t absX{ 0 };
        /**
         * 鼠标在画面的 y 轴坐标，上-->下 映射到 [0,65535]
         */
        int32_t absY{ 0 };

    };

   /**
    * @type api
    * 
	* @locale zh
    *
    * @brief 鼠标滚轮事件
    *
    */
    typedef struct MouseWheelData {
        /**
         * 鼠标滚轮竖向滚动值
         */
        int16_t wheel{ 0 };
        /**
         * 鼠标滚轮横向滚动值
         */
        int16_t hwheel{ 0 };
        /**
         * 鼠标在画面的 x 轴坐标，左-->右 映射到 [0,65535]
         */
        int32_t absX{ 0 };
        /**
         * 鼠标在画面的 y 轴坐标，上-->下 映射到 [0,65535]
         */
        int32_t absY{ 0 };

    } ;

   /**
    * @type api
    * 
	* @locale zh
    *
    * @brief 鼠标滚轮事件(arm平台)
    *
    */
    struct MouseWheelDataArm {
        /**
         * 鼠标滚轮竖向滚动值，映射到 [0,65535]
         */
        double x{ 0 };
        /**
         * 鼠标滚轮横向滚动值，映射到 [0,65535]
         */
        double y{ 0 };
        /**
         * 表示上下方向，-1.0 下 1.0 上
         */
        double axisV{ 0 };
        /**
         * 表示左右方向，-1.0 右 1.0 左
         */
        double axisH{ 0 };
        /**
         * 动作类型，0-ACTION_DOWN, 1-ACTION_UP, 2-ACTION_MOVE 8-ACTION_SCROLL
         */
        int16_t action{ -1 };
        /**
         * 0:左键， 1:右键， 2:中键
         */
        int16_t button{ -1 };

    };
}