//
// Created by stuka on 24.10.2025.
//

#pragma once

#include "SGCore/Utils/Platform.h"
#include <fmt/format.h>
#include <string_view>

#if SG_PLATFORM_OS_ANDROID

#include <android/log.h>

#define LOGCAT_I(tag, msg, ...) SGCore::__AndroidImpl::Logcat::print<true>(ANDROID_LOG_INFO, tag, msg, ##__VA_ARGS__);
#define LOGCAT_D(tag, msg, ...) SGCore::__AndroidImpl::Logcat::print<true>(ANDROID_LOG_DEBUG, tag, msg, ##__VA_ARGS__);
#define LOGCAT_W(tag, msg, ...) SGCore::__AndroidImpl::Logcat::print<true>(ANDROID_LOG_WARN, tag, msg, ##__VA_ARGS__);
#define LOGCAT_E(tag, msg, ...) SGCore::__AndroidImpl::Logcat::print<true>(ANDROID_LOG_ERROR, tag, msg, ##__VA_ARGS__);
#define LOGCAT_V(tag, msg, ...) SGCore::__AndroidImpl::Logcat::print<true>(ANDROID_LOG_VERBOSE, tag, msg, ##__VA_ARGS__);

#define LOGCAT_I_UNFORMATTED(tag, msg) SGCore::__AndroidImpl::Logcat::print<false>(ANDROID_LOG_INFO, tag, msg);
#define LOGCAT_D_UNFORMATTED(tag, msg) SGCore::__AndroidImpl::Logcat::print<false>(ANDROID_LOG_DEBUG, tag, msg);
#define LOGCAT_W_UNFORMATTED(tag, msg) SGCore::__AndroidImpl::Logcat::print<false>(ANDROID_LOG_WARN, tag, msg);
#define LOGCAT_E_UNFORMATTED(tag, msg) SGCore::__AndroidImpl::Logcat::print<false>(ANDROID_LOG_ERROR, tag, msg);
#define LOGCAT_V_UNFORMATTED(tag, msg) SGCore::__AndroidImpl::Logcat::print<false>(ANDROID_LOG_VERBOSE, tag, msg);

namespace SGCore::__AndroidImpl::Logcat
{
    template<bool EnableFormatting, typename... Args>
    using log_msg_t = std::conditional_t<EnableFormatting, fmt::format_string<Args...>, std::string_view>;

    template<bool EnableFormatting, typename... ArgsT>
    static void print(android_LogPriority priority, const std::string& tag, log_msg_t<EnableFormatting, ArgsT...>  msg, ArgsT&&... formatArgs) noexcept
    {
        std::string formattedMsg;
        if constexpr(EnableFormatting)
        {
            formattedMsg = fmt::format(msg, std::forward<ArgsT>(formatArgs)...);
        }
        else
        {
            formattedMsg = msg;
        }

        __android_log_write(priority, tag.c_str(), formattedMsg.c_str());
    }
}

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