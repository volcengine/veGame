#pragma once
#include <spdlog/spdlog.h>
#include "ve_define.h"

#define dbgLOG msvcLog()


#define veLOGD(...)                           \
	if (cloudrenderx_demo::getLoggerDebugOn()){                  \
		try{                                  \
            cloudrenderx_demo::spdLog()->debug(__VA_ARGS__);     \
		}catch(const spdlog::spdlog_ex& ex){  \
                                              \
		}                                     \
    }                                         

#define veLOGI(...)                           \
	try{									  \
        cloudrenderx_demo::spdLog()->info(__VA_ARGS__);          \
        cloudrenderx_demo::spdLog()->flush();                    \
	}catch(const spdlog::spdlog_ex& ex){      \
                                              \
	}                                     

#define veLOGW(...)                           \
	try{                                      \
        cloudrenderx_demo::spdLog()->warn(__VA_ARGS__);          \
        cloudrenderx_demo::spdLog()->flush();                    \
	}catch(const spdlog::spdlog_ex& ex){      \
											  \
	}                                     

#define veLOGE(...)                           \
	try{                                      \
        cloudrenderx_demo::spdLog()->error(__VA_ARGS__);         \
        cloudrenderx_demo::spdLog()->flush();                    \
	}catch(const spdlog::spdlog_ex& ex){      \
                                              \
	}                                         \


namespace cloudrenderx_demo {

NATIVE_SDK_API std::shared_ptr<spdlog::logger> spdLog();

NATIVE_SDK_API void setLoggerDebugOn(bool debug_on);

NATIVE_SDK_API bool getLoggerDebugOn();

}
