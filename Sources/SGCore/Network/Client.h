//
// Created by stuka on 03.08.2025.
//

#ifndef SUNGEARENGINE_NETWORK_CLIENT_H
#define SUNGEARENGINE_NETWORK_CLIENT_H

#include <WinSock2.h>

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>

#include <SGCore/Coro/Task.h>
#include <SGCore/Main/CoreGlobals.h>
#include <SGCore/Threading/Thread.h>

#include "Packet.h"

namespace SGCore::Net
{
    struct Client
    {
        using socket_t = boost::asio::ip::udp::socket;
        using endpoint_t = boost::asio::ip::udp::endpoint;

        Client() noexcept;

        void connect(const std::string& endpointAddress, boost::asio::ip::port_type endpointPort, std::chrono::system_clock::duration retryInterval = std::chrono::seconds(3), int retriesCount = 5) noexcept;

        Coro::Task<> send(Packet packet) noexcept;

        template<typename T>
        requires(std::is_invocable_v<T, const Packet&, size_t, endpoint_t>)
        Coro::Task<> runReceivePoll(T callback) noexcept
        {
            while(m_contextThread->isRunning())
            {
                if(m_socket.available() == 0 || !m_isConnected.load())
                {
                    co_await Coro::returnToCaller();
                    continue;
                }

                while(!m_isDataReceived.load())
                {
                    co_await Coro::returnToCaller();
                }

                m_isDataReceived = false;

                if(m_socket.available() > m_recvBuffer.size())
                {
                    m_recvBuffer.resize(m_socket.available());
                }

                m_socket.async_receive_from(boost::asio::buffer(m_recvBuffer), m_recvEndpoint, [this, movedCallback = std::move(callback)](const boost::system::error_code& error, std::size_t bufferSize) {
                    if(m_recvEndpoint != m_serverEndpoint)
                    {
                        std::cout << "unknown endpoint: " << m_recvEndpoint << std::endl;
                        // unknown endpoint
                        m_isDataReceived = true;
                        return;
                    }

                    if(error)
                    {
                        std::cerr << "Client: error while receiving packet: " << error.message() << '\n';
                        m_isDataReceived = true;
                        return;
                    }

                    movedCallback(m_recvBuffer, bufferSize, m_serverEndpoint);

                    m_isDataReceived = true;
                });

                co_await Coro::returnToCaller();
            }
        }

        [[nodiscard]] bool isConnected() const noexcept;

    private:
        Ref<Threading::Thread> m_contextThread = Threading::Thread::create(std::chrono::milliseconds(0));

        endpoint_t m_recvEndpoint;
        endpoint_t m_serverEndpoint;
        endpoint_t m_clientAddress;

        boost::asio::io_context m_context;

        socket_t m_socket = socket_t(m_context);

        Packet m_recvBuffer;

        std::deque<Packet> m_sentPackets;

        std::atomic<bool> m_isConnected = false;
        std::atomic<bool> m_isDataReceived = true;
    };
}

#endif // SUNGEARENGINE_NETWORK_CLIENT_H
