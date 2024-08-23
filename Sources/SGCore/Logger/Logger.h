//

#ifndef SUNGEARENGINE_LOGGER_H
#define SUNGEARENGINE_LOGGER_H

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Utils.h"

#define LOG_I(msg, ...) SGCore::Logger::getDefaultLogger()->info(msg, ##__VA_ARGS__);
#define LOG_D(msg, ...) SGCore::Logger::getDefaultLogger()->debug(msg, ##__VA_ARGS__);
#define LOG_W(msg, ...) SGCore::Logger::getDefaultLogger()->warn(msg, ##__VA_ARGS__);
#define LOG_E(msg, ...) SGCore::Logger::getDefaultLogger()->error(msg, ##__VA_ARGS__);
#define LOG_C(msg, ...) SGCore::Logger::getDefaultLogger()->critical(msg, ##__VA_ARGS__);

namespace SGCore
{
    enum class LogMessageType
    {
        T_INFO,
        T_DEBUG,
        T_WARN,
        T_ERROR,
        T_CRITICAL
    };

    struct LogMessage
    {
        LogMessageType m_type = LogMessageType::T_INFO;
        std::string m_message;
    };

    struct Logger
    {
        static Ref<Logger> createLogger(const std::string& loggerName, const std::filesystem::path& filePath, bool saveMessages = true) noexcept;

        template<typename... Args>
        void info(std::string_view msg, Args&&... args) noexcept
        {
            // std::cout << "msg before fmt: " << msg << std::endl;
            std::string formattedMsg = fmt::vformat(msg, fmt::make_format_args(args...));
            m_spdlogLogger->info(formattedMsg);

            if(m_saveMessages)
            {
                LogMessage logMessage {
                    .m_type = LogMessageType::T_INFO,
                    .m_message = fmt::format("[{}] [logger: {}] [info] {}", Utils::getTimeAsString("%Y-%m-%d %H:%M:%S"), m_name,
                                             formattedMsg)
                };

                m_infoMessages.push_back(logMessage.m_message);
                m_allMessages.push_back(logMessage);
            }
        }

        template<typename... Args>
        void debug(std::string_view msg, Args&&... args) noexcept
        {
            std::string formattedMsg = fmt::vformat(msg, fmt::make_format_args(args...));
            m_spdlogLogger->debug(formattedMsg);

            if(m_saveMessages)
            {
                LogMessage logMessage {
                        .m_type = LogMessageType::T_DEBUG,
                        .m_message = fmt::format("[{}] [logger: {}] [debug] {}", Utils::getTimeAsString("%Y-%m-%d %H:%M:%S"), m_name,
                                                 formattedMsg)
                };

                m_debugMessages.push_back(logMessage.m_message);
                m_allMessages.push_back(logMessage);
            }
        }

        template<typename... Args>
        void warn(std::string_view msg, Args&&... args) noexcept
        {
            std::string formattedMsg = fmt::vformat(msg, fmt::make_format_args(args...));
            m_spdlogLogger->warn(formattedMsg);

            if (m_saveMessages)
            {
                LogMessage logMessage {
                        .m_type = LogMessageType::T_WARN,
                        .m_message = fmt::format("[{}] [logger: {}] [warn] {}", Utils::getTimeAsString("%Y-%m-%d %H:%M:%S"), m_name,
                                                 formattedMsg)
                };

                m_warnMessages.push_back(logMessage.m_message);
                m_allMessages.push_back(logMessage);
            }
        }

        template<typename... Args>
        void error(std::string_view msg, Args&&... args) noexcept
        {
            std::string formattedMsg = fmt::vformat(msg, fmt::make_format_args(args...));
            m_spdlogLogger->error(formattedMsg);

            if (m_saveMessages)
            {
                LogMessage logMessage {
                        .m_type = LogMessageType::T_ERROR,
                        .m_message = fmt::format("[{}] [logger: {}] [error] {}", Utils::getTimeAsString("%Y-%m-%d %H:%M:%S"), m_name,
                                                 formattedMsg)
                };

                m_errorMessages.push_back(logMessage.m_message);
                m_allMessages.push_back(logMessage);
            }
        }

        template<typename... Args>
        void critical(std::string_view msg, Args&&... args) noexcept
        {
            std::string formattedMsg = fmt::vformat(msg, fmt::make_format_args(args...));
            m_spdlogLogger->critical(formattedMsg);

            if (m_saveMessages)
            {
                LogMessage logMessage {
                        .m_type = LogMessageType::T_CRITICAL,
                        .m_message = fmt::format("[{}] [logger: {}] [critical] {}", Utils::getTimeAsString("%Y-%m-%d %H:%M:%S"), m_name,
                                                 formattedMsg)
                };

                m_criticalMessages.push_back(logMessage.m_message);
                m_allMessages.push_back(logMessage);
            }
        }

        [[nodiscard]] const std::vector<std::string>& getInfoMessages() const noexcept;
        [[nodiscard]] const std::vector<std::string>& getDebugMessages() const noexcept;
        [[nodiscard]] const std::vector<std::string>& getWarnMessages() const noexcept;
        [[nodiscard]] const std::vector<std::string>& getErrorMessages() const noexcept;
        [[nodiscard]] const std::vector<std::string>& getCriticalMessages() const noexcept;
        [[nodiscard]] const std::vector<LogMessage>& getAllMessages() const noexcept;

        static void setDefaultLogger(const Ref<Logger>& logger) noexcept;
        static Ref<Logger> getDefaultLogger() noexcept;

    private:
        Logger(const std::string& loggerName, const std::filesystem::path& filePath, bool saveMessages = true) noexcept;

        static inline Ref<Logger> m_defaultLogger;
        Ref<spdlog::logger> m_spdlogLogger;

        bool m_saveMessages = true;
        std::string m_name;

        std::vector<std::string> m_infoMessages;
        std::vector<std::string> m_debugMessages;
        std::vector<std::string> m_warnMessages;
        std::vector<std::string> m_errorMessages;
        std::vector<std::string> m_criticalMessages;

        std::vector<LogMessage> m_allMessages;
    };
}

#endif //SUNGEARENGINE_LOGGER_H
