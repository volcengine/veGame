#pragma once

#define dbgLOG msvcLog()

#define veLOGD(...) spdLog()->debug(__VA_ARGS__)
#define veLOGI(...) spdLog()->info(__VA_ARGS__)
#define veLOGW(...) spdLog()->warn(__VA_ARGS__)
#define veLOGE(...) spdLog()->error(__VA_ARGS__)

bool setUpLocalSpdLog(const std::string& str_log_path, const std::string& str_tag);
std::shared_ptr<spdlog::logger> spdLog();
std::shared_ptr<spdlog::logger> msvcLog();
