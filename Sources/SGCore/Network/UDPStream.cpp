//
// Created by stuka on 13.07.2026.
//

#include "UDPStream.h"

#include "Utils.h"
#include "SGCore/Logger/Logger.h"

void SGCore::Net::UDPStream::sendPacket(strand_t& strand, const Packet& packet, endpoint_t toClient) noexcept
{
    boost::asio::post(strand, [this, packet, toClient] {
        m_socket->async_send_to(boost::asio::buffer(packet), toClient, [toClient](boost::system::error_code errorCode, size_t bufferSize) {
            if(errorCode)
            {
                LOG_E(SGCORE_TAG, "Cannot propagate buffer with size {} to client {}", bufferSize, toClient.address().to_string());
            }
        });
    });
}

void SGCore::Net::UDPStream::receive(strand_t& strand) noexcept
{
    if(!m_socket || m_socket->available() == 0)
    {
        return;
    }

    boost::asio::post(strand, [this, callback = std::move(callback)] {
        m_socket->async_receive_from(boost::asio::buffer(m_recvBuffer), m_receiveEndpoint, [this, callback = std::move(callback)](const boost::system::error_code& error, std::size_t bufferSize) {
            const auto clientEndpoint = m_receiveEndpoint;
            // auto tmpBuf = m_recvBuffer;
            Packet tmpBuf {};
            const auto originalBuffer = m_recvBuffer;

            if(error)
            {
                LOG_E(SGCORE_TAG, "Cannot read data from client {}. Error is: {}", clientEndpoint.address().to_string(), error.what());

                return;
            }

            size_t formedPacketOffset = 0;
            size_t originalPacketOffset = 0;

            // todo: fix data receiving or data sending or something like that idk
            // processing all packet. adding client endpoint
            while(true)
            {
                // std::cout << "originalPacketOffset: " << originalPacketOffset << std::endl;
                std::uint64_t dataTypeHash;
                std::memcpy(&dataTypeHash, originalBuffer.data() + originalPacketOffset, sizeof(dataTypeHash));

                auto registeredStreamIt = m_registeredDataTypes.find(dataTypeHash);
                if(registeredStreamIt == m_registeredDataTypes.end())
                {
                    // std::cout << "invalid data type: " << dataTypeHash << std::endl;

                    // todo: maybe += 1 byte and continue to trying to find valid data??
                    // invalid data type data or incomplete buffer
                    ++originalPacketOffset;
                    if(originalPacketOffset >= originalBuffer.size()) break;

                    continue;
                    // break;
                }

                auto& registeredStream = registeredStreamIt->second;
                const auto registeredTypeSize = registeredStream.m_dataSize;

                // writing data size ==========================================
                std::memcpy(tmpBuf.data() + formedPacketOffset, &dataTypeHash, sizeof(dataTypeHash));
                // ============================================================

                formedPacketOffset += sizeof(dataTypeHash);

                // writing client endpoint ====================================
                size_t clientEndpointSize = 0;
                const bool clientEndpointWriteSuccess = Utils::writeEndpoint(tmpBuf, formedPacketOffset, clientEndpoint, clientEndpointSize);
                // ============================================================

                formedPacketOffset += clientEndpointSize;

                if(!clientEndpointWriteSuccess ||
                   formedPacketOffset + registeredTypeSize > tmpBuf.size() ||
                   originalPacketOffset + sizeof(dataTypeHash) + registeredTypeSize > originalBuffer.size())
                {
                    break;
                }

                std::memcpy(tmpBuf.data() + formedPacketOffset, originalBuffer.data() + originalPacketOffset + sizeof(dataTypeHash), registeredTypeSize);

                formedPacketOffset += registeredTypeSize;
                originalPacketOffset += sizeof(dataTypeHash) + registeredTypeSize;
            }

            if(bufferSize > 0)
            {
                callback(tmpBuf, bufferSize, clientEndpoint);
            }
        });
    });
}
