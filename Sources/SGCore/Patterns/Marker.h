#ifndef SUNGEARENGINE_MARKER_H
#define SUNGEARENGINE_MARKER_H

#include <cstdint>
#include <iostream>
#include <memory>

namespace Core::Patterns
{
    template<typename T>
    class Marker : public std::enable_shared_from_this<T>
    {
    public:
        bool isFlagSet(const std::uint16_t& flag) const noexcept
        {
            // 0x000 000 11
            // 0x000 000 10
            // 0x000 000 10 != 0

            return (m_flags & flag) != 0;
        }

        std::shared_ptr<T> addFlag(const std::uint16_t& flag) noexcept
        {
            m_flags |= flag;

            // 0x000 000 00
            // 0x000 000 10

            // 0x000 000 10

            // 0x000 000 10
            // 0x000 000 01

            // 0x000 000 11

            return this->shared_from_this();
        }

        std::shared_ptr<T> removeFlag(const std::uint16_t& flag) noexcept
        {
            m_flags &= ~flag;
            // 0x000 000 11

            // 0x000 000 10

            // 0x111 111 01
            // 0x000 000 01

            // 0x000 000 01
            // 0x111 111 10
            // 0x000 000 00

            return this->shared_from_this();
        }

        std::shared_ptr<T> resetFlags() noexcept
        {
            m_flags = 0;

            return this->shared_from_this();
        }

    protected:
        std::uint16_t m_flags = 0;
    };
}

#endif //SUNGEARENGINE_MARKER_H
