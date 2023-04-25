#include "pch.h"
#include "ve_time.h"

int64_t getCurrentTimeMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
