//

#ifndef SUNGEARENGINE_LOGGER_H
#define SUNGEARENGINE_LOGGER_H

#include <iostream>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Time.h"
#include "SGCore/Utils/Utils.h"
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "AndroidLogcat.h"

#define LOG_I(tag, msg, ...) SGCore::Logger::getDefaultLogger()->info<true>(tag, msg, ##__VA_ARGS__); LOGCAT_I(tag, msg, ##__VA_ARGS__);
#define LOG_D(tag, msg, ...) SGCore::Logger::getDefaultLogger()->debug<true>(tag, msg, ##__VA_ARGS__); LOGCAT_D(tag, msg, ##__VA_ARGS__);
#define LOG_W(tag, msg, ...) SGCore::Logger::getDefaultLogger()->warn<true>(tag, msg, ##__VA_ARGS__); LOGCAT_W(tag, msg, ##__VA_ARGS__);
#define LOG_E(tag, msg, ...) SGCore::Logger::getDefaultLogger()->error<true>(tag, msg, ##__VA_ARGS__); LOGCAT_E(tag, msg, ##__VA_ARGS__);
#define LOG_C(tag, msg, ...) SGCore::Logger::getDefaultLogger()->critical<true>(tag, msg, ##__VA_ARGS__); LOGCAT_D(tag, msg, ##__VA_ARGS__);

#define LOG_NOT_IMPLEMENTED(tag) LOG_E(tag, "Do not call this function! It is not implemented!\n{}", SG_CURRENT_LOCATION_STR)
#define LOG_NOT_SUPPORTED_FUNC(tag) LOG_E(tag, "Do not call this function! It is not supported!\n{}", SG_CURRENT_LOCATION_STR)

#define LOG_I_UNFORMATTED(tag, msg, ...) SGCore::Logger::getDefaultLogger()->info<false>(tag, msg, ##__VA_ARGS__); LOGCAT_I_UNFORMATTED(tag, msg);
#define LOG_D_UNFORMATTED(tag, msg, ...) SGCore::Logger::getDefaultLogger()->debug<false>(tag, msg, ##__VA_ARGS__); LOGCAT_D_UNFORMATTED(tag, msg);
#define LOG_W_UNFORMATTED(tag, msg, ...) SGCore::Logger::getDefaultLogger()->warn<false>(tag, msg, ##__VA_ARGS__); LOGCAT_W_UNFORMATTED(tag, msg);
#define LOG_E_UNFORMATTED(tag, msg, ...) SGCore::Logger::getDefaultLogger()->error<false>(tag, msg, ##__VA_ARGS__); LOGCAT_E_UNFORMATTED(tag, msg);
#define LOG_C_UNFORMATTED(tag, msg, ...) SGCore::Logger::getDefaultLogger()->critical<false>(tag, msg, ##__VA_ARGS__); LOGCAT_E_UNFORMATTED(tag, msg);

#define SGCORE_TAG "SGCore"
#define PROJECT_BUILD_TAG "Project Build"

namespace SGCore
{
    struct Logger
    {
    private:
        template<bool UseStaticFormatting, typename... Args>
        using msg_t = std::conditional_t<UseStaticFormatting, fmt::format_string<Args...>, std::string_view>;

    public:
        enum class Level
        {
            LVL_INFO,
            LVL_DEBUG,
            LVL_WARN,
            LVL_ERROR,
            LVL_CRITICAL
        };

        static std::string levelToString(Level level) noexcept;

        struct LogMessage
        {
            Level m_level = Level::LVL_INFO;
            std::string m_tag;
            std::string m_message;
        };

        using messages_key = std::pair<Level, std::string>;

        static messages_key make_messages_key(Level lvl, const std::string& tag) noexcept
        {
            return std::make_pair(lvl, tag);
        }

        struct MessageKeyHash
        {
            inline std::size_t operator()(const messages_key& messagesKey) const noexcept
            {
                size_t h = std::hash<Level>()(messagesKey.first) ^ std::hash<std::string>()(messagesKey.second);
                return h;
            }
        };

        static Ref<Logger> createLogger(const std::string& loggerName, const std::filesystem::path& filePath, bool saveMessages = true) noexcept;

