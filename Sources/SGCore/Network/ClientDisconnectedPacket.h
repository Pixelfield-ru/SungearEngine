//
// Created by stuka on 19.08.2025.
//

#pragma once

#include <string>

namespace SGCore::Net
{
    struct SGCORE_EXPORT ClientDisconnectedPacket
    {
        static inline std::string type_name = "SGCore::Net::ClientDisconnectedPacket";
    };
}