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
                    if(error)
                    {
                        std::cout << "error while reading data from client: " << m_receivedFromEndpoint << ". error is: " << error.what() << std::endl;

                        /*m_connectedClients.erase(std::ranges::find(m_connectedClients, m_receivedFromEndpoint));
                        m_connectedClientsSet.erase(m_receivedFromEndpoint);*/

                        return;
                    }

                    // std::cout << "data received: " << bufferSize << std::endl;

                    bool isNewClient = false;

                    if(!m_connectedClientsSet.contains(m_receivedFromEndpoint))
                    {
                        m_connectedClientsSet.insert(m_receivedFromEndpoint);
                        m_connectedClients.push_back(m_receivedFromEndpoint);

                        std::cout << "new client: " << m_receivedFromEndpoint << ", clients count: " << m_connectedClients.size() << std::endl;

                        isNewClient = true;
                    }

                    m_recvBuffer[m_recvBuffer.size() - 1] = isNewClient;

                    if(bufferSize > 0)
                    {
                        callback(m_recvBuffer, bufferSize, m_receivedFromEndpoint);
                    }
                });

                co_await Coro::returnToCaller();
            }
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

        std::unordered_map<endpoint_t, Packet> m_clientsPackets;

        Packet m_recvBuffer;
        std::unordered_map<endpoint_t, Ref<Packet>> m_packetsToSend;
    };
}

#endif // SUNGEARENGINE_NETWORK_SERVER_H
