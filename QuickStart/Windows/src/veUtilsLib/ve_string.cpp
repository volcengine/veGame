#include "pch.h"
#include "ve_string.h"
#include "ve_log_lite.h"


template<typename T>
static bool splitstringInternal(const std::basic_string<T, std::char_traits<T>, std::allocator<T>> & src, \
                                const std::basic_string<T, std::char_traits<T>, std::allocator<T>> & delim, \
                                std::vector<std::basic_string<T, std::char_traits<T>, std::allocator<T>>> & segs);


bool splitString(const std::string& src, const std::string& delim, std::vector<std::string>& segs) {
    return splitstringInternal<char>(src, delim, segs);
}

bool splitString(const std::wstring & src, const std::wstring & delim, std::vector<std::wstring> & segs) {
    return splitstringInternal<wchar_t>(src, delim, segs);
}

bool isWhiteSpace(const char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

bool isWhiteSpace(const wchar_t c) {
    return c == L' ' || c == L'\n' || c == L'\r' || c == L'\f' || c == L'\v';
}

std::string toLower(const std::string & s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
        [](char c) {
            return std::tolower(c);
        }
    );

    return r;
}

std::wstring toLower(const std::wstring & s) {
    std::wstring r = s;
    std::transform(r.begin(), r.end(), r.begin(),
        [](wchar_t c) {
            return std::tolower(c);
        }
    );

    return r;
}

std::string toUpper(const std::string & s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
        [](char c) {
            return std::toupper(c);
        }
    );

    return r;
}

std::wstring toUpper(const std::wstring & s) {
    std::wstring r = s;
    std::transform(r.begin(), r.end(), r.begin(),
        [](wchar_t c) {
            return std::toupper(c);
        }
    );

    return r;
}

std::string gbkToUtf8(const std::string& gbk_str) {
    std::vector<wchar_t> buff(gbk_str.size());
#ifdef _WIN32
    std::locale loc("zh-CN");
#else
    std::locale loc("zh_CN.GB18030");
#endif
    wchar_t* pwszNext = nullptr;
    const char* pszNext = nullptr;
    mbstate_t state = {};
    int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t>>
        (loc).in(state,
            gbk_str.data(), gbk_str.data() + gbk_str.size(), pszNext,
            buff.data(), buff.data() + buff.size(), pwszNext);

    if (std::codecvt_base::ok == res) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
        return cutf8.to_bytes(std::wstring(buff.data(), pwszNext));
    }
    return "";
}

std::string utf8ToGbk(const std::string& utf8_str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
    std::wstring wTemp = cutf8.from_bytes(utf8_str);
#ifdef _WIN32
    std::locale loc("zh-CN");
#else
    std::locale loc("zh_CN.GB18030");
#endif
    const wchar_t* pwszNext = nullptr;
    char* pszNext = nullptr;
    mbstate_t state = {};

    std::vector<char> buff(wTemp.size() * 2);
    int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t>>
        (loc).out(state,
            wTemp.data(), wTemp.data() + wTemp.size(), pwszNext,
            buff.data(), buff.data() + buff.size(), pszNext);

    if (std::codecvt_base::ok == res) {
        return std::string(buff.data(), pszNext);
    }
    return "";
}

std::string unicodeToUtf8(const std::wstring& wstr) {
    std::string str;

    int nLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    str.resize((size_t)nLen);

    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), (LPSTR)str.c_str(), nLen, NULL, NULL);

    return str;
}

std::string toHex(const std::string & s) {
    if (s.empty()) {
        return "";
    }

    // 
    static const char hex2digit[] = "0123456789ABCDEF";
    std::string tmp(s.length() * 2, '\0');
    char * ptr_h = &tmp[0];
    char * ptr_s = const_cast<char *>(&s[0]);
    char * ptr_s_end = ptr_s + s.length();
    while (ptr_s < ptr_s_end) {
        *ptr_h++ = hex2digit[(*ptr_s >> 4) & 0x0F];
        *ptr_h++ = hex2digit[*ptr_s & 0x0F];
        ++ptr_s;
    }

    return tmp;
}

std::string fromHex(const std::string & h) {
    if (h.empty()) {
        return "";
    }
    if (h.length() % 2 > 0) {
        veLOGW("hex length {} is invalid", h.length());
        return "";
    }

    // 
    auto func_check = [](const char c) -> bool {
        return !(c < '0' || c >'F' || (c > '9' && c < 'A'));
    };
    auto func_calc_c = [](const char c) -> unsigned char {
        if (c >= '0' && c <= '9') {
            return static_cast<unsigned char>(c - '0');
        }
        if (c >= 'A' && c <= 'F') {
            return static_cast<unsigned char>(c - 'A') + 0x0A;
        }

        return 0;
    };
    auto func_calc = [func_calc_c](const char hi, const char lo) -> const char {
        unsigned char c = 0;
        c = func_calc_c(hi) << 4;
        c += func_calc_c(lo);
        return *reinterpret_cast<char *>(&c);
    };

    // 
    std::string tmp(h.length() / 2, '\0');
    char * ptr_s = &tmp[0];
    char * ptr_h = const_cast<char *>(&h[0]);
    char * ptr_h_end = ptr_h + h.length();
    while (ptr_h < ptr_h_end) {
        char high = *ptr_h++;
        char low = *ptr_h++;
        if (!func_check(high) || !func_check(low)) {
            veLOGW("{} is not a hex std::string", h);
            return "";
        }
        *ptr_s++ = func_calc(high, low);
    }

    return tmp;
}

template<typename T>
bool splitstringInternal(const std::basic_string<T, std::char_traits<T>, std::allocator<T>> & src, \
                         const std::basic_string<T, std::char_traits<T>, std::allocator<T>> & delim, \
                         std::vector<std::basic_string<T, std::char_traits<T>, std::allocator<T>>> & segs) {
    if (src.empty())
        return true;
    if (delim.empty()) {
        segs.push_back(src);
        return true;
    }

    // 未找到分隔符
    size_t pos = src.find(delim);
    if (pos == std::basic_string<T, std::char_traits<T>, std::allocator<T>>::npos) {
        segs.push_back(src);
        return true;
    }

    // 寻找分段
    size_t last_pos = 0;
    do {
        if (pos - last_pos > 0) {
            std::basic_string<T, std::char_traits<T>, std::allocator<T>> seg = src.substr(last_pos, pos - last_pos);
            segs.push_back(seg);
        }
        last_pos = pos + 1;
        if (last_pos + 1 > src.length())
            break;
        pos = src.find(delim, last_pos);
    } while (pos != std::basic_string<T, std::char_traits<T>, std::allocator<T>>::npos);

    // 剩余部分处理
    if (last_pos < src.length()) {
        std::basic_string<T, std::char_traits<T>, std::allocator<T>> seg = src.substr(last_pos);
        segs.push_back(seg);
    }

    return true;
}

std::wstring stringToWstring(const std::string& str) {
    std::wstring result;
    //获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), NULL, 0);
    result.resize((size_t)len);
    //多字节编码转换成宽字节编码  
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), (LPWSTR)result.data(), len);
    return result;
}

std::string wstringTostring(const std::wstring& wstr) {
    std::string result;
    //获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    result.resize((size_t)len);
    //宽字节编码转换成多字节编码  
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), (LPSTR)result.data(), len, NULL, NULL);
    return result;
}
