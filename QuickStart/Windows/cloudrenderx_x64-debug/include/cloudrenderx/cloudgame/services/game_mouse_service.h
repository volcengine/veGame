#pragma once
#include <memory>
#include <string>
#include "ve_define.h"
#include "game_mouse_listener.h"

namespace vecommon {
	class MouseCursorPos;
	class MouseMoveData;
	class MouseKeyData;
	class MouseWheelData;
	enum class CursorStyleMode;
}

namespace vegame {

	/**
	 * @type api
	 * 
	 * @brief 云游戏场景中鼠标操作服务
	 * 
	 * @locale zh
	 */
	class NATIVE_SDK_API CloudGameMouseService {
	public:

		/**
		 *
		 * @brief 析构函数
		 *
		 */
		virtual ~CloudGameMouseService() = default;

		/**
		 * @type api
		 *
		 * @brief      设置光标icon改变消息的id；
		 *			   当光标icon生效时，会基于此消息id，发送一个消息通知业务层
		 *
		 * @param [in] message_id : 消息ID。
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 参数错误导致的发送失败
		 *                     -2 : 当前角色不能发送消息
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int setCursorImgChangeMessageId(const uint16_t& message_id) = 0;

		/**
		 * @type api
		 *
		 * @brief      设置鼠标样式模式。
		 *
		 * @param [in] cursor_style_mode : 鼠标样式模式。
		 *								   vecommon::CursorStyleMode::Local : 使用本地鼠标icon
		 *                                 vecommon::CursorStyleMode::Remote: 使用pod侧返回的鼠标icon
		 *								   具体鼠标样式的生效由SDK内部完成，生效时，会通过setCursorImgChangeMessageId方法设置的消息ID，发送一个消息通知业务层
		 * 
		 * @param [in] extra_params      : 额外参数，例如，windows系统中的窗口handler
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 参数错误导致的发送失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int setCursorStyleMode(const vecommon::CursorStyleMode& cursor_style_mode) = 0;

		/**
		 * @type api
		 *
		 * @brief      设置本地鼠标图片，只有vecommon::CursorStyleMode::Local此API设置的图片才会生效。
		 *
		 * @param [in] local_img_path : 鼠标光标本地图片。
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 参数错误导致的发送失败
		 *					   -2 : 路径不存在
		 *                     -3 : 路径对应的不是常规文件类型
		 *					   -4 : 路径对应的文件不是cur类型文件或者不是ani类型文件
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int setDefaultCursorImg(const std::string& local_img_path) = 0;

		/**
		 * @type api
		 *
		 * @brief      发送鼠标光标位置事件。
		 *
		 * @param [in] mouse_cursor_pos : 鼠标光标位置。
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 参数错误导致的发送失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int sendCursorPosEvent(const vecommon::MouseCursorPos& mouse_cursor_pos) = 0;

		/**
		 * @type api
		 *
		 * @brief      设置鼠标移动灵敏度。
		 *
		 * @param [in] sensitivity_factor : 灵敏度因子，默认为1.0f。
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 参数错误导致的发送失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int setMouseMoveSensitivity(const float& sensitivity_factor) = 0;

		/**
		 * @type api
		 *
		 * @brief      发送鼠标移动事件。
		 *
		 * @param [in] mouse_move_data : 鼠标移动数据。
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 参数错误导致的发送失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int sendMoveEvent(const vecommon::MouseMoveData& mouse_move_data) = 0;

		/**
		 * @type api
		 *
		 * @brief      发送鼠标按键事件。
		 *
		 * @param [in] move_button_data : 鼠标按键数据。
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 参数错误导致的发送失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int sendButtonEvent(const vecommon::MouseKeyData& move_button_data) = 0;

		/**
		 * @type api
		 *
		 * @brief      发送鼠标滚轮事件。
		 *
		 * @param [in] move_button_data : 鼠标滚轮数据。
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 参数错误导致的发送失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int sendWheelEvent(const vecommon::MouseWheelData& move_wheel_data) = 0;

		/**
		 * @type api
		 *
		 * @brief      设置鼠标监听器。
		 *
		 * @param [in] listener       : 监听器对象，如果为空，则清空监听器。
		 *
		 * @return True表示设置成功，False表示设置失败
		 */
		virtual bool setMouseListener(std::shared_ptr<CloudGameMouseListener> listener) = 0;

		/**
		 * @type api
		 *
		 * @brief      设置鼠标move事件的发送频率。
		 *
		 * @param [in] duration       : 每隔多少毫秒发送1次鼠标move事件。
		 *
		 * @return  发送结果；具体结果码说明：
		 *						0 : 成功；
		 *                     -1 : 参数错误导致的发送失败
		 *          其他负数结果码：其他原因导致的发送失败
		 *
		 */
		virtual int setSendMouseMoveEventDuration(const int64_t& duration) = 0;
	};
}