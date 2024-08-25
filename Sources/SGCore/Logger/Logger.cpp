//
// Created by Ilya on 21.08.2024.
//

#include <spdlog/sinks/basic_file_sink.h>
#include <SGCore/Utils/Utils.h>
#include "Logger.h"

std::string SGCore::Logger::levelToString(SGCore::Logger::Level level) noexcept
{
    switch (level)
    {
        case Level::LVL_INFO:
            return "info";
        case Level::LVL_DEBUG:
            return "debug";
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

std::vector<std::string> SGCore::Logger::getAllTags() noexcept
{
    std::lock_guard lock(m_mutex);

    std::vector<std::string> tags;

    for(const auto& [key, msg] : m_sortedMessages)
    {
        if(std::find_if(tags.begin(), tags.end(), [&key](const std::string& tag) {
            return tag == key.second;
        }) == tags.end())
        {
            tags.push_back(key.second);
        }
    }

    return tags;
}

std::vector<SGCore::Logger::LogMessage> SGCore::Logger::getAllMessages() noexcept
{
    std::lock_guard lock(m_mutex);
    return m_allMessages;
}

std::vector<SGCore::Logger::LogMessage> SGCore::Logger::getMessagesWithLevel(SGCore::Logger::Level lvl) noexcept
{
    std::lock_guard lock(m_mutex);

    std::vector<LogMessage> messages;

    for(const auto& [key, msg] : m_sortedMessages)
    {
        if(key.first == lvl)
        {
            messages.insert(messages.cend(), msg.begin(), msg.end());
        }
    }

    return messages;
}

std::vector<SGCore::Logger::LogMessage> SGCore::Logger::getMessagesWithTag(const std::string& tag) noexcept
{
    std::lock_guard lock(m_mutex);

    std::vector<LogMessage> messages;

    for(const auto& [key, msg] : m_sortedMessages)
    {
        if(key.second == tag)
        {
            messages.insert(messages.cend(), msg.begin(), msg.end());
        }
    }

    return messages;
}

std::vector<SGCore::Logger::LogMessage>
SGCore::Logger::getMessagesWithLevelAndTag(SGCore::Logger::Level lvl, const std::string& tag) noexcept
{
    std::lock_guard lock(m_mutex);

    auto it = m_sortedMessages.find(make_messages_key(lvl, tag));
    if(it != m_sortedMessages.end())
    {
        return it->second;
    }

    return { };
}

void SGCore::Logger::clearAllMessages() noexcept
{
    m_allMessages.clear();
    m_sortedMessages.clear();
}

void SGCore::Logger::clearMessagesWithLevel(SGCore::Logger::Level lvl) noexcept
{
    std::erase_if(m_allMessages, [&lvl](const LogMessage& logMessage) {
        return logMessage.m_level == lvl;
    });

    std::erase_if(m_sortedMessages, [&lvl](const std::pair<messages_key, std::vector<LogMessage>>& p) {
        return p.first.first == lvl;
    });
}

void SGCore::Logger::clearMessagesWithTag(const std::string& tag) noexcept
{
    std::erase_if(m_allMessages, [&tag](const LogMessage& logMessage) {
        return logMessage.m_tag == tag;
    });

    std::erase_if(m_sortedMessages, [&tag](const std::pair<messages_key, std::vector<LogMessage>>& p) {
        return p.first.second == tag;
    });
}

void SGCore::Logger::clearMessagesWithLevelAndTag(SGCore::Logger::Level lvl, const std::string& tag) noexcept
{
    std::erase_if(m_allMessages, [&lvl, &tag](const LogMessage& logMessage) {
        return logMessage.m_level == lvl && logMessage.m_tag == tag;
    });

    std::erase_if(m_sortedMessages, [&lvl, &tag](const std::pair<messages_key, std::vector<LogMessage>>& p) {
        return p.first.first == lvl && p.first.second == tag;
    });
}
