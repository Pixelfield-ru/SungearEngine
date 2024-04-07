#ifndef SUNGEARENGINE_EVENTLISTENER_H
#define SUNGEARENGINE_EVENTLISTENER_H

#include <list>
#include <functional>
#include <memory>

namespace SGCore
{
    template<typename Return>
    struct Event;

    template<typename Return>
    struct EventListener;

    template<typename Return, typename... Args>
    struct EventListener<Return(Args...)>
    {
        friend struct Event<Return(Args...)>;

        size_t m_priority = 0;
        size_t m_hash = reinterpret_cast<std::intptr_t>(this);

        EventListener() = default;

        EventListener(const std::function<Return(Args&&...)>& func)
        {
            m_func = func;
        }

        template<typename Func>
        EventListener(Func&& func)
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

    private:
        std::function<void(Args...)> m_func;
        std::function<void(EventListener*)> m_unsubscribeFunc;
        std::list<Event<Return(Args...)>*> m_listeningEvents;
        bool m_isOwnedByEvent = false;
    };
}

#endif //SUNGEARENGINE_EVENTLISTENER_H
