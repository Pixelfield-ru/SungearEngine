//
// Created by stuka on 02.08.2025.
//

#pragma once

#include "Packet.h"

#include "../Utils/Platform.h"

#if SG_PLATFORM_OS_WINDOWS
#include <WinSock2.h>
#endif

#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/udp.hpp>

#include "ClientConnectedMessage.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Threading/ThreadsPool.h"

#include "SGCore/Coro/Task.h"

#include "ClientDisconnectedMessage.h"

#include "Utils.h"

#include "UDPStream.h"

namespace SGCore::Net
{
    // todo: make tcp
    // todo: убрать использование udp эндпоинтов
    // todo: регистрировать клиентов по tcp, если клиент отправил специфическое сообщение
    // todo: сделать менеджемент клиентами
    struct SGCORE_EXPORT Server
    {
        using endpoint_t = boost::asio::ip::udp::endpoint;

        Server(boost::asio::ip::port_type port);
        ~Server() noexcept;
        Server() noexcept;
        Server(Server&& other) noexcept;

        Coro::Task<> runReceivePoll() noexcept
        {
            while(m_contextThread->isRunning())
            {
                m_udpStream.receive(m_strand);
                co_await Coro::returnToCaller();
            }
        }

        template<typename T>
        DataType& registerDataType() noexcept
        {
            return m_udpStream.registerDataType<T>();
        }

        void registerClient(const endpoint_t& clientEndpoint, std::int64_t clientSessionID) noexcept;
        bool isClientRegistered(std::int64_t clientSessionID) const noexcept;

        template<typename MsgT>
        Coro::Task<> propagateMessage(MsgT&& msg, std::int64_t senderSessionID) noexcept
        {
            const auto clients = m_udpStream.getRegisteredClients();

            for(const auto& [clientSessionID, clientEndpoint] : clients)
            {
                if(clientSessionID == senderSessionID) continue;

                m_udpStream.sendMessage(m_strand, msg, senderSessionID, clientSessionID);

                co_await Coro::returnToCaller();
            }
        }

        template<typename MsgT>
        void sendMessage(MsgT&& message, std::int64_t targetSessionID) noexcept
        {
            m_udpStream.sendMessage(m_strand, std::forward<MsgT>(message), 0, targetSessionID);
        }

        Server& operator=(Server&& other) noexcept;

    private:
        Ref<Threading::Thread> m_contextThread;

        UDPStream m_udpStream;

        std::uint16_t m_port = 2025;

        boost::asio::io_context m_context;
        boost::asio::strand<decltype(m_context)::executor_type> m_strand = boost::asio::make_strand(m_context);

        Packet m_recvBuffer;

        void createContextThread() noexcept;
    };
}
