//
// Created by stuka on 02.08.2025.
//

#include "Server.h"

#include <boost/asio.hpp>

#include <iostream>

#include "Packet.h"

#include <SGCore/Threading/Task.h>

SGCore::Net::Server::Server(boost::asio::ip::udp protocol, boost::asio::ip::port_type port) : m_protocol(protocol),
    m_port(port),
    m_endpoint(m_protocol, m_port)
{
    m_socket = socket_t(m_context, m_endpoint);

    m_contextThread = Threading::Thread::create(std::chrono::milliseconds(0));

    auto contextTask = MakeRef<Threading::Task>();
    contextTask->m_isStatic = true;
    contextTask->setOnExecuteCallback([this]() {
        m_context.run();
    });

    m_contextThread->addTask(contextTask);
    m_contextThread->start();
}

SGCore::Net::Server::Server() noexcept :
    m_endpoint(m_protocol, m_port)
{
    m_contextThread = Threading::Thread::create(std::chrono::milliseconds(0));

    auto contextTask = MakeRef<Threading::Task>();
    contextTask->m_isStatic = true;
    contextTask->setOnExecuteCallback([this]() {
        m_context.run();
    });

    m_contextThread->addTask(contextTask);
    m_contextThread->start();
}

SGCore::Net::Server::Server(Server&& other) noexcept : m_protocol(other.m_protocol),
    m_port(other.m_port),
    m_endpoint(m_protocol, m_port)
{
    other.m_contextThread->join();

    if(other.m_socket)
    {
        other.m_socket = std::nullopt;
    }

    m_recvBuffer = std::move(other.m_recvBuffer);

    if(m_socket)
    {
        m_socket = std::nullopt;
    }

    m_socket = socket_t(m_context, m_endpoint);

    auto contextTask = MakeRef<Threading::Task>();
    contextTask->m_isStatic = true;
    contextTask->setOnExecuteCallback([this]() {
        m_context.run();
    });

    m_contextThread->addTask(contextTask);
    m_contextThread->start();
}

SGCore::Coro::Task<> SGCore::Net::Server::propagatePacket(const Packet& packet, endpoint_t fromClient) noexcept
{
    std::vector<endpoint_t> connectedClientsVec;
    {
        std::lock_guard guard(m_connectedClientsContainersMutex);
        connectedClientsVec = m_connectedClients;
    }

    for(size_t i = 0; i < connectedClientsVec.size(); ++i)
    {
        const auto& client = connectedClientsVec[i];

        if(client == fromClient) continue;

        auto clientPacket = m_packetsToSend[client];
        if(!clientPacket)
        {
            clientPacket = MakeRef<Packet>();
        }

        *clientPacket = packet;

        // capturing client packet to save data
        // m_socket->send_to(boost::asio::buffer(packet), client);
        boost::asio::post(m_strand, [this, clientPacket, client] {
            m_socket->async_send_to(boost::asio::buffer(*clientPacket), client, [client, clientPacket](boost::system::error_code errorCode, size_t bufferSize) {
                if(errorCode)
                {
                    std::cout << "can not propagate buffer with size " << bufferSize << " to client " << client << std::endl;
                }
            });
        });

        co_await Coro::returnToCaller();
    }
}

SGCore::Net::Server& SGCore::Net::Server::operator=(Server&& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    other.m_contextThread->join();

    m_recvBuffer = std::move(other.m_recvBuffer);

    m_protocol = other.m_protocol;
    m_port = other.m_port;

    m_endpoint = endpoint_t(m_protocol, m_port);

    if(other.m_socket)
    {
        other.m_socket = std::nullopt;
    }

    if(m_socket)
    {
        m_socket = std::nullopt;
    }

    m_socket = socket_t(m_context, m_endpoint);

    m_contextThread = Threading::Thread::create(std::chrono::milliseconds(0));

    auto contextTask = MakeRef<Threading::Task>();
    contextTask->m_isStatic = true;
    contextTask->setOnExecuteCallback([this]() {
        m_context.run();
    });

    m_contextThread->addTask(contextTask);
    m_contextThread->start();

    return *this;
}