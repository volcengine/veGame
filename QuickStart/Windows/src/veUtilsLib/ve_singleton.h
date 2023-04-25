#pragma once
#include <mutex>

template<class T>
static T& instance() {
    static bool ok = false;
    static std::mutex m;
    static T t;

    if (ok)
        return t;

    std::unique_lock<std::mutex> l(m);
    if (ok)
        return t;
    ok = true;
    return t;
}

template<class T>
class SingletonInSameModule {
public:
    static T& instance() {
        static bool ok = false;
        static std::mutex m;
        static T t;

        if (ok)
            return t;

        std::unique_lock<std::mutex> l(m);
        if (ok)
            return t;
        ok = true;
        return t;
    }

protected:
    SingletonInSameModule() {}
    virtual ~SingletonInSameModule() {}
};