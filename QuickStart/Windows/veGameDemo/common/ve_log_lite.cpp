//#include "pch.h"
#include <memory>
#include <mutex>
#include <string>
#include "ve_log_lite.h"
#include <spdlog/sinks/rotating_file_sink.h>

namespace cloudrenderx_demo {
    static std::shared_ptr<spdlog::logger> spdLogInstance_ = nullptr;

    static bool enable_log = false;

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

    void setLoggerDebugOn(bool debug_on)
    {
        enable_log = debug_on;
    }

    bool getLoggerDebugOn()
    {
        return enable_log;
    }
}
