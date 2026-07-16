//
// Created by stuka on 13.07.2026.
//

#pragma once

#include <functional>
#include <cstdint>
#include <boost/asio/ip/udp.hpp>

#include "Packet.h"

namespace SGCore::Net
{
    struct DataType
    {
        std::function<void(const Packet& data, boost::asio::ip::udp::endpoint senderEndpoint, std::uint64_t senderSessionID)> onReceive;
        std::uint64_t m_dataSize = 0;
        bool m_authRequired = true;
    };
}