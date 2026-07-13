//
// Created by stuka on 13.07.2026.
//

#pragma once

#include <functional>
#include <cstdint>

#include "Packet.h"

namespace SGCore::Net
{
    struct DataType
    {
        std::function<void(const Packet& data)> onReceive;
        std::uint64_t m_dataSize = 0;
    };
}