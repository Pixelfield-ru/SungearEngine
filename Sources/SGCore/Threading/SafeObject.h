//
// Created by Ilya on 04.03.2024.
//

#ifndef SUNGEARENGINE_SAFEOBJECT_H
#define SUNGEARENGINE_SAFEOBJECT_H

#include <SGCore/pch.h>

#include "SGUtils/Event.h"
#include "SGUtils/EventListener.h"

namespace SGCore
{
    template<typename ObjT>
    struct SafeObject
    {
        Event<void()> onUnlock;
        
        inline void lock() noexcept
        {
            m_currentLocksCount = m_subscribersCount;
            m_isLocked.store(true);
        }

        inline void unlock() noexcept
        {
            --m_currentLocksCount;
            if(m_currentLocksCount <= 0)
            {
                m_isLocked.store(false);
                m_currentLocksCount = 0;
            }
        }
        
        inline ObjT& getObject() noexcept
        {
            return m_obj;
        }

        [[nodiscard]] inline bool isLocked() const noexcept
        {
            return m_isLocked.load();
        }
        
        inline void subscribe() noexcept
        {
            if(isLocked())
            {
                ++m_currentLocksCount;
            }
            ++m_subscribersCount;
        }
        
        inline void unSubscribe() noexcept
        {
            if(isLocked())
            {
                --m_currentLocksCount;
                if(m_currentLocksCount <= 0)
                {
                    unlock();
                }
            }
            --m_subscribersCount;
        }
        
        inline std::int64_t getSubscribersCount() noexcept
        {
            return m_subscribersCount;
        }
        
    private:
        std::atomic<bool> m_isLocked;
        std::int64_t m_subscribersCount = 0;
        std::int64_t m_currentLocksCount = 0;
        
        ObjT m_obj;
    };
}

#endif // SUNGEARENGINE_SAFEOBJECT_H
