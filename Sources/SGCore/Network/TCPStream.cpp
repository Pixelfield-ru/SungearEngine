//
// Created by stuka on 13.07.2026.
//

#include "TCPStream.h"

void SGCore::Net::TCPStream::sendPacket(strand_t& strand, const Packet& packet, endpoint_t toClient) noexcept
{

}

void SGCore::Net::TCPStream::receive(strand_t& strand) noexcept
{
    boost::asio::post(strand, [this, callback = std::move(callback)] {
        m_socket->async_receive(boost::asio::buffer(m_recvBuffer), [this, callback = std::move(callback)](const boost::system::error_code& error, std::size_t bufferSize) {
            // m_socket->remote_endpoint();
        });
    });
}
