#pragma once
#include "ve_define.h"
namespace vecommon {
	namespace error {
		/**
		 * @type api
		 *
		 * @brief      是否是paas返回的错误码
		 *
		 * @param [in] error_code : 待判断的错误码。
		 *
		 */
		NATIVE_SDK_API bool isPaasErrorCode(int error_code);


		/**
		 * @type api
		 * @locale zh
		 *
		 * @brief      是否是创建RTC实例失败的错误码
		 *
		 * @param [in] error_code : 待判断的错误码。
		 *
		 */
		NATIVE_SDK_API bool isCreateRTCErrorCode(int error_code);

		/**
		 * @type api
		 * @locale zh
		 *
		 * @brief      是否是rtc 流相关的错误码
		 *
		 * @param [in] error_code : 待判断的错误码。
		 *
		 */
		NATIVE_SDK_API bool isRTCStreamErrorCode(int error_code);

		/**
		 * @type api
		 * @locale zh
		 *
		 * @brief      是否是无效start参数相关的错误码
		 *
		 * @param [in] error_code : 待判断的错误码。
		 *
		 */
		NATIVE_SDK_API bool isParamCheckErrorCode(int error_code);


		/**
		 * @type api
		 * @locale zh
		 *
		 * @brief      是否是多用户登录导致的pod退出
		 *
		 * @param [in] error_code : 待判断的错误码。
		 *
		 */
		NATIVE_SDK_API bool isPodExitByDulipcateUser(int error_code);

		/**
		 * @type api
		 * @locale zh
		 *
		 * @brief      是否是由于游戏本身崩溃、游戏内主动退出、操作超时等原因导致的pod退出
		 *
		 * @param [in] error_code : 待判断的错误码。
		 *
		 */
		NATIVE_SDK_API bool isPodExitErrorCode(int err_code);


		/**
		 * @type api
		 * @locale zh
		 *
		 * @brief      是否是网络错误(包含http请求、响应错误，例如，404等)
		 *
		 * @param [in] error_code : 待判断的错误码。
		 *
		 */
		NATIVE_SDK_API bool isNetworkErrorCode(int error_code);


		/**
		 * @type api
		 * @locale zh
		 *
		 * @brief      是否是解析paas返回的response内容失败，导致的错误
		 *
		 * @param [in] error_code : 待判断的错误码。
		 *
		 */
		NATIVE_SDK_API bool isHttpResponseFromatError(int err_code);
	}
}