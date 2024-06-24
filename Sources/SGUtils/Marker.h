#ifndef SUNGEARENGINE_MARKER_H
#define SUNGEARENGINE_MARKER_H

#include <SGUtils/pch.h>

namespace SGCore
{
    template<typename IntT>
    requires(std::is_integral_v<IntT>)
    class Marker
    {
    public:
        virtual bool isFlagSet(const IntT& flag) const noexcept
        {
            // 000 000 11
            // 000 000 10
            // 000 000 10 != 0

            return (m_flags & flag) != 0;
        }

        virtual void addFlag(const IntT& flag) noexcept
        {
            m_flags |= flag;

            // 000 000 00
            // 000 000 10

            // 000 000 10

            // 000 000 10
            // 000 000 01

            // 000 000 11
        }

        virtual void removeFlag(const IntT& flag) noexcept
        {
            m_flags &= ~flag;
            // 000 000 11

            // 000 000 10

            // 111 111 01
            // 000 000 01

            // 000 000 01
            // 111 111 10
            // 000 000 00
        }

        virtual void resetFlags() noexcept
        {
            m_flags = 0;
        }
        
        IntT m_flags = 0;
    };
}

#endif //SUNGEARENGINE_MARKER_H
