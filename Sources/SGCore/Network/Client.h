//
// Created by stuka on 03.08.2025.
//

#ifndef SUNGEARENGINE_NETWORK_CLIENT_H
#define SUNGEARENGINE_NETWORK_CLIENT_H

#include "SGCore/CrashHandler/Platform.h"

#ifdef PLATFORM_OS_WINDOWS
#include <WinSock2.h>
#endif

#include <boost/asio/strand.hpp>

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>

#include "SGCore/Coro/Task.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Threading/Thread.h"

#include "Packet.h"

namespace SGCore::Net
{
    struct DataStream
    {
        friend struct Client;

        std::function<void(const Packet& data, boost::asio::ip::udp::endpoint from)> onReceive;

    private:
        std::uint64_t m_dataSize = 0;
    };

    struct Client
    {
        using socket_t = boost::asio::ip::udp::socket;
        using endpoint_t = boost::asio::ip::udp::endpoint;

        Client() noexcept;

        void connect(const std::string& endpointAddress, boost::asio::ip::port_type endpointPort, std::chrono::system_clock::duration retryInterval = std::chrono::seconds(3), int retriesCount = 5) noexcept;

        template<typename T>
        requires(requires { T::type_name; })
        Coro::Task<> send(const T& data) noexcept
        {
            const std::uint64_t dataTypeHash = SGCore::hashString(T::type_name);

            if(!m_registeredDataStreams.contains(dataTypeHash))
            {
                // unregistered data type
                co_return;
            }

            Packet packet;
            std::memcpy(packet.data(), &dataTypeHash, sizeof(dataTypeHash));
            std::memcpy(packet.data() + sizeof(dataTypeHash), &data, sizeof(data));

            auto sharedPacket = MakeRef<Packet>(packet);

            while(!m_isConnected)
            {
                co_await Coro::returnToCaller();
            }

            // capturing sharedPacket to save packet (extend the lifetime)
            boost::asio::post(m_strand, [this, sharedPacket] {
                m_socket.async_send(boost::asio::buffer(*sharedPacket), [this, sharedPacket](boost::system::error_code errorCode, size_t bytesCnt) {
                    if(errorCode)
                    {
                        std::cout << "send failed: " << errorCode.message() << ". count of bytes to send: " << bytesCnt << std::endl;
                        return;
                    }
                });
            });
        }

        Coro::Task<> runReceivePoll() noexcept;

        template<typename T>
        DataStream& registerDataStream() noexcept
        {
            auto& dataStream = m_registeredDataStreams[SGCore::hashString(T::type_name)];
            dataStream.m_dataSize = sizeof(T);

            return dataStream;
        }

        [[nodiscard]] bool isConnected() const noexcept;

    private:
        Ref<Threading::Thread> m_contextThread = Threading::Thread::create(std::chrono::milliseconds(0));

        endpoint_t m_recvEndpoint;
        endpoint_t m_serverEndpoint;
        endpoint_t m_clientAddress;

        boost::asio::io_context m_context;
        boost::asio::strand<decltype(m_context)::executor_type> m_strand = boost::asio::make_strand(m_context);

        socket_t m_socket = socket_t(m_context);

        Packet m_recvBuffer;
        size_t m_bytesToSkip = 0;

        std::unordered_map<std::uint64_t, DataStream> m_registeredDataStreams;

        std::atomic<bool> m_isConnected = false;
    };
}

#endif // SUNGEARENGINE_NETWORK_CLIENT_H
