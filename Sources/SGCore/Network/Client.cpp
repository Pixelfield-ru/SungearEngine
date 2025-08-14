//
// Created by stuka on 03.08.2025.
//

#include "Client.h"

#include <SGCore/Threading/Task.h>

SGCore::Net::Client::Client() noexcept
{
    auto contextTask = MakeRef<Threading::Task>();
    contextTask->m_isStatic = true;
    contextTask->setOnExecuteCallback([this]() {
        m_context.run();
    });

    m_contextThread->addTask(contextTask);
    m_contextThread->start();

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    const auto port = 1024 + (std::rand() % (49151 - 1024 + 1));
    std::cout << "client port: " << port << std::endl;

    m_socket.open(boost::asio::ip::udp::v4());
    m_socket.bind(endpoint_t(boost::asio::ip::make_address("127.0.0.1"), port));
}

void SGCore::Net::Client::connect(const std::string& endpointAddress,
                             boost::asio::ip::port_type endpointPort,
                             std::chrono::system_clock::duration retryInterval,
                             int retriesCount) noexcept
{
    m_serverEndpoint = endpoint_t(boost::asio::ip::make_address(endpointAddress), endpointPort);

    m_socket.async_connect(m_serverEndpoint, [this, retriesCount, retryInterval](boost::system::error_code errorCode) -> Coro::Task<> {
        if(retriesCount == 0)
        {
            std::cout << "server does not respond" << std::endl;
            m_isConnected = false;
            co_return;
        }

        if(errorCode)
        {
            std::cout << "connection failed: " << errorCode.message() << ". retrying..." << std::endl;

            co_await retryInterval;

            connect(m_serverEndpoint.address().to_string(), m_serverEndpoint.port(), retryInterval, retriesCount - 1);

            co_return;
        }

        m_isConnected = true;
        std::cout << "connection established" << std::endl;
    });
}

SGCore::Coro::Task<> SGCore::Net::Client::send(Packet packet) noexcept
{
    m_packetsQueue.push_back(MakeRef<Packet>(packet));

    size_t packetIdx = m_packetsQueue.size() - 1;

    while(!m_isConnected)
    {
        co_await Coro::returnToCaller();
    }

    m_socket.async_send(boost::asio::buffer(*m_packetsQueue[packetIdx]), [this, packetIdx](boost::system::error_code errorCode, size_t bytesCnt) {
        if(errorCode)
        {
            std::cout << "send failed: " << errorCode.message() << ". count of bytes to send: " << bytesCnt << std::endl;
            return;
        }

        m_sentPackets.push_back(packetIdx);

        // std::cout << "sent to server " << bytesCnt << " bytes" << std::endl;
    });
}

bool SGCore::Net::Client::isConnected() const noexcept
{
    return m_isConnected.load();
}
