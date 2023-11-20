//
// Created by stuka on 22.04.2023.
//

#pragma once

#ifndef NATIVECORE_LOG_H
#define NATIVECORE_LOG_H

// DEFINES. -----------------------------------------------
// F - FILE
// SL - use source location
// C - CONSOLE

#define SG_GL_SUPPORTING_EXTENSIONS_FILE "SGSupportingGLExtensions.txt"
#define SG_LOG_CURRENT_SESSION_FILE "SGCurrentSession.txt"
#define SG_LOG_GAPI_FILE "SGGAPILog.txt"

#define SGC_ERROR(msg)                                      SGCore::Log::printf(SGCore::MessageType::SG_ERROR, msg)
#define SGC_ERROR_SL(msg, sourceLocation)                   SGCore::Log::printf(SGCore::MessageType::SG_ERROR, msg, SGCore::WriteType::SG_CONSOLE, "", sourceLocation)
#define SGF_ERROR(msg, filePath)                            SGCore::Log::printf(SGCore::MessageType::SG_ERROR, msg, SGCore::WriteType::SG_FILE, filePath)
#define SGF_ERROR_SL(msg, filePath, sourceLocation)         SGCore::Log::printf(SGCore::MessageType::SG_ERROR, msg, SGCore::WriteType::SG_FILE, filePath, sourceLocation)
#define SGCF_ERROR(msg, filePath)                           SGCore::Log::printf(SGCore::MessageType::SG_ERROR, msg, SGCore::WriteType::SG_CONSOLE_FILE, filePath)
#define SGCF_ERROR_SL(msg, filePath, sourceLocation)        SGCore::Log::printf(SGCore::MessageType::SG_ERROR, msg, SGCore::WriteType::SG_CONSOLE_FILE, filePath, sourceLocation)

#define SGC_WARNING(msg)                                    SGCore::Log::printf(SGCore::MessageType::SG_WARNING, msg)
#define SGC_WARNING_SL(msg, sourceLocation)                 SGCore::Log::printf(SGCore::MessageType::SG_WARNING, msg, SGCore::WriteType::SG_CONSOLE, "", sourceLocation)
#define SGF_WARNING(msg, filePath)                          SGCore::Log::printf(SGCore::MessageType::SG_WARNING, msg, SGCore::WriteType::SG_FILE, filePath)
#define SGF_WARNING_SL(msg, filePath, sourceLocation)       SGCore::Log::printf(SGCore::MessageType::SG_WARNING, msg, SGCore::WriteType::SG_FILE, filePath, sourceLocation)
#define SGCF_WARNING(msg, filePath)                         SGCore::Log::printf(SGCore::MessageType::SG_WARNING, msg, SGCore::WriteType::SG_CONSOLE_FILE, filePath)
#define SGCF_WARNING_SL(msg, filePath, sourceLocation)      SGCore::Log::printf(SGCore::MessageType::SG_WARNING, msg, SGCore::WriteType::SG_CONSOLE_FILE, filePath, sourceLocation)

#define SGC_SUCCESS(msg)                                    SGCore::Log::printf(SGCore::MessageType::SG_SUCCESS, msg)
#define SGC_SUCCESS_SL(msg, sourceLocation)                 SGCore::Log::printf(SGCore::MessageType::SG_SUCCESS, msg, SGCore::WriteType::SG_CONSOLE, "", sourceLocation)
#define SGF_SUCCESS(msg, filePath)                          SGCore::Log::printf(SGCore::MessageType::SG_SUCCESS, msg, SGCore::WriteType::SG_FILE, filePath)
#define SGF_SUCCESS_SL(msg, filePath, sourceLocation)       SGCore::Log::printf(SGCore::MessageType::SG_SUCCESS, msg, SGCore::WriteType::SG_FILE, filePath, sourceLocation)
#define SGCF_SUCCESS(msg, filePath)                         SGCore::Log::printf(SGCore::MessageType::SG_SUCCESS, msg, SGCore::WriteType::SG_CONSOLE_FILE, filePath)
#define SGCF_SUCCESS_SL(msg, filePath, sourceLocation)      SGCore::Log::printf(SGCore::MessageType::SG_SUCCESS, msg, SGCore::WriteType::SG_CONSOLE_FILE, filePath, sourceLocation)

#define SGC_INFO(msg)                                       SGCore::Log::printf(SGCore::MessageType::SG_INFO, msg)
#define SGC_INFO_SL(msg, sourceLocation)                    SGCore::Log::printf(SGCore::MessageType::SG_INFO, msg, SGCore::WriteType::SG_CONSOLE, "", sourceLocation)
#define SGF_INFO(msg, filePath)                             SGCore::Log::printf(SGCore::MessageType::SG_INFO, msg, SGCore::WriteType::SG_FILE, filePath)
#define SGF_INFO_SL(msg, filePath, sourceLocation)          SGCore::Log::printf(SGCore::MessageType::SG_INFO, msg, SGCore::WriteType::SG_FILE, filePath, sourceLocation)
#define SGCF_INFO(msg, filePath)                            SGCore::Log::printf(SGCore::MessageType::SG_INFO, msg, SGCore::WriteType::SG_CONSOLE_FILE, filePath)
#define SGCF_INFO_SL(msg, filePath, sourceLocation)         SGCore::Log::printf(SGCore::MessageType::SG_INFO, msg, SGCore::WriteType::SG_CONSOLE_FILE, filePath, sourceLocation)

// ------------------------------------------------------------------------------------

#include <iostream>
#include <source_location>

namespace SGCore
{
    enum MessageType
    {
        SG_ERROR,
        SG_WARNING,
        SG_SUCCESS,
        SG_INFO
    };

    enum WriteType
    {
        SG_CONSOLE,
        SG_FILE,
        SG_CONSOLE_FILE
    };

    class Settings
    {

    };

    struct Log
    {
        static void init();

        static void printf(const MessageType&, const std::string& text,
                    const WriteType& writeType = WriteType::SG_CONSOLE, const std::string_view& filePath = "",
                    std::source_location location = std::source_location::current());

        static std::string messageTypeToString(const MessageType&, const bool& addColor);
    };
}

#endif //NATIVECORE_LOG_H
