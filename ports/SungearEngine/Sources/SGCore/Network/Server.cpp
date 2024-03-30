// TODO: make for linux and mac

#include <spdlog/spdlog.h>
#include "Server.h"

#include <iostream>
#include "SGUtils/Utils.h"

SGCore::Server::Server
(const SGCore::ConnectionType& connectionType, const std::string& ip, const std::uint16_t& port) noexcept
{
    m_connectionType = connectionType;

    #if defined(WIN32) || defined(WIN64)

    // | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
    // 514
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA WSAData;

    if(WSAStartup(sockVersion, &WSAData) != 0)
    {
        spdlog::error("Can not initialize WSA for server. Error: {0}\n{1}", WSAGetLastError(), SG_CURRENT_LOCATION_STR);
        return;
    }
    else
    {
        spdlog::info("Server WSA initialized!");
    }

    // AF_INET is IPv4
    // SOCK_DGRAM - UDP SOCKET
    // SOCK_STREAM - TCP SOCKET
    m_socket = socket(AF_INET, m_connectionType == ConnectionType::SGCT_TCP ? SOCK_STREAM : SOCK_DGRAM, 0);

    if(m_socket == INVALID_SOCKET)
    {
        spdlog::error("Can not initialize socket. Error: {0}\n{1}", WSAGetLastError(), SG_CURRENT_LOCATION_STR);

        closesocket(m_socket);
        WSACleanup();

        return;
    }



    // TODO: to finish

    #endif
}
