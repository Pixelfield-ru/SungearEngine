//
// Created by stuka on 03.08.2025.
//

#ifndef SUNGEARENGINE_NETWORK_PACKET_H
#define SUNGEARENGINE_NETWORK_PACKET_H

#include <cstdint>
#include <vector>

namespace SGCore::Net
{
    // using Packet = std::vector<std::uint8_t>;
    using Packet = std::array<std::uint8_t, 1024>;
    /*struct Packet
    {
        friend struct Client;

        std::array<std::uint8_t, 512> m_data { };
        std::int64_t m_size { };

        auto begin() const noexcept
        {
            return m_data.begin() + (m_size - m_dataRemaining);
        }

        auto begin() noexcept
        {
            return m_data.begin() + (m_size - m_dataRemaining);
        }

        auto end() const noexcept
        {
            return m_data.begin() + m_size;
        }

        auto end() noexcept
        {
            return m_data.begin() + m_size;
        }

        std::int64_t size() const noexcept
        {
            return m_dataRemaining;
        }

    private:
        std::int64_t m_dataRemaining = m_size;
    };*/
}

#endif // SUNGEARENGINE_NETWORK_PACKET_H
