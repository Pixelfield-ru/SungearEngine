//
// Created by stuka on 02.08.2025.
//

#ifndef SUNGEARENGINE_NETWORK_SERVER_H
#define SUNGEARENGINE_NETWORK_SERVER_H

#include "Packet.h"

#include "../Utils/Platform.h"

#ifdef SG_PLATFORM_OS_WINDOWS
#include <WinSock2.h>
#endif

#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/udp.hpp>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Threading/ThreadsPool.h"

#include "SGCore/Coro/Task.h"

#include "ClientDisconnectedPacket.h"

#include "Utils.h"

namespace SGCore::Net
{
    struct Server
    {
        using socket_t = boost::asio::ip::udp::socket;
        using endpoint_t = boost::asio::ip::udp::endpoint;

        Server(boost::asio::ip::udp protocol, boost::asio::ip::port_type port);
        Server() noexcept;
        Server(Server&& other) noexcept;

        template<typename T>
        requires(std::is_invocable_v<T, const Packet&, size_t, endpoint_t>)
        Coro::Task<> runReceivePoll(T callback) noexcept
        {
            while(m_contextThread->isRunning())
            {
                if(!m_socket || m_socket->available() == 0)
                {
                    co_await Coro::returnToCaller();
                    continue;
                }

                boost::asio::post(m_strand, [this, callback] {
                    m_socket->async_receive_from(boost::asio::buffer(m_recvBuffer), m_receivedFromEndpoint, [this, callback](const boost::system::error_code& error, std::size_t bufferSize) {
                        const auto clientEndpoint = m_receivedFromEndpoint;
                        auto tmpBuf = m_recvBuffer;
                        const auto originalData = m_recvBuffer;

                        if(error)
                        {
                            std::cout << "error while reading data from client: " << clientEndpoint << ". error is: " << error.what() << std::endl;

                            {
                                std::lock_guard guard(m_connectedClientsContainersMutex);
                                m_connectedClients.erase(std::ranges::find(m_connectedClients, m_receivedFromEndpoint));
                                m_connectedClientsSet.erase(m_receivedFromEndpoint);
                            }

                            tmpBuf = {};

                            const std::uint64_t dataTypeHash = SGCore::hashString(ClientDisconnectedPacket::type_name);

                            size_t clientEndpointSize = 0;

                            std::memcpy(tmpBuf.data(), &dataTypeHash, sizeof(dataTypeHash));
                            Utils::writeEndpoint(tmpBuf, sizeof(dataTypeHash), clientEndpoint, clientEndpointSize);

                            // we will definitely notify other clients about the client disconnection.
                            propagatePacket(tmpBuf, clientEndpoint);

                            return;
                        }

                        if(!m_connectedClientsSet.contains(clientEndpoint))
                        {
                            {
                                std::lock_guard guard(m_connectedClientsContainersMutex);
                                m_connectedClientsSet.insert(clientEndpoint);
                                m_connectedClients.push_back(clientEndpoint);
                            }

                            std::cout << "new client: " << clientEndpoint << ", clients count: " << m_connectedClients.size() << std::endl;
                        }

                        size_t formedPacketOffset = 0;
                        size_t originalPacketOffset = 0;

                        // processing all packet
                        while(true)
                        {
                            std::uint64_t dataTypeHash;
                            std::memcpy(&dataTypeHash, tmpBuf.data() + formedPacketOffset, sizeof(dataTypeHash));

                            auto registeredTypeIt = m_registeredTypes.find(dataTypeHash);
                            if(registeredTypeIt == m_registeredTypes.end())
                            {
                                std::cout << "invalid data type: " << dataTypeHash << std::endl;

                                // todo: maybe += 1 byte and continue to trying to find valid data??
                                // invalid data type data or incomplete buffer
                                return;
                            }

                            const auto registeredTypeSize = registeredTypeIt->second;

                            formedPacketOffset += sizeof(dataTypeHash);

                            // writing client endpoint ====================================
                            size_t clientEndpointSize = 0;
                            bool clientEndpointWriteSuccess = Utils::writeEndpoint(tmpBuf, formedPacketOffset, clientEndpoint, clientEndpointSize);
                            formedPacketOffset += clientEndpointSize;
                            // ============================================================

                            if(!clientEndpointWriteSuccess ||
                               formedPacketOffset + registeredTypeSize > tmpBuf.size() ||
                               originalPacketOffset + sizeof(dataTypeHash) + registeredTypeSize > originalData.size())
                            {
                                break;
                            }

                            std::memcpy(tmpBuf.data() + formedPacketOffset, originalData.data() + originalPacketOffset + sizeof(dataTypeHash), registeredTypeSize);

                            if(bufferSize > 0)
                            {
                                callback(tmpBuf, bufferSize, clientEndpoint);
                            }

                            formedPacketOffset += registeredTypeSize;
                            originalPacketOffset += sizeof(dataTypeHash) + registeredTypeSize;
                        }
                    });
                });

                co_await Coro::returnToCaller();
            }
        }

        template<typename T>
        void registerDataType() noexcept
        {
            m_registeredTypes[SGCore::hashString(T::type_name)] = sizeof(T);
        }

        Coro::Task<> propagatePacket(const Packet& packet, endpoint_t fromClient) noexcept;

        Server& operator=(Server&& other) noexcept;

    private:
        Ref<Threading::Thread> m_contextThread;

        boost::asio::ip::udp m_protocol = boost::asio::ip::udp::v4();
        boost::asio::ip::port_type m_port = 2025;
        boost::asio::io_context m_context;
        boost::asio::strand<decltype(m_context)::executor_type> m_strand = boost::asio::make_strand(m_context);

        endpoint_t m_endpoint = endpoint_t(m_protocol, m_port);

        std::optional<socket_t> m_socket = std::nullopt;

        endpoint_t m_receivedFromEndpoint;

        std::unordered_set<endpoint_t> m_connectedClientsSet;
        std::vector<endpoint_t> m_connectedClients;
        std::mutex m_connectedClientsContainersMutex;

        Packet m_recvBuffer;
        std::unordered_map<endpoint_t, Ref<Packet>> m_packetsToSend;

        // first - data type id, second - sizeof data
        std::unordered_map<std::uint64_t, std::uint64_t> m_registeredTypes;
        // std::unordered_map<endpoint_t, std::vector<Ref<Packet>>> m_sentPackets;
    };
}

#endif // SUNGEARENGINE_NETWORK_SERVER_H
