//
// Created by stuka on 02.08.2025.
//

#include "Server.h"

#include <boost/asio.hpp>

#include <iostream>

#include "Packet.h"

#include <SGCore/Threading/Task.h>

SGCore::Net::Server::Server(boost::asio::ip::port_type port)
{
    m_port = port;
    m_stream.m_serverEndpoint = RUDPStream::endpoint_t(boost::asio::ip::udp::v4(), m_port);
    m_stream.m_socket = RUDPStream::socket_t(m_context, m_stream.m_serverEndpoint);

    m_stream.m_sessionID = 0;

    m_stream.registerDataType<ClientDisconnectedMessage>();
    m_stream.registerDataType<ClientConnectedMessage>();

    createContextThread();
}

SGCore::Net::Server::~Server() noexcept
{
    m_context.stop();
    m_contextThread->join();
}

SGCore::Net::Server::Server() noexcept
{
    m_stream.m_serverEndpoint = RUDPStream::endpoint_t(boost::asio::ip::udp::v4(), m_port);

    createContextThread();
}

SGCore::Net::Server::Server(Server&& other) noexcept
{
    m_port = other.m_port;

    m_stream.m_sessionID = 0;
    m_stream.m_serverEndpoint = RUDPStream::endpoint_t(boost::asio::ip::udp::v4(), m_port);
    m_stream.registerDataType<ClientDisconnectedMessage>();
    m_stream.registerDataType<ClientConnectedMessage>();

    if(other.m_contextThread)
    {
        other.m_contextThread->join();
    }

    if(other.m_stream.m_socket)
    {
        other.m_stream.m_socket = std::nullopt;
    }

    m_recvBuffer = other.m_recvBuffer;

    if(m_stream.m_socket)
    {
        m_stream.m_socket = std::nullopt;
    }

    m_stream.m_socket = RUDPStream::socket_t(m_context, m_stream.m_serverEndpoint);

    createContextThread();
}

SGCore::Coro::Task<> SGCore::Net::Server::runReceivePoll() noexcept
{
    while(m_contextThread->isRunning())
    {
        const auto registeredClients = m_stream.getRegisteredClients();
        for(const auto& [sessionID, endpointInfo] : registeredClients)
        {
            if(std::chrono::steady_clock::now() - endpointInfo.m_lastSendTime > m_clientTimeout)
            {
                ClientDisconnectedMessage disconnectedMessage;
                propagate(disconnectedMessage, sessionID);

                m_stream.removeClient(sessionID);
            }
        }

        m_stream.pollReliableStreams();

        m_stream.receive(m_strand);

        co_await Coro::returnToCaller();
    }
}

SGCore::Net::Server& SGCore::Net::Server::operator=(Server&& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    if(other.m_contextThread)
    {
        other.m_contextThread->join();
    }

    m_recvBuffer = other.m_recvBuffer;

    m_port = other.m_port;

    m_stream.m_serverEndpoint = endpoint_t(boost::asio::ip::udp::v4(), m_port);

    if(other.m_stream.m_socket)
    {
        other.m_stream.m_socket = std::nullopt;
    }

    if(m_stream.m_socket)
    {
        m_stream.m_socket = std::nullopt;
    }

    m_stream.m_socket = RUDPStream::socket_t(m_context, m_stream.m_serverEndpoint);

    createContextThread();

    return *this;
}

void SGCore::Net::Server::createContextThread() noexcept
{
    if(m_contextThread) return;

    m_contextThread = Threading::Thread::create(std::chrono::milliseconds(0));

    auto contextTask = MakeRef<Threading::Task>();
    contextTask->m_isStatic = true;
    contextTask->setOnExecuteCallback([this]() {
        m_context.run();
    });

    m_contextThread->addTask(contextTask);
    m_contextThread->start();
}
