// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#define WIN32_LEAN_AND_MEAN
// windows
#include <Windows.h>
#include <WinInet.h>
#include <winnt.h>
#include <Shlobj.h>

// c++ 
#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>
#include <codecvt>
#include <locale>


// 3rd party
#include <spdlog\spdlog.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

// 
#include "sha256/sha256.h"
#include "sha256/hmac_sha256.h"

#endif //PCH_H