        template<bool UseStaticFormatting, typename... Args>
        void info(const std::string& tag, msg_t<UseStaticFormatting, Args...> msg, Args&&... args) noexcept
        {
            log<UseStaticFormatting>(Level::LVL_INFO, tag, msg, std::forward<Args>(args)...);
        }

        template<bool UseStaticFormatting, typename... Args>
        void debug(const std::string& tag, msg_t<UseStaticFormatting, Args...> msg, Args&&... args) noexcept
        {
            log<UseStaticFormatting>(Level::LVL_DEBUG, tag, msg, std::forward<Args>(args)...);
        }

        template<bool UseStaticFormatting, typename... Args>
        void warn(const std::string& tag, msg_t<UseStaticFormatting, Args...> msg, Args&&... args) noexcept
        {
            log<UseStaticFormatting>(Level::LVL_WARN, tag, msg, std::forward<Args>(args)...);
        }

        template<bool UseStaticFormatting, typename... Args>
        void error(const std::string& tag, msg_t<UseStaticFormatting, Args...> msg, Args&&... args) noexcept
        {
            log<UseStaticFormatting>(Level::LVL_ERROR, tag, msg, std::forward<Args>(args)...);
        }

        template<bool UseStaticFormatting, typename... Args>
        void critical(const std::string& tag, msg_t<UseStaticFormatting, Args...> msg, Args&&... args) noexcept
        {
            log<UseStaticFormatting>(Level::LVL_CRITICAL, tag, msg, std::forward<Args>(args)...);
        }

        template<bool UseStaticFormatting, typename... Args>
        void log(Level level, const std::string& tag, msg_t<UseStaticFormatting, Args...> msg, Args&&... args) noexcept
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

            std::lock_guard lock(m_mutex);

            const auto finalMessage = fmt::format("[{}] [logger: {}] [{}] [tag: {}] {}",
                                                  Utils::getTimeAsString("%Y-%m-%d %H:%M:%S"),
                                                  m_name,
                                                  levelStr,
                                                  tag,
                                                  formattedMsg);

            std::cout << finalMessage << std::endl;

            if(!m_spdlogLogger) return;

            switch (level)
            {
                case Level::LVL_INFO:
                {
                    m_spdlogLogger->info(formattedMsg);
                    break;
                }
                case Level::LVL_DEBUG:
                {
                    m_spdlogLogger->debug(formattedMsg);
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

            if (m_saveMessages)
            {
                LogMessage logMessage {
                    .m_level = level,
                    .m_tag = tag,
                    .m_message = finalMessage
                };

                m_sortedMessages[make_messages_key(level, tag)].push_back(logMessage);
                m_allMessages.push_back(logMessage);
            }
        }

        [[nodiscard]] std::vector<std::string> getAllTags() noexcept;

        [[nodiscard]] std::vector<LogMessage> getAllMessages() noexcept;
        [[nodiscard]] std::vector<LogMessage> getMessagesWithLevel(Level lvl) noexcept;
        [[nodiscard]] std::vector<LogMessage> getMessagesWithTag(const std::string& tag) noexcept;
        [[nodiscard]] std::vector<LogMessage> getMessagesWithLevelAndTag(Level lvl, const std::string& tag) noexcept;

        void clearAllMessages() noexcept;
        void clearMessagesWithLevel(Level lvl) noexcept;
        void clearMessagesWithTag(const std::string& tag) noexcept;
        void clearMessagesWithLevelAndTag(Level lvl, const std::string& tag) noexcept;

        const std::filesystem::path& getLogFilePath() const noexcept;

        static void setDefaultLogger(const Ref<Logger>& logger) noexcept;
        static Ref<Logger> getDefaultLogger() noexcept;

    private:
        Logger(const std::string& loggerName, const std::filesystem::path& filePath, bool saveMessages = true) noexcept;

        static inline Ref<Logger> m_defaultLogger;
        Ref<spdlog::logger> m_spdlogLogger;

        std::mutex m_mutex;

        bool m_saveMessages = true;
        std::string m_name;
        std::filesystem::path m_logFilePath;

        std::unordered_map<messages_key, std::vector<LogMessage>, MessageKeyHash> m_sortedMessages;
        std::vector<LogMessage> m_allMessages;
    };
}

#endif //SUNGEARENGINE_LOGGER_H
