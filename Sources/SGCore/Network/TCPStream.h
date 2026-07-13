//
// Created by stuka on 13.07.2026.
//

#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>

#include "Packet.h"
#include "DataType.h"

#include "sgcore_export.h"

namespace SGCore::Net
{
    // todo: сделать
    struct TCPStream
    {
        using socket_t = boost::asio::ip::tcp::socket;
        using endpoint_t = boost::asio::ip::tcp::endpoint;
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

        void sendPacket(strand_t& strand, const Packet& packet, endpoint_t toClient) noexcept;
        void receive(strand_t& strand) noexcept;

    private:
        Packet m_recvBuffer {};

        std::unordered_map<std::uint64_t, DataType> m_registeredDataTypes;
    };
}
