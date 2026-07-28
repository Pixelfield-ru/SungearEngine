//
// Created by Ilya on 21.08.2024.
//

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

#include "SGCore/Utils/Utils.h"

#if SG_PLATFORM_OS_ANDROID
#include <spdlog/sinks/android_sink.h>
#endif

#include "Logger.h"

#include "SGCore/Utils/FileUtils.h"

SGCore::Ref<SGCore::Logger> SGCore::Logger::m_defaultLogger {};

std::string SGCore::Logger::levelToString(Level level) noexcept
{
    switch (level)
    {
        case Level::LVL_TRACE:
            return "trace";
        case Level::LVL_DEBUG:
            return "debug";
        case Level::LVL_INFO:
            return "info";
        case Level::LVL_WARN:
            return "warn";
        case Level::LVL_ERROR:
            return "error";
        case Level::LVL_CRITICAL:
            return "critical";
    }

    return "";
}

SGCore::Ref<SGCore::Logger>
SGCore::Logger::createLogger(const std::string& loggerName, const std::filesystem::path& filePath) noexcept
{
    auto logger = Ref<Logger>(new Logger(loggerName, filePath));

    return logger;
}

SGCore::Ref<SGCore::Logger> SGCore::Logger::createLogger(const std::string& loggerName) noexcept
{
    auto logger = Ref<Logger>(new Logger(loggerName));

    return logger;
}

SGCore::Logger::Logger(const std::string& loggerName, const std::filesystem::path& filePath) noexcept
{
    m_logFilePath = filePath;

    const std::string u8Path = Utils::toUTF8(filePath);

    m_spdlogLogger = spdlog::basic_logger_mt(loggerName, u8Path);

    auto stdoutSink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
    m_spdlogLogger->sinks().push_back(stdoutSink);

#if SG_PLATFORM_OS_ANDROID
    auto androidSink = std::make_shared<spdlog::sinks::android_sink>();
    m_spdlogLogger->sinks().push_back(androidSink);
#endif

    m_spdlogLogger->flush_on(spdlog::level::info);

    spdlog::register_logger(m_spdlogLogger);
}

SGCore::Logger::Logger(const std::string& loggerName) noexcept
{
    m_spdlogLogger = std::make_shared<spdlog::logger>(loggerName);

    auto stdoutSink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
    m_spdlogLogger->sinks().push_back(stdoutSink);

#if SG_PLATFORM_OS_ANDROID
    auto androidSink = std::make_shared<spdlog::sinks::android_sink>();
    m_spdlogLogger->sinks().push_back(androidSink);
#endif

    m_spdlogLogger->flush_on(spdlog::level::info);

    spdlog::register_logger(m_spdlogLogger);
}

void SGCore::Logger::setDefaultLogger(const Ref<Logger>& logger) noexcept
{
    m_defaultLogger = logger;
}

SGCore::Ref<SGCore::Logger> SGCore::Logger::getDefaultLogger() noexcept
{
    if(!m_defaultLogger)
    {
        const std::string finalLogFileName = Utils::toUTF8(FileUtils::getAppPublicResourcesPath()) + "/logs/sg_log_" +
                                             Utils::getTimeAsString("%Y_%m_%d_%H_%M_%S") + ".log";

        setDefaultLogger(createLogger("SGCore", finalLogFileName));
    }

    return m_defaultLogger;
}

std::vector<std::shared_ptr<spdlog::sinks::sink>>& SGCore::Logger::sinks() noexcept
{
    return m_spdlogLogger->sinks();
}

const std::vector<std::shared_ptr<spdlog::sinks::sink>>& SGCore::Logger::sinks() const noexcept
{
    return m_spdlogLogger->sinks();
}

const std::filesystem::path& SGCore::Logger::getLogFilePath() const noexcept
{
    return m_logFilePath;
}
