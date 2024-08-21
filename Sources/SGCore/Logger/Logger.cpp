//
// Created by Ilya on 21.08.2024.
//

#include <spdlog/sinks/basic_file_sink.h>
#include <SGCore/Utils/Utils.h>
#include "Logger.h"

SGCore::Ref<SGCore::Logger>
SGCore::Logger::createLogger(const std::string& loggerName, const std::filesystem::path& filePath,
                             bool saveMessages) noexcept
{
    auto logger = Ref<Logger>(new Logger(loggerName, filePath, saveMessages));

    return logger;
}

SGCore::Logger::Logger(const std::string& loggerName, const std::filesystem::path& filePath, bool saveMessages) noexcept
{
    const std::string u8Path = Utils::toUTF8(filePath.u16string());
    m_spdlogLogger = spdlog::basic_logger_mt(loggerName, u8Path);
    spdlog::flush_on(spdlog::level::info);

    m_saveMessages = saveMessages;
    m_name = loggerName;
}

void SGCore::Logger::setDefaultLogger(const SGCore::Ref<SGCore::Logger>& logger) noexcept
{
    m_defaultLogger = logger;
}

SGCore::Ref<SGCore::Logger> SGCore::Logger::getDefaultLogger() noexcept
{
    return m_defaultLogger;
}

const std::vector<std::string>& SGCore::Logger::getInfoMessages() const noexcept
{
    return m_infoMessages;
}

const std::vector<std::string>& SGCore::Logger::getDebugMessages() const noexcept
{
    return m_debugMessages;
}

const std::vector<std::string>& SGCore::Logger::getWarnMessages() const noexcept
{
    return m_warnMessages;
}

const std::vector<std::string>& SGCore::Logger::getErrorMessages() const noexcept
{
    return m_errorMessages;
}

const std::vector<std::string>& SGCore::Logger::getCriticalMessages() const noexcept
{
    return m_criticalMessages;
}

const std::vector<SGCore::LogMessage>& SGCore::Logger::getAllMessages() const noexcept
{
    return m_allMessages;
}
