//
// Created by stuka on 03.08.2025.
//

#include "Client.h"

#include "SGCore/Threading/Task.h"

#include "Utils.h"

SGCore::Net::Client::Client() noexcept
{
    m_stream.m_socket = RUDPStream::socket_t(m_context);
    m_stream.m_socket->open(boost::asio::ip::udp::v4());
    m_stream.m_socket->bind(endpoint_t(boost::asio::ip::make_address("127.0.0.1"), 0));

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
    auto& serverEndpoint = m_stream.m_serverEndpoint;

    serverEndpoint = endpoint_t(boost::asio::ip::make_address(endpointAddress), endpointPort);
    // server always has session with ID equals to 0
    m_stream.registerClient(serverEndpoint, 0);

    m_stream.m_socket->async_connect(serverEndpoint, [this, retriesCount, retryInterval, &serverEndpoint](boost::system::error_code errorCode) -> Coro::Task<> {
        if(retriesCount == 0)
        {
            LOG_E(SGCORE_TAG, "Server {} does not respond.", serverEndpoint.address().to_string());
            setConnected(false);
            co_return;
        }

        if(errorCode)
        {
            LOG_E(SGCORE_TAG, "Cannot connect to server {}: {}. Retrying...", serverEndpoint.address().to_string(), errorCode.message());

            co_await retryInterval;

            connect(serverEndpoint.address().to_string(), serverEndpoint.port(), retryInterval, retriesCount - 1);

            co_return;
        }

        setConnected(true);
        LOG_I(SGCORE_TAG, "Connected to server {}", serverEndpoint.address().to_string());
    });
}

SGCore::Coro::Task<> SGCore::Net::Client::runReceivePoll() noexcept
{
    while(m_contextThread->isRunning())
    {
        if(!m_isConnected)
        {
            co_await Coro::returnToCaller();
            continue;
        }

        m_stream.pollReliableStreams();

        m_stream.receive(m_strand);

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

void SGCore::Net::Client::setConnected(bool connected) noexcept
{
    m_isConnected = connected;

    if(connected && onConnected)
    {
        onConnected();
    }
    else if(!connected && onDisconnected)
    {
        onDisconnected();
    }
}
