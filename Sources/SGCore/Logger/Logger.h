//

#ifndef SUNGEARENGINE_LOGGER_H
#define SUNGEARENGINE_LOGGER_H

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Utils/TypeTraits.h"

#define LOG_I(tag, msg, ...) SGCore::Logger::getDefaultLogger()->info(tag, msg, ##__VA_ARGS__);
#define LOG_D(tag, msg, ...) SGCore::Logger::getDefaultLogger()->debug(tag, msg, ##__VA_ARGS__);
#define LOG_W(tag, msg, ...) SGCore::Logger::getDefaultLogger()->warn(tag, msg, ##__VA_ARGS__);
#define LOG_E(tag, msg, ...) SGCore::Logger::getDefaultLogger()->error(tag, msg, ##__VA_ARGS__);
#define LOG_C(tag, msg, ...) SGCore::Logger::getDefaultLogger()->critical(tag, msg, ##__VA_ARGS__);

#define SGCORE_TAG "SGCore"

namespace SGCore
{
    struct Logger
    {
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

        template<typename... Args>
        void info(const std::string& tag, std::string_view msg, Args&&... args) noexcept
        {
            log(Level::LVL_INFO, tag, msg, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void debug(const std::string& tag, std::string_view msg, Args&&... args) noexcept
        {
            log(Level::LVL_DEBUG, tag, msg, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void warn(const std::string& tag, std::string_view msg, Args&&... args) noexcept
        {
            log(Level::LVL_WARN, tag, msg, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void error(const std::string& tag, std::string_view msg, Args&&... args) noexcept
        {
            log(Level::LVL_ERROR, tag, msg, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void critical(const std::string& tag, std::string_view msg, Args&&... args) noexcept
        {
            log(Level::LVL_CRITICAL, tag, msg, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void log(Level level, const std::string& tag, std::string_view msg, Args&&... args) noexcept
        {
            std::string formattedMsg = fmt::vformat(msg, fmt::make_format_args(args...));
            std::string levelStr = levelToString(level);
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
                    .m_message = fmt::format("[{}] [logger: {}] [{}] [tag: {}] {}",
                                             Utils::getTimeAsString("%Y-%m-%d %H:%M:%S"),
                                             m_name,
                                             levelStr,
                                             tag,
                                             formattedMsg)
                };

                m_sortedMessages[make_messages_key(level, tag)].push_back(logMessage);
                m_allMessages.push_back(logMessage);
            }
        }

        [[nodiscard]] const std::vector<LogMessage>& getAllMessages() const noexcept;
        [[nodiscard]] std::vector<LogMessage> getMessagesWithLevel(Level lvl) const noexcept;
        [[nodiscard]] std::vector<LogMessage> getMessagesWithTag(const std::string& tag) const noexcept;
        [[nodiscard]] std::vector<LogMessage> getMessagesWithLevelAndTag(Level lvl, const std::string& tag) const noexcept;

        static void setDefaultLogger(const Ref<Logger>& logger) noexcept;
        static Ref<Logger> getDefaultLogger() noexcept;

    private:
        Logger(const std::string& loggerName, const std::filesystem::path& filePath, bool saveMessages = true) noexcept;

        static inline Ref<Logger> m_defaultLogger;
        Ref<spdlog::logger> m_spdlogLogger;

        bool m_saveMessages = true;
        std::string m_name;

        std::unordered_map<messages_key, std::vector<LogMessage>, MessageKeyHash> m_sortedMessages;
        std::vector<LogMessage> m_allMessages;
    };
}

#endif //SUNGEARENGINE_LOGGER_H
