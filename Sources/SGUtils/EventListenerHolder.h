//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_EVENTLISTENERHOLDER_H
#define SUNGEARENGINE_EVENTLISTENERHOLDER_H

#include <list>
#include <functional>
#include <memory>

#include "UUID.h"

namespace SGUtils
{
    template<typename Signature>
    struct Event;

    template <typename Signature>
    struct EventListenerHolder;

    template<typename... Args>
    struct EventListenerHolder<void(Args...)>
    {
        friend struct Event<void(Args...)>;

        EventListenerHolder() = default;
        EventListenerHolder(const std::function<void(Args...)>& func)
        {
            m_func = func;
        }
        EventListenerHolder(const EventListenerHolder& e) = default;
        EventListenerHolder(EventListenerHolder&&) noexcept = default;

        ~EventListenerHolder()
        {
            if(m_unsubscribeFunc)
            {
                m_unsubscribeFunc();
            }
        }

        EventListenerHolder& operator=(const std::function<void(Args...)>& func) noexcept
        {
            m_func = func;

            return *this;
        }

        void operator()(Args&&... args) const noexcept
        {
            if(m_func)
            {
                m_func(std::forward<Args>(args)...);
            }
        }

    private:
        std::function<void(Args...)> m_func;
        std::function<void()> m_unsubscribeFunc;
        std::list<std::shared_ptr<Event<void(Args...)>>> m_listeningEvents;
    };
}

#endif //SUNGEARENGINE_EVENTLISTENERHOLDER_H
