//

#pragma once

#include <iostream>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Time.h"
#include "SGCore/Utils/Utils.h"
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "SGCore/Utils/Signal.h"

#define SG_LOG_T(msg, ...) SGCore::Logger::getDefaultLogger()->trace<true>(msg, ##__VA_ARGS__)
#define SG_LOG_D(msg, ...) SGCore::Logger::getDefaultLogger()->debug<true>(msg, ##__VA_ARGS__)
#define SG_LOG_I(msg, ...) SGCore::Logger::getDefaultLogger()->info<true>(msg, ##__VA_ARGS__)
#define SG_LOG_W(msg, ...) SGCore::Logger::getDefaultLogger()->warn<true>(msg, ##__VA_ARGS__)
#define SG_LOG_E(msg, ...) SGCore::Logger::getDefaultLogger()->error<true>(msg, ##__VA_ARGS__)
#define SG_LOG_C(msg, ...) SGCore::Logger::getDefaultLogger()->critical<true>(msg, ##__VA_ARGS__)

#define SG_LOG_NOT_IMPLEMENTED() SG_LOG_E("Do not call this function! It is not implemented!\n{}", SG_CURRENT_LOCATION_STR)
#define SG_LOG_NOT_SUPPORTED_FUNC() SG_LOG_E("Do not call this function! It is not supported!\n{}", SG_CURRENT_LOCATION_STR)

#define SG_LOG_T_UNFORMATTED(msg) SGCore::Logger::getDefaultLogger()->trace<false>(msg)
#define SG_LOG_D_UNFORMATTED(msg) SGCore::Logger::getDefaultLogger()->debug<false>(msg)
#define SG_LOG_I_UNFORMATTED(msg) SGCore::Logger::getDefaultLogger()->info<false>(msg)
#define SG_LOG_W_UNFORMATTED(msg) SGCore::Logger::getDefaultLogger()->warn<false>(msg)
#define SG_LOG_E_UNFORMATTED(msg) SGCore::Logger::getDefaultLogger()->error<false>(msg)
#define SG_LOG_C_UNFORMATTED(msg) SGCore::Logger::getDefaultLogger()->critical<false>(msg)

namespace SGCore
{
    struct SGCORE_EXPORT Logger
    {
    private:
        template<bool UseStaticFormatting, typename... Args>
        using msg_t = std::conditional_t<UseStaticFormatting, fmt::format_string<Args...>, std::string_view>;

    public:
        enum class Level
        {
            LVL_TRACE,
            LVL_DEBUG,
            LVL_INFO,
            LVL_WARN,
            LVL_ERROR,
            LVL_CRITICAL
        };

        static std::string levelToString(Level level) noexcept;

        static Ref<Logger> createLogger(const std::string& loggerName, const std::filesystem::path& filePath) noexcept;
        static Ref<Logger> createLogger(const std::string& loggerName) noexcept;

        template<bool UseStaticFormatting, typename... Args>
        void trace(msg_t<UseStaticFormatting, Args...> msg, Args&&... args) noexcept
        {
            log<UseStaticFormatting>(Level::LVL_TRACE, msg, std::forward<Args>(args)...);
        }

        template<bool UseStaticFormatting, typename... Args>
        void debug(msg_t<UseStaticFormatting, Args...> msg, Args&&... args) noexcept
        {
            log<UseStaticFormatting>(Level::LVL_DEBUG, msg, std::forward<Args>(args)...);
        }

        template<bool UseStaticFormatting, typename... Args>
        void info(msg_t<UseStaticFormatting, Args...> msg, Args&&... args) noexcept
        {
            log<UseStaticFormatting>(Level::LVL_INFO, msg, std::forward<Args>(args)...);
        }

        template<bool UseStaticFormatting, typename... Args>
        void warn(msg_t<UseStaticFormatting, Args...> msg, Args&&... args) noexcept
        {
            log<UseStaticFormatting>(Level::LVL_WARN, msg, std::forward<Args>(args)...);
        }

        template<bool UseStaticFormatting, typename... Args>
        void error(msg_t<UseStaticFormatting, Args...> msg, Args&&... args) noexcept
        {
            log<UseStaticFormatting>(Level::LVL_ERROR, msg, std::forward<Args>(args)...);
        }

        template<bool UseStaticFormatting, typename... Args>
        void critical(msg_t<UseStaticFormatting, Args...> msg, Args&&... args) noexcept
        {
            log<UseStaticFormatting>(Level::LVL_CRITICAL, msg, std::forward<Args>(args)...);
        }

        template<bool UseStaticFormatting, typename... Args>
        void log(Level level, msg_t<UseStaticFormatting, Args...> msg, Args&&... args) noexcept
        {
            const auto levelStr = levelToString(level);

            std::string formattedMsg;
            if constexpr(UseStaticFormatting)
            {
                formattedMsg = fmt::format(msg, std::forward<Args>(args)...);
            }
            else
            {
                formattedMsg = msg;
            }

            const auto now = std::chrono::system_clock::now();

            const auto finalText = fmt::format("[{}] [{}] [{}] {}",
                                                  Utils::getTimeAsString("%Y-%m-%d %H:%M:%S", now),
                                                  m_spdlogLogger->name(),
                                                  levelStr,
                                                  formattedMsg);

            if(!m_spdlogLogger) return;

            switch (level)
            {
                case Level::LVL_TRACE:
                {
                    m_spdlogLogger->trace(formattedMsg);
                    break;
                }
                case Level::LVL_DEBUG:
                {
                    m_spdlogLogger->debug(formattedMsg);
                    break;
                }
                case Level::LVL_INFO:
                {
                    m_spdlogLogger->info(formattedMsg);
                    break;
                }
                case Level::LVL_WARN:
                {
                    m_spdlogLogger->warn(formattedMsg);
                    break;
                }
                case Level::LVL_ERROR:
                {
                    m_spdlogLogger->error(formattedMsg);
                    break;
                }
                case Level::LVL_CRITICAL:
                {
                    m_spdlogLogger->critical(formattedMsg);
                    break;
                }
            }
        }

        std::vector<std::shared_ptr<spdlog::sinks::sink>>& sinks() noexcept;
        const std::vector<std::shared_ptr<spdlog::sinks::sink>>& sinks() const noexcept;

        const std::filesystem::path& getLogFilePath() const noexcept;

        static void setDefaultLogger(const Ref<Logger>& logger) noexcept;
        static Ref<Logger> getDefaultLogger() noexcept;

    private:
        Logger(const std::string& loggerName, const std::filesystem::path& filePath) noexcept;
        Logger(const std::string& loggerName) noexcept;

        static Ref<Logger> m_defaultLogger;

        Ref<spdlog::logger> m_spdlogLogger;

        std::filesystem::path m_logFilePath;
    };
}
