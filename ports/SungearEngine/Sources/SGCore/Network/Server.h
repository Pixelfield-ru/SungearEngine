//
// Created by stuka on 03.07.2023.
//

#ifndef SUNGEARENGINE_SERVER_H
#define SUNGEARENGINE_SERVER_H

#include <string>
#include <cstdint>

#include "ConnectionType.h"

#if defined(WIN32) || defined(WIN64)

#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

#endif

// TODO: MAKE MULTITHREADED
namespace SGCore
{
    class Server
    {
    private:
        #if defined(WIN32) || defined(WIN64)

        SOCKET m_socket = -1;
        //
        sockaddr_in m_socketInfo;

        #endif

        ConnectionType m_connectionType = SGCT_TCP;

        std::string m_ip;
        std::uint16_t m_port;

    public:
        explicit Server(const ConnectionType& connectionType, const std::string& ip, const std::uint16_t& port) noexcept;
    };
}

#endif //SUNGEARENGINE_SERVER_H
