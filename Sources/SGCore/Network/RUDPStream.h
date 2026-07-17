//
// Created by stuka on 13.07.2026.
//

#pragma once

#include <boost/asio/ip/udp.hpp>
#include <queue>
#include <map>

#include "IStream.h"

namespace SGCore::Net
{
    // todo: fix rudp
    struct SGCORE_EXPORT RUDPStream : IStream<boost::asio::ip::udp>
    {
        static constexpr char footer[4] { 'S', 'G', 'P', 'K' };

        struct ReliablePacket
        {
            Ref<Packet> m_packet;
            std::chrono::steady_clock::time_point m_sendTime = std::chrono::steady_clock::now();
            std::uint64_t m_packetTypeID {};
            session_id_t m_targetSessionID {};
        };

        struct ReliableStream
        {
            RUDPStream* m_udpStream {};

            session_id_t m_targetSessionID {};
            std::queue<ReliablePacket> m_reliablePackets;

            void pollPackets() noexcept;
        };

        std::chrono::steady_clock::duration m_reliablePacketSendTimeout = std::chrono::seconds(3);
        std::chrono::steady_clock::duration m_reliablePacketTryTimeout = std::chrono::milliseconds(500);

        RUDPStream() noexcept;

        template<typename MsgT>
        void send(strand_t& strand, MsgT message, std::int64_t senderSessionID, std::int64_t targetSessionID) noexcept
        {
            static const auto messageTypeID = MsgT::getTypeIDStatic();

            boost::asio::post(strand, [this, senderSessionID, targetSessionID, msg = std::move(message)] {
                auto packet = MakeRef<Packet>();
                endpoint_t targetClientEndpoint;

                {
                    std::lock_guard lock(m_dataAccessMutex);

                    if(!m_registeredDataTypes.contains(messageTypeID))
                    {
                        LOG_E(SGCORE_TAG, "Unknown message type: {}. Type name: {}", messageTypeID, typeid(MsgT).name());
                        return;
                    }

                    const auto targetClientIt = m_registeredClients.find(targetSessionID);
                    if(targetClientIt == m_registeredClients.end())
                    {
                        LOG_E(SGCORE_TAG,
                              "Unknown target session: {}. Type ID: {}, type name: {}",
                              targetSessionID, messageTypeID, typeid(MsgT).name());
                        return;
                    }

                    targetClientEndpoint = targetClientIt->second.m_endpoint;

                    // writing message type id
                    std::memcpy(packet->data(), &messageTypeID, sizeof(messageTypeID));
                    // writing client session id
                    std::memcpy(packet->data() + sizeof(messageTypeID), &senderSessionID, sizeof(senderSessionID));
                    // writing message data
                    std::memcpy(packet->data() + sizeof(messageTypeID) + sizeof(senderSessionID), &msg, sizeof(MsgT));
                    // writing footer
                    std::memcpy(packet->data() + sizeof(messageTypeID) + sizeof(senderSessionID) + sizeof(MsgT), footer, sizeof(footer));
                }

                ++m_packetsSent;

                if constexpr(requires {
                    MsgT::use_rudp;
                })
                {
                    if constexpr(MsgT::use_rudp)
                    {
                        auto& reliableStream = m_reliableStreams[targetSessionID];
                        reliableStream.m_udpStream = this;
                        reliableStream.m_reliablePackets.emplace(packet, std::chrono::steady_clock::now(), messageTypeID, targetSessionID);

                        // if this packet is first
                        if(reliableStream.m_reliablePackets.size() == 1)
                        {
                            sendPacket(packet, targetClientEndpoint);
                        }
                        return;
                    }
                }

                sendPacket(packet, targetClientEndpoint);
            });
        }

        void receive(strand_t& strand) noexcept;

        void pollReliableStreams() noexcept;

    private:
        std::atomic<packet_id_t> m_packetsSent {};
        // first - client session id, second - stream
        std::unordered_map<session_id_t, ReliableStream> m_reliableStreams;

        void sendPacket(const Ref<Packet>& packet, const endpoint_t& targetEndpoint);
    };
}
