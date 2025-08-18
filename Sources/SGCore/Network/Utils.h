//
// Created by stuka on 14.08.2025.
//

#ifndef SUNGEARENGINE_NETWORK_UTILS_H
#define SUNGEARENGINE_NETWORK_UTILS_H

#include <boost/asio.hpp>
#include <boost/container_hash/hash_fwd.hpp>

#include "Packet.h"

namespace SGCore::Net
{
    struct Utils
    {
        static auto readEndpoint(const Packet& fromPacket, size_t inPacketOffset, size_t& endpointSize) noexcept
        {
            boost::asio::ip::udp::endpoint endpoint;

            const auto ipType = fromPacket[inPacketOffset];

            size_t endpointOutputSize = 0;

            if(ipType == 4)
            {
                boost::asio::ip::address_v4::bytes_type addressBytes;

                constexpr size_t addressBytesCount = decltype(addressBytes){}.size();

                std::copy_n(fromPacket.data() + inPacketOffset + 1, addressBytesCount, addressBytes.begin());
                const std::uint16_t port = (static_cast<std::uint16_t>(fromPacket[inPacketOffset + 1 + addressBytesCount]) << 8) | fromPacket[inPacketOffset + 1 + addressBytesCount + 1];
                endpoint = { boost::asio::ip::address_v4(addressBytes), port };

                endpointOutputSize += 1 + addressBytesCount + sizeof(port);
            }
            else if(ipType == 6)
            {
                boost::asio::ip::address_v6::bytes_type addressBytes;

                constexpr size_t addressBytesCount = decltype(addressBytes){}.size();

                std::copy_n(fromPacket.data() + inPacketOffset + 1, addressBytesCount, addressBytes.begin());
                const std::uint16_t port = (static_cast<std::uint16_t>(fromPacket[inPacketOffset + 1 + addressBytesCount]) << 8) | fromPacket[inPacketOffset + 1 + addressBytesCount + 1];
                endpoint = { boost::asio::ip::address_v6(addressBytes), port };

                endpointOutputSize += 1 + addressBytesCount + sizeof(port);
            }

            endpointSize = endpointOutputSize;

            return endpoint;
        }

        static void writeEndpoint(Packet& toPacket, size_t inPacketOffset, boost::asio::ip::udp::endpoint endpoint, size_t& endpointSize) noexcept
        {
            size_t outputEndpointSize = 0;

            if(endpoint.address().is_v4())
            {
                toPacket[inPacketOffset] = 4;

                const auto addressBytes = endpoint.address().to_v4().to_bytes();
                std::memcpy(toPacket.data() + 1 + inPacketOffset, addressBytes.data(), addressBytes.size());

                outputEndpointSize = addressBytes.size() + 1;
            }
            else if(endpoint.address().is_v6())
            {
                toPacket[inPacketOffset] = 6;

                const auto addressBytes = endpoint.address().to_v6().to_bytes();
                std::memcpy(toPacket.data() + 1 + inPacketOffset, addressBytes.data(), addressBytes.size());

                outputEndpointSize = addressBytes.size() + 1;
            }

            inPacketOffset += outputEndpointSize;

            const std::uint16_t port = endpoint.port();

            toPacket[inPacketOffset] = static_cast<std::uint8_t>((port >> 8) & 0xFF);
            toPacket[inPacketOffset + 1] = static_cast<std::uint8_t>(port & 0xFF);

            outputEndpointSize += sizeof(port);

            endpointSize = outputEndpointSize;
        }
    };
}

#endif // SUNGEARENGINE_NETWORK_UTILS_H
