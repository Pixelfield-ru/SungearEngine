//
// Created by stuka on 30.07.2023.
//

#ifndef SUNGEARENGINE_MARKER_H
#define SUNGEARENGINE_MARKER_H

#include <cstdint>

namespace Core::Patterns
{
    class Marker
    {
    public:
        bool isFlagSet(const std::uint16_t& flag) const noexcept
        {
            return (m_flags & flag) != 0;
        }

        void addFlag(const std::uint16_t& flag) noexcept
        {
            m_flags |= flag;
        }

        void removeFlag(const std::uint16_t& flag) noexcept
        {
            m_flags &= ~flag;
        }

        void resetFlags() noexcept
        {
            m_flags = 0;
        }

    protected:
        std::uint16_t m_flags = 0;
    };
}

#endif //SUNGEARENGINE_MARKER_H
