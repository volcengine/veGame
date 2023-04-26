#include "pch.h"
#include "demo_utils.h"

std::string getClipboardString() {
    std::string res;

    EmptyClipboard();
    if (!OpenClipboard(nullptr)) {
        return res;
    }
    HANDLE h_data = GetClipboardData(CF_TEXT);
    if (h_data == NULL) {
        return res;
    }

    do {
        char* tmp_text = static_cast<char*>(GlobalLock(h_data));
        if (!tmp_text) {
            break;
        }

        res = tmp_text;
        GlobalUnlock(h_data);
    } while (false);

    if (!CloseClipboard()) {
        return res;
    }
    return res;
}
