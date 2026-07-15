//
// Created by stuka on 15.07.2026.
//

#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <mutex>

#include "DataType.h"
#include "Packet.h"
#include "SGCore/Logger/Logger.h"
#include "Utils.h"

#include "sgcore_export.h"

namespace SGCore::Net
{
    template<typename ProtocolT>
    struct SGCORE_EXPORT IStream
    {
        using socket_t = ProtocolT::socket;
        using endpoint_t = ProtocolT::endpoint;
        using strand_t = boost::asio::strand<boost::asio::io_context::executor_type>;

        /// Current device session ID. By default equals to -1.
        std::atomic<std::int64_t> m_sessionID = -1;

        /// Current device socket.
        std::optional<socket_t> m_socket = std::nullopt;

        /// Server endpoint to connect to.
        endpoint_t m_serverEndpoint;

        template<typename DataT>
        DataType& registerDataType() noexcept
        {
            std::lock_guard lock(m_dataAccessMutex);

            auto& data = m_registeredDataTypes[DataT::getTypeIDStatic()];
            data.m_dataSize = sizeof(DataT);

            return data;
        }

        void registerClient(const endpoint_t& clientEndpoint, std::int64_t clientSessionID) noexcept
        {
            std::lock_guard lock(m_dataAccessMutex);
            m_registeredClients[clientSessionID] = clientEndpoint;
        }

        bool isClientRegistered(std::int64_t clientSessionID) const noexcept
        {
            std::lock_guard lock(m_dataAccessMutex);
            return m_registeredClients.contains(clientSessionID);
        }

        [[nodiscard]] auto getRegisteredClients() const noexcept
        {
            std::lock_guard lock(m_dataAccessMutex);
            return m_registeredClients;
        }

    protected:
        endpoint_t m_receiveEndpoint;
        Packet m_recvBuffer {};

        mutable std::mutex m_dataAccessMutex;

        std::unordered_map<std::uint64_t, DataType> m_registeredDataTypes;
        std::unordered_map<std::int64_t, endpoint_t> m_registeredClients;
    };
}
