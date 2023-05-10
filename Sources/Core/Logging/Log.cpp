#include "Log.h"

#include <chrono>
#include <ctime>
#include <cstdarg>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void Core::Logging::init()
{
    consolePrintf(MessageType::SG_ERROR, "Log init: this is error message!");
    consolePrintf(MessageType::SG_WARNING, "Log init: this is warning message!");
    consolePrintf(MessageType::SG_SUCCESS, "Log init: this is success message!");
    consolePrintf(MessageType::SG_INFO, "Log init: this is info message!");
}

// TODO: исправить вывод
void Core::Logging::consolePrintf(const MessageType& messageType, const char* text, ...)
{
    std::string msgStr = std::string {text };

    char buf[64];

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", std::localtime(&time));

    std::string errorAdditionalInfo;

    std::source_location location = std::source_location::current();

    if(messageType == MessageType::SG_ERROR)
    {
        errorAdditionalInfo = std::string(ANSI_COLOR_RED) + "\tFile: " + std::string(location.file_name()) + "\n"
                              "\tFunction: " + std::string(location.function_name()) + "\n" +
                              "\tLine: " + std::to_string(location.line()) + "\n" +
                              "\tColumn: " + std::to_string(location.column()) + "\n";
    }

    std::string finalString = ANSI_COLOR_RESET + std::string(buf) + " (len: " + std::to_string(msgStr.length()) + ") | [" +
            messageTypeToString(messageType, true) + ANSI_COLOR_RESET + "]: " + text + "\n" + errorAdditionalInfo;

    va_list argsPtr;
    va_start(argsPtr, text);
    vprintf(finalString.c_str(), argsPtr);
    va_end(argsPtr);
}

std::string Core::Logging::messageTypeToString(const MessageType& messageType, const bool& addColor)
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
