//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_EVENT_H
#define SUNGEARENGINE_EVENT_H

#include <list>
#include <functional>
#include "EventListener.h"

#include "UUID.h"

namespace SGCore
{
    template <typename Signature>
    struct EventImpl;

    template<typename Return>
    struct EventImpl;

    template<typename Return, typename... Args>
    struct EventImpl<Return(Args...)> : public std::enable_shared_from_this<EventImpl<Return(Args...)>>
    {
    private:
        using HolderT = EventListenerImpl<Return(Args...)>;

    public:
        EventImpl& operator+=(HolderT* eventHolder)
        {
            m_callbacks.emplace_back(eventHolder);
            eventHolder->m_unsubscribeFunc = [eventHolder]()
            {
                for(auto& listeningEvent : eventHolder->m_listeningEvents)
                {
                    (listeningEvent)->m_callbacks.remove(eventHolder);
                }
            };
            eventHolder->m_listeningEvents.push_back(this->shared_from_this());

            return *this;
        }

        EventImpl& operator-=(HolderT* eventHolder)
        {
            m_callbacks.remove(eventHolder);

            return *this;
        }

        EventImpl& operator+=(const std::unique_ptr<HolderT>& eventHolder)
        {
            auto* rawHolder = eventHolder.get();

            m_callbacks.emplace_back(rawHolder);
            rawHolder->m_unsubscribeFunc = [rawHolder]()
            {
                for(auto& listeningEvent : rawHolder->m_listeningEvents)
                {
                    (listeningEvent)->m_callbacks.remove(rawHolder);
                }
            };
            rawHolder->m_listeningEvents.push_back(this->shared_from_this());

            return *this;
        }

        EventImpl& operator-=(const std::unique_ptr<HolderT>& eventHolder)
        {
            m_callbacks.remove(eventHolder.get());

            return *this;
        }

        void operator()(Args&&... args)
        {
            for(const auto& callback : m_callbacks)
            {
                (*callback)(std::forward<Args>(args)...);
            }
        }

    private:
        std::list<HolderT*> m_callbacks;
    };

    template <typename T>
    using Event = std::shared_ptr<EventImpl<T>>;

    template<typename T>
    constexpr Event<T> MakeEvent()
    {
        return std::make_shared<EventImpl<T>>();
    }
}

#endif //SUNGEARENGINE_EVENT_H
