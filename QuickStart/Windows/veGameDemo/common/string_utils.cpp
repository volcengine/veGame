#include "string_utils.h"
#include <algorithm>
#include <windows.h>
#include <WinNls.h>

// 是否全是数字
bool cloudrenderx_demo::isDigits(const std::string& str, bool allow_negative) {
    if (str.empty()) {
        return false;
    }
    auto it = str.begin();
    if (allow_negative && (*it) == '-') {
        // 允许负数
        it++;
    }
    return std::all_of(it, str.end(), ::isdigit);
}