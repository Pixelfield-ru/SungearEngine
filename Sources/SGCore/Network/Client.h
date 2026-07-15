//
// Created by stuka on 03.08.2025.
//

#pragma once

#include "SGCore/Utils/Platform.h"

#if SG_PLATFORM_OS_WINDOWS
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
#include "UDPStream.h"

namespace SGCore::Net
{
    // todo: make tcp
    // todo: добавить UDPStream и убрать использование udp endpoint из структуры Client.
    // todo: отправлять специфическое сообщение серверу в функции Client::connect() для регистрации клиента
    struct SGCORE_EXPORT Client
    {
        using socket_t = boost::asio::ip::udp::socket;
        using endpoint_t = boost::asio::ip::udp::endpoint;

        std::function<void()> onConnected;
        std::function<void()> onDisconnected;

        Client() noexcept;
        ~Client() noexcept;

        void connect(const std::string& endpointAddress, boost::asio::ip::port_type endpointPort, std::chrono::system_clock::duration retryInterval = std::chrono::seconds(3), int retriesCount = 5) noexcept;

        template<typename MessageT>
        requires(requires { MessageT::getTypeIDStatic(); })
        Coro::Task<> send(MessageT&& data) noexcept
        {
            while(!m_isConnected)
            {
                co_await Coro::returnToCaller();
            }

            m_udpStream.sendMessage(m_strand, std::forward<MessageT>(data), m_udpStream.m_sessionID, 0);
        }

        Coro::Task<> runReceivePoll() noexcept;

        template<typename T>
        DataType& registerDataType() noexcept
        {
            return m_udpStream.registerDataType<T>();
        }

        void setSessionID(std::int64_t id) noexcept;
        std::int64_t getSessionID() const noexcept;

        [[nodiscard]] bool isConnected() const noexcept;

    private:
        Ref<Threading::Thread> m_contextThread;

        boost::asio::io_context m_context;
        boost::asio::strand<decltype(m_context)::executor_type> m_strand = boost::asio::make_strand(m_context);

        std::atomic<bool> m_isConnected = false;

        std::atomic<std::uint16_t> m_maxSendErrors = 8;
        std::uint16_t m_sendErrorsCount {};

        UDPStream m_udpStream;

        void createContextThread() noexcept;
        void setConnected(bool connected) noexcept;
    };
}
