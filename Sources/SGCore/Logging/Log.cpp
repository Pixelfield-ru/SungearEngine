#include "Log.h"

#include <chrono>
#include <ctime>
#include <cstdarg>
#include <filesystem>
#include "SGCore/Utils/FileUtils.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void SGCore::Log::init()
{
    SGC_ERROR("Log init: this is error message!");
    SGC_WARNING("Log init: this is warning message!");
    SGC_SUCCESS("Log init: this is success message!");
    SGC_INFO("Log init: this is info message!");

    std::filesystem::remove(SG_GL_SUPPORTING_EXTENSIONS_FILE);
    std::filesystem::remove(SG_LOG_CURRENT_SESSION_FILE);
    std::filesystem::remove(SG_LOG_GAPI_FILE);

    SGF_INFO("Log init", SG_LOG_CURRENT_SESSION_FILE);
}

// TODO: исправить вывод
void SGCore::Log::printf(const MessageType& messageType, const std::string& text,
                           const WriteType& writeType, const std::string_view& filePath, std::source_location location)
{
    std::string msgStr = std::string {text};

    char buf[64];

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", std::localtime(&time));

    std::string errorAdditionalInfo;

    if(messageType == MessageType::SG_ERROR)
    {
        errorAdditionalInfo = std::string(ANSI_COLOR_RED) + "\tFile: " + std::string(location.file_name()) + "\n"
                                                                                                             "\tFunction: " +
                              std::string(location.function_name()) + "\n" + "\tLine: " +
                              std::to_string(location.line()) + "\n" + "\tColumn: " +
                              std::to_string(location.column()) + "\n";
    }

    std::string finalString =
            ANSI_COLOR_RESET + std::string(buf) + " (len: " + std::to_string(msgStr.length()) + ") | [" +
            messageTypeToString(messageType, true) + ANSI_COLOR_RESET + "]: " + text + "\n" + errorAdditionalInfo + ANSI_COLOR_RESET;

    if(writeType == SG_FILE || writeType == SG_CONSOLE_FILE)
    {
        FileUtils::writeToFile(filePath, finalString, true);
    }
    if(writeType == SG_CONSOLE || writeType == SG_CONSOLE_FILE)
    {
        std::cout << finalString;
    }
}

std::string SGCore::Log::messageTypeToString(const MessageType& messageType, const bool& addColor)
{
    std::string ret;
    switch(messageType)
    {
        case MessageType::SG_ERROR: return addColor ? (ret + ANSI_COLOR_RED + "SG_ERROR") : "SG_ERROR";
        case MessageType::SG_WARNING: return addColor ? (ret + ANSI_COLOR_YELLOW + "SG_WARNING") : "SG_WARNING";
        case MessageType::SG_SUCCESS: return addColor ? (ret + ANSI_COLOR_GREEN + "SG_SUCCESS") : "SG_SUCCESS";
        case MessageType::SG_INFO: return addColor ? (ret + ANSI_COLOR_MAGENTA + "SG_INFO") : "SG_INFO";
        default: return "MT_UNKNOWN_TYPE";
    }
}
