//
// Created by stuka on 24.10.2025.
//

#pragma once

#include "SGCore/Utils/Platform.h"
#include <fmt/format.h>

#ifdef SG_PLATFORM_OS_ANDROID

#include <android/log.h>

#define LOGCAT_I(tag, msg, ...) __android_log_write(ANDROID_LOG_INFO, tag, fmt::format(msg, ##__VA_ARGS__).c_str());
#define LOGCAT_D(tag, msg, ...) __android_log_write(ANDROID_LOG_DEBUG, tag, fmt::format(msg, ##__VA_ARGS__).c_str());
#define LOGCAT_W(tag, msg, ...) __android_log_write(ANDROID_LOG_WARN, tag, fmt::format(msg, ##__VA_ARGS__).c_str());
#define LOGCAT_E(tag, msg, ...) __android_log_write(ANDROID_LOG_ERROR, tag, fmt::format(msg, ##__VA_ARGS__).c_str());
#define LOGCAT_V(tag, msg, ...) __android_log_write(ANDROID_LOG_VERBOSE, tag, fmt::format(msg, ##__VA_ARGS__).c_str());

#define LOGCAT_I_UNFORMATTED(tag, msg) __android_log_write(ANDROID_LOG_INFO, tag, msg);
#define LOGCAT_D_UNFORMATTED(tag, msg) __android_log_write(ANDROID_LOG_DEBUG, tag, msg);
#define LOGCAT_W_UNFORMATTED(tag, msg) __android_log_write(ANDROID_LOG_WARN, tag, msg);
#define LOGCAT_E_UNFORMATTED(tag, msg) __android_log_write(ANDROID_LOG_ERROR, tag, msg);
#define LOGCAT_V_UNFORMATTED(tag, msg) __android_log_write(ANDROID_LOG_VERBOSE, tag, msg);

#else

#define LOGCAT_I(tag, msg, ...)
#define LOGCAT_D(tag, msg, ...)
#define LOGCAT_W(tag, msg, ...)
#define LOGCAT_E(tag, msg, ...)
#define LOGCAT_V(tag, msg, ...)

#define LOGCAT_I_UNFORMATTED(tag, msg)
#define LOGCAT_D_UNFORMATTED(tag, msg)
#define LOGCAT_W_UNFORMATTED(tag, msg)
#define LOGCAT_E_UNFORMATTED(tag, msg)
#define LOGCAT_V_UNFORMATTED(tag, msg)

#endif