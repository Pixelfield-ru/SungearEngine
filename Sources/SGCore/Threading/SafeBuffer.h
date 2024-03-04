//
// Created by Ilya on 04.03.2024.
//

#ifndef SUNGEARENGINE_SAFEBUFFER_H
#define SUNGEARENGINE_SAFEBUFFER_H

#include <atomic>
#include <vector>

namespace SGCore
{
    template<typename T>
    struct SafeBuffer
    {
        inline void lock() noexcept
        {
            m_isLocked.store(true);
        }

        inline void unlock() noexcept
        {
            m_isLocked.store(false);
        }

        inline void clearBuffer() noexcept
        {
            m_buffer.clear();
        }

        inline std::vector<T>& getBuffer() noexcept
        {
            return m_buffer;
        }

        inline bool isLocked() const noexcept
        {
            return m_isLocked.load();
        }

    private:
        std::atomic<bool> m_isLocked;

        std::vector<T> m_buffer;
    };
}

#endif // SUNGEARENGINE_SAFEBUFFER_H
