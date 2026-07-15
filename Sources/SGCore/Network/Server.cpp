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
    m_udpStream.m_serverEndpoint = UDPStream::endpoint_t(boost::asio::ip::udp::v4(), m_port);
    m_udpStream.m_socket = UDPStream::socket_t(m_context, m_udpStream.m_serverEndpoint);

    createContextThread();
}

SGCore::Net::Server::~Server() noexcept
{
    m_context.stop();
    m_contextThread->join();
}

SGCore::Net::Server::Server() noexcept
{
    m_udpStream.m_serverEndpoint = UDPStream::endpoint_t(boost::asio::ip::udp::v4(), m_port);

    createContextThread();
}

SGCore::Net::Server::Server(Server&& other) noexcept
{
    m_port = other.m_port;
    m_udpStream.m_serverEndpoint = UDPStream::endpoint_t(boost::asio::ip::udp::v4(), m_port);

    if(other.m_contextThread)
    {
        other.m_contextThread->join();
    }

    if(other.m_udpStream.m_socket)
    {
        other.m_udpStream.m_socket = std::nullopt;
    }

    m_recvBuffer = other.m_recvBuffer;

    if(m_udpStream.m_socket)
    {
        m_udpStream.m_socket = std::nullopt;
    }

    m_udpStream.m_socket = UDPStream::socket_t(m_context, m_udpStream.m_serverEndpoint);

    createContextThread();
}

void SGCore::Net::Server::registerClient(const endpoint_t& clientEndpoint, std::int64_t clientSessionID) noexcept
{
    m_udpStream.registerClient(clientEndpoint, clientSessionID);
}

bool SGCore::Net::Server::isClientRegistered(std::int64_t clientSessionID) const noexcept
{
    return m_udpStream.isClientRegistered(clientSessionID);
}

SGCore::Net::Server& SGCore::Net::Server::operator=(Server&& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    if(other.m_contextThread)
    {
        other.m_contextThread->join();
    }

    m_recvBuffer = std::move(other.m_recvBuffer);

    m_port = other.m_port;

    m_udpStream.m_serverEndpoint = endpoint_t(boost::asio::ip::udp::v4(), m_port);

    if(other.m_udpStream.m_socket)
    {
        other.m_udpStream.m_socket = std::nullopt;
    }

    if(m_udpStream.m_socket)
    {
        m_udpStream.m_socket = std::nullopt;
    }

    m_udpStream.m_socket = UDPStream::socket_t(m_context, m_udpStream.m_serverEndpoint);

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
