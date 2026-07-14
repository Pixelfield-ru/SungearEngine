//
// Created by stuka on 15.07.2026.
//

#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>

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

        std::optional<socket_t> m_socket = std::nullopt;

        endpoint_t m_receiveEndpoint;
        endpoint_t m_serverEndpoint;

        template<typename DataT>
        DataType& registerDataType() noexcept
        {
            auto& data = m_registeredDataTypes[DataT::getTypeIDStatic()];
            data.m_dataSize = sizeof(DataT);

            return data;
        }

    protected:
        Packet m_recvBuffer {};

        std::unordered_map<std::uint64_t, DataType> m_registeredDataTypes;
        std::unordered_map<std::int64_t, endpoint_t> m_registeredClients;
    };
}
