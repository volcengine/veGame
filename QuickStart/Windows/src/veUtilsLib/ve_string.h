#pragma once


// 字符串分割
bool splitString(const std::string& src, const std::string& delim, std::vector<std::string>& segs);
bool splitString(const std::wstring& src, const std::wstring& delim, std::vector<std::wstring>& segs);

// 
bool isWhiteSpace(const char c);
bool isWhiteSpace(const wchar_t c);

// 大小写转换
std::string toLower(const std::string& s);
std::wstring toLower(const std::wstring& s);
std::string toUpper(const std::string& s);
std::wstring toUpper(const std::wstring& s);

// 字符转换
std::string gbkToUtf8(const std::string& gbk_str);
std::string utf8ToGbk(const std::string& utf8_str);
std::string unicodeToUtf8(const std::wstring& uni_str);

// 十六进制工具
std::string toHex(const std::string& s);
std::string fromHex(const std::string& h);

// 
std::wstring stringToWstring(const std::string& str);
std::string wstringTostring(const std::wstring& wstr);

