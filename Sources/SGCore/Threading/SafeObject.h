//
// Created by Ilya on 04.03.2024.
//

#ifndef SUNGEARENGINE_SAFEOBJECT_H
#define SUNGEARENGINE_SAFEOBJECT_H

#include <atomic>
#include <vector>

namespace SGCore
{
    template<typename ObjT>
    struct SafeObject
    {
        inline void lock() noexcept
        {
            m_isLocked.store(true);
        }

        inline void unlock() noexcept
        {
            m_isLocked.store(false);
        }
        
        inline ObjT& getObject() noexcept
        {
            return m_obj;
        }

        [[nodiscard]] inline bool isLocked() const noexcept
        {
            return m_isLocked.load();
        }

    private:
        std::atomic<bool> m_isLocked;
        
        ObjT m_obj;
    };
}

#endif // SUNGEARENGINE_SAFEOBJECT_H
