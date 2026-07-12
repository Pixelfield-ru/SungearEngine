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

    createContextThread();
}

SGCore::Net::Server::~Server() noexcept
{
    m_context.stop();
    m_contextThread->join();
}

SGCore::Net::Server::Server() noexcept :
    m_endpoint(m_protocol, m_port)
{
    createContextThread();
}

SGCore::Net::Server::Server(Server&& other) noexcept : m_protocol(other.m_protocol),
    m_port(other.m_port),
    m_endpoint(m_protocol, m_port)
{
    if(other.m_contextThread)
    {
        other.m_contextThread->join();
    }

    if(other.m_socket)
    {
        other.m_socket = std::nullopt;
    }

    m_recvBuffer = other.m_recvBuffer;

    if(m_socket)
    {
        m_socket = std::nullopt;
    }

    m_socket = socket_t(m_context, m_endpoint);

    createContextThread();
}

SGCore::Coro::Task<> SGCore::Net::Server::propagatePacket(const Packet& packet, endpoint_t fromClient) noexcept
{
    std::vector<endpoint_t> connectedClientsVec;
    {
        std::lock_guard guard(m_connectedClientsContainersMutex);
        connectedClientsVec = m_connectedClients;
    }

    for(const auto& client : connectedClientsVec)
    {
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
                    LOG_E(SGCORE_TAG, "Cannot propagate buffer with size {} to client {}", bufferSize, client.address().to_string());
                }
            });
        });

        co_await Coro::returnToCaller();
    }
}

void SGCore::Net::Server::sendPacket(const Packet& packet, endpoint_t toClient) noexcept
{
    auto clientPacket = m_packetsToSend[toClient];
    if(!clientPacket)
    {
        clientPacket = MakeRef<Packet>();
    }

    *clientPacket = packet;

    // capturing client packet to save data
    boost::asio::post(m_strand, [this, clientPacket, toClient] {
        m_socket->async_send_to(boost::asio::buffer(*clientPacket), toClient, [toClient, clientPacket](boost::system::error_code errorCode, size_t bufferSize) {
            if(errorCode)
            {
                LOG_E(SGCORE_TAG, "Cannot send buffer with size {} to client {}", bufferSize, toClient.address().to_string());
            }
        });
    });
}

SGCore::Net::Server& SGCore::Net::Server::operator=(Server&& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    if(other.m_contextThread)
    {
        other.m_contextThread->join();
    }

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
