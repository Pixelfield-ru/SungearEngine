//
// Created by stuka on 03.08.2025.
//

#include "Client.h"

#include "SGCore/Threading/Task.h"

#include "Utils.h"

SGCore::Net::Client::Client() noexcept
{
    auto contextTask = MakeRef<Threading::Task>();
    contextTask->m_isStatic = true;
    contextTask->setOnExecuteCallback([this]() {
        m_context.run();
    });

    m_contextThread->addTask(contextTask);
    m_contextThread->start();

    m_socket.open(boost::asio::ip::udp::v4());
    m_socket.bind(endpoint_t(boost::asio::ip::make_address("127.0.0.1"), 0));
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

SGCore::Coro::Task<> SGCore::Net::Client::runReceivePoll() noexcept
{
    while(m_contextThread->isRunning())
    {
        if(m_socket.available() == 0 || !m_isConnected.load())
        {
            co_await Coro::returnToCaller();
            continue;
        }

        boost::asio::post(m_strand, [this] {
            m_socket.async_receive_from(boost::asio::buffer(m_recvBuffer), m_recvEndpoint, [this](const boost::system::error_code& error, std::size_t bufferSize) {
                const auto tmpBuf = m_recvBuffer;

                if(m_recvEndpoint != m_serverEndpoint)
                {
                    std::cout << "unknown endpoint: " << m_recvEndpoint << std::endl;
                    // unknown endpoint
                    return;
                }

                if(error)
                {
                    std::cerr << "Client: error while receiving packet: " << error.message() << '\n';
                    return;
                }

                // todo: make cycle for full packet

                std::uint64_t dataTypeHash;
                std::memcpy(&dataTypeHash, tmpBuf.data(), sizeof(dataTypeHash));

                const auto dataStreamIt = m_registeredDataStreams.find(dataTypeHash);

                if(dataStreamIt == m_registeredDataStreams.end())
                {
                    // invalid data type or incomplete buffer
                    std::cout << "invalid data type: " << dataTypeHash << std::endl;
                    return;
                }

                size_t metaDataSize = sizeof(dataTypeHash);

                // reading from client endpoint =======
                size_t fromClientEndpointSize = 0;
                const endpoint_t fromClient = Utils::readEndpoint(tmpBuf, metaDataSize, fromClientEndpointSize);

                metaDataSize += fromClientEndpointSize;
                // ====================================

                Packet pureData;

                std::memcpy(pureData.data(), tmpBuf.data() + metaDataSize, bufferSize - metaDataSize);

                // m_recvBuffer = { };

                dataStreamIt->second.onReceive(pureData, fromClient);
            });
        });

        co_await Coro::returnToCaller();
    }
}

bool SGCore::Net::Client::isConnected() const noexcept
{
    return m_isConnected.load();
}
