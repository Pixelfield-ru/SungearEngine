//
// Created by stuka on 19.08.2025.
//

#ifndef SUNGEARENGINE_NETWORK_CLIENTDISCONNECTEDPACKET_H
#define SUNGEARENGINE_NETWORK_CLIENTDISCONNECTEDPACKET_H

namespace SGCore::Net
{
    struct ClientDisconnectedPacket
    {
        static inline std::string type_name = "SGCore::Net::ClientDisconnectedPacket";
    };
}

#endif // SUNGEARENGINE_NETWORK_CLIENTDISCONNECTEDPACKET_H
