//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_EVENT_H
#define SUNGEARENGINE_EVENT_H

#include <list>
#include <functional>
#include "EventListenerHolder.h"

#include "UUID.h"

namespace SGUtils
{
    template <typename Signature>
    struct EventListenerHolder;

    template <typename Signature>
    struct Event;

    template<typename... Args>
    struct Event<void(Args...)> : public std::enable_shared_from_this<Event<void(Args...)>>
    {
    private:
        using HolderT = EventListenerHolder<void(Args...)>;

    public:
        Event& operator+=(HolderT* eventHolder)
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

        Event& operator-=(HolderT* eventHolder)
        {
            m_callbacks.remove(eventHolder);

            return *this;
        }

        Event& operator+=(const std::unique_ptr<HolderT>& eventHolder)
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

        Event& operator-=(const std::unique_ptr<HolderT>& eventHolder)
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
}

#endif //SUNGEARENGINE_EVENT_H
