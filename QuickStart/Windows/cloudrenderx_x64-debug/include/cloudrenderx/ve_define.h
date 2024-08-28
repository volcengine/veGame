#pragma once

#ifdef NATIVE_SDK_EXPORTS
#ifdef _WIN32
#define NATIVE_SDK_API __declspec(dllexport)
#else
#define NATIVE_SDK_API
#endif
#else
#ifdef _WIN32
#define NATIVE_SDK_API __declspec(dllimport)
#else
#define NATIVE_SDK_API
#endif
#endif // NATIVESDK_EXPORTS