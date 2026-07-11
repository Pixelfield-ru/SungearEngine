//
// Created by stuka on 03.08.2025.
//

#include "Client.h"

#include "SGCore/Threading/Task.h"

#include "Utils.h"

SGCore::Net::Client::Client() noexcept
{
    m_socket.open(boost::asio::ip::udp::v4());
    m_socket.bind(endpoint_t(boost::asio::ip::make_address("127.0.0.1"), 0));

    createContextThread();
}

SGCore::Net::Client::~Client() noexcept
{
    m_context.stop();
    m_contextThread->join();
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
            LOG_E(SGCORE_TAG, "Server {} does not respond.", m_serverEndpoint.address().to_string());
            m_isConnected = false;
            co_return;
        }

        if(errorCode)
        {
            LOG_E(SGCORE_TAG, "Cannot connect to server {}: {}. Retrying...", m_serverEndpoint.address().to_string(), errorCode.message());

            co_await retryInterval;

            connect(m_serverEndpoint.address().to_string(), m_serverEndpoint.port(), retryInterval, retriesCount - 1);

            co_return;
        }

        m_isConnected = true;
        LOG_I(SGCORE_TAG, "Connected to server {}", m_serverEndpoint.address().to_string());
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
                const auto originalBuffer = m_recvBuffer;

                if(m_recvEndpoint != m_serverEndpoint)
                {
                    LOG_E(SGCORE_TAG, "Unknown endpoint: {}", m_recvEndpoint.address().to_string());
                    return;
                }

                if(error)
                {
                    LOG_E(SGCORE_TAG, "Error while receiving packet: {}", error.message());
                    return;
                }

                size_t currentPacketOffset = 0;

                // processing all packet
                while(true)
                {
                    std::uint64_t dataTypeHash;
                    std::memcpy(&dataTypeHash, originalBuffer.data() + currentPacketOffset, sizeof(dataTypeHash));

                    const auto dataStreamIt = m_registeredDataStreams.find(dataTypeHash);

                    if(dataStreamIt == m_registeredDataStreams.end())
                    {
                        // std::cout << "invalid data type: " << dataTypeHash << std::endl;

                        // todo: maybe += 1 byte and continue to trying to find valid data??
                        // invalid data type or incomplete buffer
                        ++currentPacketOffset;
                        if(currentPacketOffset >= originalBuffer.size()) break;

                        continue;
                        // break;
                    }

                    auto& dataStream = dataStreamIt->second;

                    const auto registeredTypeSize = dataStream.m_dataSize;

                    currentPacketOffset += sizeof(dataTypeHash);

                    // reading from client endpoint =======
                    size_t fromClientEndpointSize = 0;
                    bool isReadClientEndpointSuccessful = false;
                    const endpoint_t fromClient = Utils::readEndpoint(originalBuffer, currentPacketOffset, fromClientEndpointSize, isReadClientEndpointSuccessful);
                    currentPacketOffset += fromClientEndpointSize;
                    // ====================================

                    if(!isReadClientEndpointSuccessful ||
                       currentPacketOffset + registeredTypeSize > originalBuffer.size())
                    {
                        break;
                    }

                    Packet pureData;

                    std::memcpy(pureData.data(), originalBuffer.data() + currentPacketOffset, registeredTypeSize);

                    dataStream.onReceive(pureData, fromClient);

                    currentPacketOffset += registeredTypeSize;
                }
            });
        });

        co_await Coro::returnToCaller();
    }
}

bool SGCore::Net::Client::isConnected() const noexcept
{
    return m_isConnected.load();
}

void SGCore::Net::Client::createContextThread() noexcept
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
