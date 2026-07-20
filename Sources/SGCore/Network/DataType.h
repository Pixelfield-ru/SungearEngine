//
// Created by stuka on 13.07.2026.
//

#pragma once

#include <functional>
#include <cstdint>
#include <boost/asio/ip/udp.hpp>

#include "Packet.h"
#include "Types.h"

namespace SGCore::Net
{
    template<typename ProtocolT>
    struct IStream;

    struct DataType
    {
        template<typename ProtocolT>
        friend struct IStream;

        /**
         * @brief Used to handle packet receipt.
         * @note You must send GotReliablePacketMessage message before sending other RUDP messages if you are handling custom auth message.
         */
        std::function<void(const Packet& data, boost::asio::ip::udp::endpoint senderEndpoint, session_id_t senderSessionID)> onReceive;
        std::function<void(const Packet& data, boost::asio::ip::udp::endpoint targetEndpoint, session_id_t targetSessionID)> onSendFailed;
        bool m_authRequired = true;

        std::uint64_t getDataSize() const noexcept
        {
            return m_dataSize;
        }

        bool isUsingRUDP() const noexcept
        {
            return m_useRUDP;
        }

        bool isUsedForAuth() const noexcept
        {
            return m_isUsedForAuth;
        }

    private:
        std::uint64_t m_dataSize = 0;
        bool m_useRUDP = false;
        // is this type of message is using for auth
        bool m_isUsedForAuth = false;
    };
}