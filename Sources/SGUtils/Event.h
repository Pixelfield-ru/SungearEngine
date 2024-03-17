//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_EVENT_H
#define SUNGEARENGINE_EVENT_H

#include <list>
#include <functional>
#include "EventListener.h"
#include "SGUtils/TypeInfo.h"

#include "UUID.h"

namespace SGCore
{
    template<typename Return>
    struct EventImpl;

    template<typename Return, typename... Args>
    struct EventImpl<Return(Args...)> : public std::enable_shared_from_this<EventImpl<Return(Args...)>>
    {
    private:
        using HolderT = EventListenerImpl<Return(Args...)>;

    public:
        ~EventImpl()
        {
            for(auto& holder : m_callbacks)
            {
                if(holder.second->m_isLambda)
                {
                    delete holder.second;
                }
            }
        }
        
        EventImpl& operator+=(HolderT* holder)
        {
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    (listeningEvent)->m_callbacks.erase(holder->m_hash);
                }
            };
            disconnect(holder->m_hash);
            m_callbacks[holder->m_hash] = holder;
            holder->m_listeningEvents.push_back(this->shared_from_this());

            return *this;
        }

        EventImpl& operator+=(const std::unique_ptr<HolderT>& eventHolder)
        {
            auto* holder = eventHolder.get();
            
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    (listeningEvent)->m_callbacks.erase(holder->m_hash);
                }
            };
            disconnect(holder->m_hash);
            m_callbacks[holder->m_hash] = holder;
            holder->m_listeningEvents.push_back(this->shared_from_this());

            return *this;
        }
        
        EventImpl& operator+=(const HolderT* holder)
        {
            // m_callbacks.contains(holder->m_hash)
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    (listeningEvent)->m_callbacks.erase(holder->m_hash);
                }
            };
            disconnect(holder->m_hash);
            m_callbacks[holder->m_hash] = holder;
            holder->m_listeningEvents.push_back(this->shared_from_this());
            
            return *this;
        }
        
        template<typename Func>
        requires(std::is_invocable_v<Func, Args...>)
        EventImpl& operator+=(Func&& l)
        {
            auto* holder = new HolderT(l);
            holder->m_isLambda = true;
            std::cout << "hash : " << holder->m_hash << std::endl;
            
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    (listeningEvent)->m_callbacks.erase(holder->m_hash);
                }
            };
            disconnect(holder->m_hash);
            m_callbacks[holder->m_hash] = holder;
            holder->m_listeningEvents.push_back(this->shared_from_this());
            
            return *this;
        }
        
        template<auto FuncPtr>
        void connect(SGUtils::MemberFunctionTraits<decltype(FuncPtr)>::instance_type& obj)
        {
            auto* holder = new HolderT;
            holder->m_hash = std::hash<const char*>()(typeid(FuncPtr).name()) ^ *reinterpret_cast<std::intptr_t*>(&obj);
            holder->m_isLambda = true;
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    (listeningEvent)->m_callbacks.erase(holder->m_hash);
                }
            };
            auto funcLambda = [&obj](Args&&... args) {
                (obj.*FuncPtr)(std::forward<Args>(args)...);
            };
            (*holder) = funcLambda;
            
            disconnect(holder->m_hash);
            m_callbacks[holder->m_hash] = holder;
            holder->m_listeningEvents.push_back(this->shared_from_this());
        }
        
        template<auto FuncPtr>
        void connect()
        {
            auto* holder = new HolderT;
            holder->m_hash = std::hash<const char*>()(typeid(FuncPtr).name());
            holder->m_isLambda = true;
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    (listeningEvent)->m_callbacks.erase(holder->m_hash);
                }
            };
            auto funcLambda = [](Args&&... args) {
                (*FuncPtr)(std::forward<Args>(args)...);
            };
            (*holder) = funcLambda;
            
            disconnect(holder->m_hash);
            m_callbacks[holder->m_hash] = holder;
            holder->m_listeningEvents.push_back(this->shared_from_this());
        }
        
        template<auto FuncPtr>
        void disconnect(SGUtils::MemberFunctionTraits<decltype(FuncPtr)>::instance_type& obj)
        {
            const size_t hash = std::hash<const char*>()(typeid(FuncPtr).name()) ^ *reinterpret_cast<std::intptr_t*>(&obj);
            
            disconnect(hash);
        }
        
        void disconnect(const size_t& funcHash)
        {
            auto it = m_callbacks.find(funcHash);
            if(it != m_callbacks.end())
            {
                if(it->second->m_isLambda)
                {
                    delete it->second;
                }
                m_callbacks.erase(it);
            }
        }
        
        EventImpl& operator-=(const std::unique_ptr<HolderT>& eventHolder)
        {
            disconnect(eventHolder->m_hash);

            return *this;
        }
        
        EventImpl& operator-=(const HolderT* eventHolder)
        {
            disconnect(eventHolder->m_hash);
            
            return *this;
        }
        
        template<typename... Args0>
        void operator()(Args0&&... args)
        {
            for(auto& callback : m_callbacks)
            {
                (*(callback.second))(std::forward<Args0>(args)...);
            }
        }

        size_t listenersCount() const noexcept
        {
            return m_callbacks.size();
        }
        
    private:
        std::unordered_map<long, HolderT*> m_callbacks;
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
