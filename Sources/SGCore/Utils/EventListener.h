#ifndef SUNGEARENGINE_EVENTLISTENER_H
#define SUNGEARENGINE_EVENTLISTENER_H

#include <SGCore/pch.h>

namespace SGCore
{
    namespace Threading
    {
        struct Task;
    }

    template<typename Return>
    struct Event;
    
    template<typename Return>
    struct EventListener;
    
    template<typename Return, typename... Args>
    struct EventListener<Return(Args...)>
    {
        friend struct Event<Return(Args...)>;
        friend struct Threading::Task;
        
        size_t m_priority = 0;
        
        EventListener() = default;
        
        EventListener(const std::function<Return(Args&&...)>& func)
        {
            m_func = func;
        }
        
        template<typename Func>
        requires(std::is_invocable_r_v<Return, Func, Args...>)
        EventListener(const Func& func)
        {
            m_func = func;
        }
        
        EventListener(const EventListener& e) = default;
        EventListener(EventListener&&) noexcept = default;
        
        ~EventListener()
        {
            if(m_unsubscribeFunc)
            {
                m_unsubscribeFunc(this);
            }
        }

        EventListener& operator=(const EventListener& other) noexcept
        {
            if(m_copyToEventsFunc)
            {
                m_copyToEventsFunc(&other, this);
            }

            m_func = other.m_func;
        }

        EventListener& operator=(EventListener&&) noexcept = default;

        EventListener& operator=(const std::function<Return(Args&&...)>& func) noexcept
        {
            m_func = func;
            
            return *this;
        }
        
        template<typename Func>
        EventListener& operator=(Func&& func) noexcept
        {
            m_func = func;
            
            return *this;
        }
        
        template<typename... Args0>
        void operator()(Args0&&... args) const noexcept
        {
            if(m_func)
            {
                m_func(std::forward<Args0>(args)...);
            }
        }

        [[nodiscard]] size_t getHash() const noexcept
        {
            return m_hash;
        }
    
    private:
        std::function<void(Args...)> m_func;
        std::function<void(EventListener*)> m_unsubscribeFunc;
        std::function<void(const EventListener* from, EventListener* to)> m_copyToEventsFunc;
        std::list<Event<Return(Args...)>*> m_listeningEvents;
        bool m_isOwnedByEvent = false;

        size_t m_hash = reinterpret_cast<std::intptr_t>(this);
    };
}


#endif //SUNGEARENGINE_EVENTLISTENER_H
