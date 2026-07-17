//
// Created by stuka on 13.07.2026.
//

#include "RUDPStream.h"

#include <ranges>

#include "GotReliablePacketMessage.h"
#include "Utils.h"
#include "SGCore/Logger/Logger.h"

void SGCore::Net::RUDPStream::ReliableStream::pollPackets() noexcept
{
    if(m_reliablePackets.empty()) return;

    auto& packet = m_reliablePackets.front();

    const auto now = std::chrono::steady_clock::now();

    const auto timeElapsed = now - packet.m_sendTime;
    const bool isSendFailed = timeElapsed > m_udpStream->m_reliablePacketSendTimeout;
    const bool isTryFailed = timeElapsed > m_udpStream->m_reliablePacketTryTimeout;

    if(isSendFailed)
    {
        auto& dataType = m_udpStream->m_registeredDataTypes.at(packet.m_packetTypeID);
        const auto targetEndpoint = m_udpStream->m_registeredClients.at(packet.m_targetSessionID).m_endpoint;

        Packet pureData {};
        std::memcpy(pureData.data(), packet.m_packet->data() + sizeof(packet.m_packetTypeID) + sizeof(session_id_t), dataType.getDataSize());

        if(dataType.onSendFailed)
        {
            dataType.onSendFailed(pureData, targetEndpoint, packet.m_targetSessionID);
        }

        m_reliablePackets.pop();

        // sending next packet
        if(!m_reliablePackets.empty())
        {
            auto& nextPacket = m_reliablePackets.front();
            nextPacket.m_sendTime = std::chrono::steady_clock::now();

            const auto nextTargetEndpoint = m_udpStream->m_registeredClients.at(nextPacket.m_targetSessionID).m_endpoint;

            m_udpStream->sendPacket(nextPacket.m_packet, nextTargetEndpoint);
        }
    }

    if(isTryFailed)
    {
        const auto targetEndpoint = m_udpStream->m_registeredClients.at(packet.m_targetSessionID).m_endpoint;

        // send packet again
        m_udpStream->sendPacket(packet.m_packet, targetEndpoint);
    }
}

SGCore::Net::RUDPStream::RUDPStream() noexcept
{
    auto& reliablePacketType = registerDataType<GotReliablePacketMessage>();
    reliablePacketType.m_authRequired = false;
    reliablePacketType.onReceive = [this](const Packet& packet, endpoint_t senderEndpoint, std::int64_t senderSessionID) {
        const auto reliableStreamIt = m_reliableStreams.find(senderSessionID);
        if(reliableStreamIt == m_reliableStreams.end())
        {
            LOG_E(SGCORE_TAG, "Unknown sender: {}", senderSessionID);
            return;
        }

        auto& reliableStream = reliableStreamIt->second;
        reliableStream.m_reliablePackets.pop();

        if(reliableStream.m_reliablePackets.empty()) return;

        // sending next packet
        auto& nextPacket = reliableStream.m_reliablePackets.front();
        nextPacket.m_sendTime = std::chrono::steady_clock::now();

        sendPacket(nextPacket.m_packet, senderEndpoint);
    };
}

void SGCore::Net::RUDPStream::receive(strand_t& strand) noexcept
{
    if(!m_socket || m_socket->available() == 0)
    {
        return;
    }

    boost::asio::post(strand, [this, &strand] {
        m_socket->async_receive_from(boost::asio::buffer(m_recvBuffer), m_receiveEndpoint, [this, &strand](const boost::system::error_code& error, std::size_t bufferSize) {
            if(error)
            {
                LOG_E(SGCORE_TAG, "Cannot read data from {}. Error is: {}", m_receiveEndpoint.address().to_string(), error.what());

                return;
            }

            decltype(m_registeredDataTypes) registeredDataTypes;

            {
                std::lock_guard lock(m_dataAccessMutex);
                registeredDataTypes = m_registeredDataTypes;
            }

            std::uint64_t dataTypeHash;
            std::memcpy(&dataTypeHash, m_recvBuffer.data(), sizeof(dataTypeHash));

            auto registeredStreamIt = registeredDataTypes.find(dataTypeHash);
            if(registeredStreamIt == registeredDataTypes.end())
            {
                // invalid data type
                return;
            }

            auto& registeredStream = registeredStreamIt->second;
            const auto registeredTypeSize = registeredStream.getDataSize();

            std::int64_t sessionID {};

            char factFooter[4];
            std::memcpy(factFooter, m_recvBuffer.data() + sizeof(dataTypeHash) + sizeof(sessionID) + registeredTypeSize, 4);
            if(std::memcmp(factFooter, footer, 4) != 0)
            {
                // broken packet
                return;
            }

            std::memcpy(&sessionID, m_recvBuffer.data() + sizeof(dataTypeHash), sizeof(sessionID));

            {
                std::lock_guard lock(m_dataAccessMutex);

                if(!m_registeredClients.contains(sessionID))
                {
                    // unauthorized
                    if(registeredStream.m_authRequired) return;
                }
                else
                {
                    m_registeredClients[sessionID].m_lastSendTime = std::chrono::steady_clock::now();
                }
            }

            // we bypass looping by checking that the received type is not GotReliablePacketMessage.
            const bool isMessageIsReliableACK = dataTypeHash == GotReliablePacketMessage::getTypeIDStatic();

            // if type is not used for auth then sending response GotReliablePacketMessage immediately
            // to avoid wasting time on user data processing
            if(!isMessageIsReliableACK && registeredStream.isUsingRUDP() && !registeredStream.isUsedForAuth())
            {
                send(strand, GotReliablePacketMessage {}, m_sessionID, sessionID);
            }

            Packet pureData {};
            std::memcpy(pureData.data(), m_recvBuffer.data() + sizeof(dataTypeHash) + sizeof(sessionID), registeredTypeSize);

            if(registeredStream.onReceive)
            {
                registeredStream.onReceive(pureData, m_receiveEndpoint, sessionID);
            }

            // if type is used for auth then sending response GotReliablePacketMessage after user data processing.
            // it is because user must set session id for new client
            if(!isMessageIsReliableACK && registeredStream.isUsingRUDP() && registeredStream.isUsedForAuth())
            {
                // getting new session id.
                // i think that this cycle is not too bad because auth message won't be used so often
                for(const auto& [clientSessionID, endpointInfo] : m_registeredClients)
                {
                    if(endpointInfo.m_endpoint == m_receiveEndpoint)
                    {
                        sessionID = clientSessionID;
                    }
                }

                std::cout << "sending reliable to: " << sessionID << std::endl;

                send(strand, GotReliablePacketMessage {}, m_sessionID, sessionID);
            }
        });
    });
}

void SGCore::Net::RUDPStream::pollReliableStreams() noexcept
{
    for(auto& reliableStream : m_reliableStreams | std::views::values)
    {
        reliableStream.pollPackets();
    }
}

void SGCore::Net::RUDPStream::sendPacket(const Ref<Packet>& packet, const endpoint_t& targetEndpoint)
{
    m_socket->async_send_to(boost::asio::buffer(*packet), targetEndpoint, [targetEndpoint, packet](boost::system::error_code errorCode, size_t bufferSize) {
        if(errorCode)
        {
            LOG_E(SGCORE_TAG, "Cannot send packet with size {} to client {}", bufferSize, targetEndpoint.address().to_string());
        }
    });
}