#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
// windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinInet.h>
#include <winnt.h>
#include <iphlpapi.h>

#include <commdlg.h>
#include <d2d1.h>
#include <dwmapi.h>
#include <dwrite.h>
#include <hidusage.h>
#include <imm.h>
#include <shellapi.h>
#include <ShellScalingApi.h>
#include <Shlobj.h>
#include <ShObjIdl.h>
#include <ShObjIdl_core.h>
#include <rpc.h>
#include <wincodec.h>
#include <winrt/base.h>
#include <Xinput.h>

// c++ 
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <codecvt>
#include <locale>

// 3rd party
#include <spdlog/spdlog.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

// 
#include "json.hpp"
#include "cppcodec/base64_default_rfc4648.hpp"
#include "sha256/sha256.h"
#include "sha256/hmac_sha256.h"

#include "ve_crash_dumper.h"
#include "ve_log_lite.h"
#include "ve_singleton.h"
#include "ve_string.h"
#include "ve_time.h"

#endif //PCH_H
