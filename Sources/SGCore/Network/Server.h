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

#include "RUDPStream.h"

namespace SGCore::Net
{
    // todo: make tcp
    // todo: убрать использование udp эндпоинтов
    // todo: регистрировать клиентов по tcp, если клиент отправил специфическое сообщение
    // todo: сделать менеджемент клиентами
    struct SGCORE_EXPORT Server
    {
        using endpoint_t = boost::asio::ip::udp::endpoint;

        RUDPStream m_stream;
        std::chrono::steady_clock::duration m_clientTimeout = std::chrono::seconds(3);

        Server(boost::asio::ip::port_type port);
        ~Server() noexcept;
        Server() noexcept;
        Server(Server&& other) noexcept;

        Coro::Task<> runReceivePoll() noexcept;

        template<typename MsgT>
        Coro::Task<> propagate(MsgT&& msg, std::int64_t senderSessionID) noexcept
        {
            const auto clients = m_stream.getRegisteredClients();

            for(const auto& [clientSessionID, clientEndpoint] : clients)
            {
                if(clientSessionID == senderSessionID) continue;

                m_stream.send(m_strand, msg, senderSessionID, clientSessionID);

                co_await Coro::returnToCaller();
            }
        }

        template<typename MsgT>
        void send(MsgT&& message, std::int64_t senderSessionID, std::int64_t targetSessionID) noexcept
        {
            m_stream.send(m_strand, std::forward<MsgT>(message), senderSessionID, targetSessionID);
        }

        template<typename MsgT>
        void send(MsgT&& message, std::int64_t targetSessionID) noexcept
        {
            m_stream.send(m_strand, std::forward<MsgT>(message), m_stream.m_sessionID, targetSessionID);
        }

        Server& operator=(Server&& other) noexcept;

    private:
        Ref<Threading::Thread> m_contextThread;

        std::uint16_t m_port = 2025;

        boost::asio::io_context m_context;
        boost::asio::strand<decltype(m_context)::executor_type> m_strand = boost::asio::make_strand(m_context);

        Packet m_recvBuffer;

        void createContextThread() noexcept;
    };
}
