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
        static auto readEndpoint(const Packet& fromPacket, size_t inPacketOffset, size_t& endpointSize, bool& isSuccessful) noexcept
        {
            // number '1' is size of ip type in bytes. number '2' is size of port in bytes

            boost::asio::ip::udp::endpoint endpoint;

            if(inPacketOffset >= fromPacket.size())
            {
                isSuccessful = false;
                return endpoint;
            }

            const auto ipType = fromPacket[inPacketOffset];

            size_t endpointOutputSize = 0;

            isSuccessful = true;

            if(ipType == 4)
            {
                boost::asio::ip::address_v4::bytes_type addressBytes;

                constexpr size_t addressBytesCount = decltype(addressBytes){}.size();

                if(inPacketOffset + 1 + addressBytes.size() + 2 > fromPacket.size())
                {
                    isSuccessful = false;
                    return endpoint;
                }

                std::copy_n(fromPacket.data() + inPacketOffset + 1, addressBytesCount, addressBytes.begin());
                const std::uint16_t port = (static_cast<std::uint16_t>(fromPacket[inPacketOffset + 1 + addressBytesCount]) << 8) | fromPacket[inPacketOffset + 1 + addressBytesCount + 1];
                endpoint = { boost::asio::ip::address_v4(addressBytes), port };

                endpointOutputSize += 1 + addressBytesCount + sizeof(port);
            }
            else if(ipType == 6)
            {
                boost::asio::ip::address_v6::bytes_type addressBytes;

                constexpr size_t addressBytesCount = decltype(addressBytes){}.size();

                if(inPacketOffset + 1 + addressBytes.size() + 2 > fromPacket.size())
                {
                    isSuccessful = false;
                    return endpoint;
                }

                std::copy_n(fromPacket.data() + inPacketOffset + 1, addressBytesCount, addressBytes.begin());
                const std::uint16_t port = (static_cast<std::uint16_t>(fromPacket[inPacketOffset + 1 + addressBytesCount]) << 8) | fromPacket[inPacketOffset + 1 + addressBytesCount + 1];
                endpoint = { boost::asio::ip::address_v6(addressBytes), port };

                endpointOutputSize += 1 + addressBytesCount + sizeof(port);
            }

            endpointSize = endpointOutputSize;

            return endpoint;
        }

        static bool writeEndpoint(Packet& toPacket, size_t inPacketOffset, boost::asio::ip::udp::endpoint endpoint, size_t& endpointSize) noexcept
        {
            // number '1' is size of ip type in bytes. number '2' is size of port in bytes

            // 1 + 4 + 2 = 7
            // buf[0] = type;
            // buf[1] = ipv4_0;
            // buf[2] = ipv4_1;
            // buf[3] = ipv4_2;
            // buf[4] = ipv4_3;
            // buf[5] = port_0;
            // buf[6] = port_1;

            if(inPacketOffset >= toPacket.size()) return false;

            size_t outputEndpointSize = 0;

            if(endpoint.address().is_v4())
            {
                toPacket[inPacketOffset] = 4;

                const auto addressBytes = endpoint.address().to_v4().to_bytes();

                if(inPacketOffset + 1 + addressBytes.size() + 2 > toPacket.size()) return false;

                std::memcpy(toPacket.data() + 1 + inPacketOffset, addressBytes.data(), addressBytes.size());

                outputEndpointSize = 1 + addressBytes.size();
            }
            else if(endpoint.address().is_v6())
            {
                toPacket[inPacketOffset] = 6;

                const auto addressBytes = endpoint.address().to_v6().to_bytes();

                if(inPacketOffset + 1 + addressBytes.size() + 2 > toPacket.size()) return false;

                std::memcpy(toPacket.data() + 1 + inPacketOffset, addressBytes.data(), addressBytes.size());

                outputEndpointSize = 1 + addressBytes.size();
            }
            else return false;

            inPacketOffset += outputEndpointSize;

            const std::uint16_t port = endpoint.port();

            toPacket[inPacketOffset] = static_cast<std::uint8_t>((port >> 8) & 0xFF);
            toPacket[inPacketOffset + 1] = static_cast<std::uint8_t>(port & 0xFF);

            outputEndpointSize += sizeof(port);

            endpointSize = outputEndpointSize;

            return true;
        }
    };
}

#endif // SUNGEARENGINE_NETWORK_UTILS_H
