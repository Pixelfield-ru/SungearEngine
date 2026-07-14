//
// Created by stuka on 13.07.2026.
//

#include "UDPStream.h"

#include "Utils.h"
#include "SGCore/Logger/Logger.h"

void SGCore::Net::UDPStream::receive(strand_t& strand) noexcept
{
    if(!m_socket || m_socket->available() == 0)
    {
        return;
    }

    boost::asio::post(strand, [this] {
        m_socket->async_receive_from(boost::asio::buffer(m_recvBuffer), m_receiveEndpoint, [this](const boost::system::error_code& error, std::size_t bufferSize) {
            if(error)
            {
                LOG_E(SGCORE_TAG, "Cannot read data from client {}. Error is: {}", m_receiveEndpoint.address().to_string(), error.what());

                return;
            }

            std::uint64_t dataTypeHash;
            std::memcpy(&dataTypeHash, m_recvBuffer.data(), sizeof(dataTypeHash));

            auto registeredStreamIt = m_registeredDataTypes.find(dataTypeHash);
            if(registeredStreamIt == m_registeredDataTypes.end())
            {
                // invalid data type
                return;
            }

            auto& registeredStream = registeredStreamIt->second;
            const auto registeredTypeSize = registeredStream.m_dataSize;

            std::int64_t sessionID {};

            char factFooter[4];
            std::memcpy(factFooter, m_recvBuffer.data() + sizeof(dataTypeHash) + sizeof(sessionID) + registeredTypeSize, 4);
            if(factFooter[0] == footer[0] &&
               factFooter[1] == footer[1] &&
               factFooter[2] == footer[2] &&
               factFooter[3] == footer[3])
            {
                // broken packet
                return;
            }

            std::memcpy(&sessionID, m_recvBuffer.data() + sizeof(dataTypeHash), sizeof(sessionID));
            if(!m_registeredClients.contains(sessionID))
            {
                // unauthorized
                if(registeredStream.m_authRequired) return;

                sessionID = -1;
            }

            Packet pureData {};
            std::memcpy(pureData.data(), m_recvBuffer.data() + sizeof(dataTypeHash) + sizeof(sessionID), registeredTypeSize);

            if(registeredStream.onReceive)
            {
                registeredStream.onReceive(pureData, m_receiveEndpoint, sessionID);
            }
        });
    });
}
