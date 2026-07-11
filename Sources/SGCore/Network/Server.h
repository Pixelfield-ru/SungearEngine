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

namespace SGCore::Net
{
    struct SGCORE_EXPORT Server
    {
        using socket_t = boost::asio::ip::udp::socket;
        using endpoint_t = boost::asio::ip::udp::endpoint;

        Server(boost::asio::ip::udp protocol, boost::asio::ip::port_type port);
        ~Server() noexcept;
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

                boost::asio::post(m_strand, [this, callback = std::move(callback)] {
                    m_socket->async_receive_from(boost::asio::buffer(m_recvBuffer), m_receivedFromEndpoint, [this, callback = std::move(callback)](const boost::system::error_code& error, std::size_t bufferSize) {
                        const auto clientEndpoint = m_receivedFromEndpoint;
                        // auto tmpBuf = m_recvBuffer;
                        Packet tmpBuf {};
                        const auto originalBuffer = m_recvBuffer;

                        if(error)
                        {
                            LOG_E(SGCORE_TAG, "Cannot read data from client {}. Error is: {}", clientEndpoint.address().to_string(), error.what());

                            {
                                std::lock_guard guard(m_connectedClientsContainersMutex);
                                m_connectedClients.erase(std::ranges::find(m_connectedClients, clientEndpoint));
                                m_connectedClientsSet.erase(clientEndpoint);
                            }

                            tmpBuf = {};

                            Utils::writeMessage<ClientDisconnectedMessage>(tmpBuf, clientEndpoint);

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

                            LOG_E(SGCORE_TAG, "New client {} connected. Clients count: {}", clientEndpoint.address().to_string(), m_connectedClients.size());

                            Packet outPacket;
                            Utils::writeMessage<ClientConnectedMessage>(outPacket, clientEndpoint);

                            propagatePacket(outPacket, clientEndpoint);
                        }

                        size_t formedPacketOffset = 0;
                        size_t originalPacketOffset = 0;

                        // todo: fix data receiving or data sending or something like that idk
                        // processing all packet. adding client endpoint
                        while(true)
                        {
                            // std::cout << "originalPacketOffset: " << originalPacketOffset << std::endl;
                            std::uint64_t dataTypeHash;
                            std::memcpy(&dataTypeHash, originalBuffer.data() + originalPacketOffset, sizeof(dataTypeHash));

                            auto registeredTypeIt = m_registeredTypes.find(dataTypeHash);
                            if(registeredTypeIt == m_registeredTypes.end())
                            {
                                // std::cout << "invalid data type: " << dataTypeHash << std::endl;

                                // todo: maybe += 1 byte and continue to trying to find valid data??
                                // invalid data type data or incomplete buffer
                                ++originalPacketOffset;
                                if(originalPacketOffset >= originalBuffer.size()) break;

                                continue;
                                // break;
                            }

                            std::cout << "normal data: " << originalPacketOffset << std::endl;

                            const auto registeredTypeSize = registeredTypeIt->second;

                            formedPacketOffset += sizeof(dataTypeHash);

                            // writing client endpoint ====================================
                            size_t clientEndpointSize = 0;
                            const bool clientEndpointWriteSuccess = Utils::writeEndpoint(tmpBuf, formedPacketOffset, clientEndpoint, clientEndpointSize);
                            formedPacketOffset += clientEndpointSize;
                            // ============================================================

                            if(!clientEndpointWriteSuccess ||
                               formedPacketOffset + registeredTypeSize > tmpBuf.size() ||
                               originalPacketOffset + sizeof(dataTypeHash) + registeredTypeSize > originalBuffer.size())
                            {
                                break;
                            }

                            std::memcpy(tmpBuf.data() + formedPacketOffset, originalBuffer.data() + originalPacketOffset + sizeof(dataTypeHash), registeredTypeSize);

                            formedPacketOffset += registeredTypeSize;
                            originalPacketOffset += sizeof(dataTypeHash) + registeredTypeSize;
                        }

                        if(bufferSize > 0)
                        {
                            callback(tmpBuf, bufferSize, clientEndpoint);
                        }
                    });
                });

                co_await Coro::returnToCaller();
            }
        }

        template<typename T>
        void registerDataType() noexcept
        {
            m_registeredTypes[T::getTypeIDStatic()] = sizeof(T);
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

        void createContextThread() noexcept;
    };
}
