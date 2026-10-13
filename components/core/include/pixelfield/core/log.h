//
// Created by stuka on 13.10.2026.
//

#pragma once

#include <functional>
#include <string_view>

namespace pf::core
{
    enum class LogLevel
    {
        kTrace,
        kDebug,
        kInfo,
        kWarn,
        kError,
        kFatal
    };

    struct LogCallback final
    {
        bool enabled {};
        std::function<void(LogLevel, std::string_view)> function;

        void operator()(LogLevel level, std::string_view msg) const noexcept
        {
            if(enabled && function)
            {
                function(level, msg);
            }
        }
    };
}