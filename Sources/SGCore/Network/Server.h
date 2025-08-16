//
// Created by stuka on 02.08.2025.
//

#ifndef SUNGEARENGINE_NETWORK_SERVER_H
#define SUNGEARENGINE_NETWORK_SERVER_H

#include <WinSock2.h>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Threading/ThreadsPool.h"

#include "SGCore/Coro/Task.h"

#include "Packet.h"

namespace SGCore::Net
{
    struct Server
    {
        using socket_t = boost::asio::ip::udp::socket;
        using endpoint_t = boost::asio::ip::udp::endpoint;

        Server(boost::asio::ip::udp protocol, boost::asio::ip::port_type port);
        Server() noexcept;
        Server(Server&& other) noexcept;

        template<typename T>
        requires(std::is_invocable_v<T, const Packet&, size_t, endpoint_t>)
        Coro::Task<> runReceivePoll(T callback) noexcept
        {
            while(m_contextThread->isRunning())
            {
                if(!m_socket || m_socket->available() == 0)
                {
                    co_await Coro::returnToCaller();
                    continue;
                }

                /*if(getSocket().available() + 1 > m_recvBuffer.size())
                {
                    m_recvBuffer.resize(getSocket().available() + 1);
                }*/

                m_socket->async_receive_from(boost::asio::buffer(m_recvBuffer), m_receivedFromEndpoint, [this, callback](const boost::system::error_code& error, std::size_t bufferSize) {
                    const auto clientEndpoint = m_receivedFromEndpoint;

                    if(error)
                    {
                        std::cout << "error while reading data from client: " << clientEndpoint << ". error is: " << error.what() << std::endl;

                        /*m_connectedClients.erase(std::ranges::find(m_connectedClients, m_receivedFromEndpoint));
                        m_connectedClientsSet.erase(m_receivedFromEndpoint);*/

                        return;
                    }

                    // std::cout << "data received: " << bufferSize << std::endl;

                    if(!m_connectedClientsSet.contains(clientEndpoint))
                    {
                        m_connectedClientsSet.insert(clientEndpoint);
                        m_connectedClients.push_back(clientEndpoint);

                        std::cout << "new client: " << clientEndpoint << ", clients count: " << m_connectedClients.size() << std::endl;
                    }

                    const std::uint64_t clientEndpointSize = clientEndpoint.size();

                    const auto tmpBuf = m_recvBuffer;

                    // todo: make cycle for full packet

                    std::uint64_t dataTypeHash;

                    std::memcpy(&dataTypeHash, m_recvBuffer.data(), sizeof(std::uint64_t));

                    if(!m_registeredTypes.contains(dataTypeHash))
                    {
                        std::cout << "invalid data type: " << dataTypeHash << std::endl;

                        // invalid data type data or incomplete buffer
                        return;
                    }

                    // putting client size of address in buffer
                    std::memcpy(m_recvBuffer.data() + sizeof(dataTypeHash), &clientEndpointSize, sizeof(clientEndpointSize));
                    // putting client address in buffer
                    std::memcpy(m_recvBuffer.data() + sizeof(dataTypeHash) + sizeof(clientEndpointSize), clientEndpoint.data(), clientEndpoint.size());

                    const size_t metaDataSize = sizeof(dataTypeHash) + sizeof(clientEndpointSize) + clientEndpointSize;

                    std::memcpy(m_recvBuffer.data() + metaDataSize, tmpBuf.data() + sizeof(dataTypeHash), tmpBuf.size() - metaDataSize);

                    if(bufferSize > 0)
                    {
                        callback(m_recvBuffer, bufferSize, clientEndpoint);
                    }
                });

                co_await Coro::returnToCaller();
            }
        }

        template<typename T>
        void registerDataType() noexcept
        {
            m_registeredTypes[SGCore::hashString(T::type_name)] = sizeof(T);
        }

        Coro::Task<> propagatePacket(const Packet& packet, endpoint_t fromClient) noexcept;

        Server& operator=(Server&& other) noexcept;

    private:
        Ref<Threading::Thread> m_contextThread;

        boost::asio::ip::udp m_protocol = boost::asio::ip::udp::v4();
        boost::asio::ip::port_type m_port = 2025;
        boost::asio::io_context m_context;

        endpoint_t m_endpoint = endpoint_t(m_protocol, m_port);

        std::optional<socket_t> m_socket = std::nullopt;

        endpoint_t m_receivedFromEndpoint;

        std::unordered_set<endpoint_t> m_connectedClientsSet;
        std::vector<endpoint_t> m_connectedClients;

        Packet m_recvBuffer;
        std::unordered_map<endpoint_t, Ref<Packet>> m_packetsToSend;

        // first - data type id, second - sizeof data
        std::unordered_map<std::uint64_t, std::uint64_t> m_registeredTypes;
        // std::unordered_map<endpoint_t, std::vector<Ref<Packet>>> m_sentPackets;
    };
}

#endif // SUNGEARENGINE_NETWORK_SERVER_H
