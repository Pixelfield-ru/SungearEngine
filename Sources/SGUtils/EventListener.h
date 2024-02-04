//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_EVENTLISTENER_H
#define SUNGEARENGINE_EVENTLISTENER_H

#include <list>
#include <functional>
#include <memory>

#include "UUID.h"

namespace SGCore
{
    template<typename Return>
    struct EventImpl;

    template<typename Return>
    struct EventListenerImpl;

    template<typename Return, typename... Args>
    struct EventListenerImpl<Return(Args...)>
    {
        friend struct EventImpl<Return(Args...)>;

        EventListenerImpl() = default;
        EventListenerImpl(const std::function<Return(Args&&...)>& func)
        {
            m_func = func;
        }
        EventListenerImpl(const EventListenerImpl& e) = default;
        EventListenerImpl(EventListenerImpl&&) noexcept = default;

        ~EventListenerImpl()
        {
            if(m_unsubscribeFunc)
            {
                m_unsubscribeFunc();
            }
        }

        EventListenerImpl& operator=(const std::function<Return(Args&&...)>& func) noexcept
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
        std::list<std::shared_ptr<EventImpl<Return(Args...)>>> m_listeningEvents;
    };

    template <typename T>
    using EventListener = std::unique_ptr<EventListenerImpl<T>>;

    template<typename T, typename Func>
    constexpr EventListener<T> MakeEventListener(Func&& func)
    {
        return std::make_unique<EventListenerImpl<T>>(func);
    }
}

#endif //SUNGEARENGINE_EVENTLISTENER_H
