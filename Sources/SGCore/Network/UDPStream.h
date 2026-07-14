//
// Created by stuka on 13.07.2026.
//

#pragma once

#include <boost/asio/ip/udp.hpp>

#include "IStream.h"

namespace SGCore::Net
{
    // здесь вроде всё
    struct SGCORE_EXPORT UDPStream : IStream<boost::asio::ip::udp>
    {
        static constexpr char footer[4] { 'S', 'G', 'P', 'K' };

        template<typename MsgT>
        void sendMessage(strand_t& strand, const MsgT& message, std::int64_t currentSessionID, std::int64_t targetSessionID) noexcept
        {
            static const auto messageTypeID = MsgT::getTypeIDStatic();

            if(!m_registeredDataTypes.contains(messageTypeID))
            {
                return;
            }

            const auto targetClientIt = m_registeredClients.find(targetSessionID);
            if(targetClientIt == m_registeredClients.end())
            {
                // unknown session
                return;
            }

            const auto& targetClientEndpoint = targetClientIt->second;

            Packet packet {};
            // writing message type id
            std::memcpy(packet.data(), &messageTypeID, sizeof(messageTypeID));
            // writing client session id
            std::memcpy(packet.data() + sizeof(messageTypeID), &currentSessionID, sizeof(currentSessionID));
            // writing message data
            std::memcpy(packet.data() + sizeof(messageTypeID) + sizeof(currentSessionID), &message, sizeof(MsgT));
            // writing footer
            std::memcpy(packet.data() + sizeof(messageTypeID) + sizeof(currentSessionID) + sizeof(MsgT), footer, sizeof(footer));

            boost::asio::post(strand, [this, packet, targetClientEndpoint] {
                m_socket->async_send_to(boost::asio::buffer(packet), targetClientEndpoint, [targetClientEndpoint](boost::system::error_code errorCode, size_t bufferSize) {
                    if(errorCode)
                    {
                        LOG_E(SGCORE_TAG, "Cannot send packet with size {} to client {}", bufferSize, targetClientEndpoint.address().to_string());
                    }
                });
            });
        }

        void receive(strand_t& strand) noexcept;
    };
}
