#include "pch.h"
#include "ve_log_lite.h"

static std::shared_ptr<spdlog::logger> spdLogInstance_ = nullptr;
static std::shared_ptr<spdlog::logger> msvcLogInstanc_ = nullptr;

bool setUpLocalSpdLog(const std::string& str_log_path, const std::string& str_tag) {
    if (spdLogInstance_) {
        return true;
    }
    spdlog::set_level(spdlog::level::debug);
    spdlog::flush_every(std::chrono::seconds(3));
    auto max_size = 1048576 * 5;
    auto max_files = 2;
    spdLogInstance_ = spdlog::rotating_logger_mt(str_tag, str_log_path, max_size, max_files, false);
    spdLogInstance_->set_pattern("[%L][%H:%M:%S.%e > %3o][T%5t][P%5P][%n] %v");
    spdLogInstance_->info("===SpdLog Ceated, Name:{} ===", str_tag);

    return true;
}

std::shared_ptr<spdlog::logger> spdLog() {
    std::string log_dir = ".\\log\\";
    DWORD pid = GetCurrentProcessId();
    if (!spdLogInstance_) {
        static std::mutex mtx;
        mtx.lock();
        if (!spdLogInstance_) {
            setUpLocalSpdLog(log_dir + "ve_sdk." + std::to_string(pid) + ".log", "volc_engine");
        }
        mtx.unlock();
    }
    return spdLogInstance_;
}

std::shared_ptr<spdlog::logger> msvcLog() {
    if (!msvcLogInstanc_) {
        static std::mutex mtx;
        mtx.lock();
        if (!msvcLogInstanc_) {
            auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
            msvcLogInstanc_ = std::make_shared<spdlog::logger>("msvc_logger", sink);
            msvcLogInstanc_->set_pattern("[%L][%H:%M:%S %e->%5o][%n][T:%5t][%s:%#] %v");
        }
        mtx.unlock();
    }
    return msvcLogInstanc_;
}
