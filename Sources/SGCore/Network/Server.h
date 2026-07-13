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

        template<typename T>
        requires(std::is_invocable_v<T, const Packet&, size_t, endpoint_t>)
        Coro::Task<> runReceivePoll(T callback) noexcept
        {
            while(m_contextThread->isRunning())
            {
                m_udpStream.receive(m_strand);
                co_await Coro::returnToCaller();
            }
        }

        template<typename T>
        void registerDataType() noexcept
        {
            m_udpStream.registerDataType<T>();
        }

        Coro::Task<> propagatePacket(const Packet& packet, endpoint_t fromClient) noexcept;
        void sendPacket(const Packet& packet, endpoint_t toClient) noexcept;

        Server& operator=(Server&& other) noexcept;

    private:
        Ref<Threading::Thread> m_contextThread;

        UDPStream m_udpStream;

        std::uint16_t m_port = 2025;

        boost::asio::io_context m_context;
        boost::asio::strand<decltype(m_context)::executor_type> m_strand = boost::asio::make_strand(m_context);

        endpoint_t m_receivedFromEndpoint;

        std::unordered_set<endpoint_t> m_connectedClientsSet;
        std::vector<endpoint_t> m_connectedClients;
        std::mutex m_connectedClientsContainersMutex;

        Packet m_recvBuffer;
        std::unordered_map<endpoint_t, Ref<Packet>> m_packetsToSend;

        void createContextThread() noexcept;
    };
}
