// TODO: make for linux and mac

#include "Server.h"

#include <iostream>
#include "SGCore/Logging/Log.h"

SGCore::Network::Server::Server
(const SGCore::Network::ConnectionType& connectionType, const std::string& ip, const std::uint16_t& port) noexcept
{
    m_connectionType = connectionType;

    #if defined(WIN32) || defined(WIN64)

    // | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
    // 514
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA WSAData;

    if(WSAStartup(sockVersion, &WSAData) != 0)
    {
        SGCF_ERROR("Can not initialize WSA for server. Error: " + std::to_string(WSAGetLastError()), SG_LOG_CURRENT_SESSION_FILE);
        return;
    }
    else
    {
        SGCF_SUCCESS("Server WSA initialized!", SG_LOG_CURRENT_SESSION_FILE);
    }

    // AF_INET is IPv4
    // SOCK_DGRAM - UDP SOCKET
    // SOCK_STREAM - TCP SOCKET
    m_socket = socket(AF_INET, m_connectionType == ConnectionType::SGCT_TCP ? SOCK_STREAM : SOCK_DGRAM, 0);

    if(m_socket == INVALID_SOCKET)
    {
        SGCF_ERROR("Can not initialize socket. Error: " + std::to_string(WSAGetLastError()), SG_LOG_CURRENT_SESSION_FILE);

        closesocket(m_socket);
        WSACleanup();

        return;
    }



    // TODO: to finish

    #endif
}
