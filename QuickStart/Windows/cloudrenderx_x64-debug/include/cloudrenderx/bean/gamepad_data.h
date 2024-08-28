#pragma once

namespace vecommon {
    /**
    * @type api
    * 
	* @locale zh
    *
    * @brief 游戏手柄按键相关的输入数据结构
    *
    */
    struct GamepadInputData {
        /**
         * 手柄索引值
         */
        uint8_t index{ 0 };
        /**
         * 手柄按键值，参数 msdn XINPUT_GAMEPAD 结构 https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_gamepad
         */
        int32_t buttons{ 0 };
        /**
         * left trigger，范围 [0, 256)
         */
        uint8_t lt{ 0 };
        /**
         * right trigger，范围 [0, 256)
         */
        uint8_t rt{ 0 };
        /**
         * left thumbstick x，范围 [-32768, 32768)
         */
        int16_t lx{ 0 };
        /**
         * left thumbstick y，范围 [-32768, 32768)
         */
        int16_t ly{ 0 };
        /**
         * right thumbstick x，范围 [-32768, 32768)
         */
        int16_t rx{ 0 };
        /**
         * right thumbstick y，范围 [-32768, 32768)
         */
        int16_t ry{ 0 };

        bool operator!=(const GamepadInputData& other) const {
            return buttons != other.buttons
                || index != other.index
                || lt != other.lt
                || rt != other.rt
                || lx != other.lx
                || ly != other.ly
                || rx != other.rx
                || ry != other.ry;
        }
    };
}