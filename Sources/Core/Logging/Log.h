//
// Created by stuka on 22.04.2023.
//

#pragma once

#ifndef NATIVECORE_LOG_H
#define NATIVECORE_LOG_H

#include <iostream>

//#date "Log.cpp"

namespace Core::Logging
{
    enum MessageType
    {
        SG_ERROR,
        SG_WARNING,
        SG_SUCCESS,
        SG_INFO
    };

    class Settings
    {

    };

    void init();

    void consolePrintf(const MessageType&, const char* text, ...);

    std::string messageTypeToString(const MessageType&, const bool& addColor);
}


#endif //NATIVECORE_LOG_H
