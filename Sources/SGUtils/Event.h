//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_EVENT_H
#define SUNGEARENGINE_EVENT_H

#include <list>
#include <functional>
#include <map>

#include "EventListener.h"
#include "SGUtils/TypeTraits.h"

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
            m_currentMaxPriority = std::max(m_currentMaxPriority, holder->m_priority);
            
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    listeningEvent->m_callbacks.remove(std::make_pair(holder->m_hash, holder));
                    
                    listeningEvent->sortByPriorities();
                }
            };
            disconnect(holder->m_hash);
            m_callbacks.emplace_back(holder->m_hash, holder);
            holder->m_listeningEvents.push_back(this->shared_from_this());
            
            sortByPriorities();

            return *this;
        }

        EventImpl& operator+=(const std::unique_ptr<HolderT>& eventHolder)
        {
            auto* holder = eventHolder.get();
            
            m_currentMaxPriority = std::max(m_currentMaxPriority, holder->m_priority);
            
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    listeningEvent->m_callbacks.remove(std::make_pair(holder->m_hash, holder));
                    
                    listeningEvent->sortByPriorities();
                }
            };
            disconnect(holder->m_hash);
            m_callbacks.emplace_back(holder->m_hash, holder);
            holder->m_listeningEvents.push_back(this->shared_from_this());
            
            sortByPriorities();

            return *this;
        }
        
        EventImpl& operator+=(const HolderT* holder)
        {
            m_currentMaxPriority = std::max(m_currentMaxPriority, holder->m_priority);
            
            // m_callbacks.contains(holder->m_hash)
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    listeningEvent->m_callbacks.remove(std::make_pair(holder->m_hash, holder));
                    
                    listeningEvent->sortByPriorities();
                }
            };
            disconnect(holder->m_hash);
            m_callbacks.emplace_back(holder->m_hash, holder);
            holder->m_listeningEvents.push_back(this->shared_from_this());
            
            sortByPriorities();
            
            return *this;
        }
        
        template<typename Func>
        requires(std::is_invocable_v<Func, Args...>)
        EventImpl& operator+=(Func&& l)
        {
            ++m_currentMaxPriority;
            
            auto* holder = new HolderT(l);
            holder->m_priority = m_currentMaxPriority;
            holder->m_isLambda = true;
            std::cout << "hash : " << holder->m_hash << std::endl;
            
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    listeningEvent->m_callbacks.remove(std::make_pair(holder->m_hash, holder));
                    
                    listeningEvent->sortByPriorities();
                }
            };
            disconnect(holder->m_hash);
            m_callbacks.emplace_back(holder->m_hash, holder);
            holder->m_listeningEvents.push_back(this->shared_from_this());
            
            sortByPriorities();
            
            return *this;
        }
        
        template<auto FuncPtr>
        void connect(class_function_traits<remove_noexcept_t<decltype(FuncPtr)>>::instance_type& obj, const size_t& priority)
        {
            const auto fnPtr = FuncPtr;
            
            auto* holder = new HolderT;
            holder->m_priority = priority;
            holder->m_hash = std::hash<const char*>()(static_cast<const char*>(static_cast<const void*>(&fnPtr))) ^ *reinterpret_cast<std::intptr_t*>(&obj);
            holder->m_isLambda = true;
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    listeningEvent->m_callbacks.remove(std::make_pair(holder->m_hash, holder));
                    
                    listeningEvent->sortByPriorities();
                }
            };
            auto funcLambda = [&obj](Args&&... args) {
                (obj.*FuncPtr)(std::forward<Args>(args)...);
            };
            (*holder) = funcLambda;
            
            disconnect(holder->m_hash);
            m_callbacks.emplace_back(holder->m_hash, holder);
            holder->m_listeningEvents.push_back(this->shared_from_this());
            
            sortByPriorities();
        }
        
        template<auto FuncPtr>
        void connect(class_function_traits<remove_noexcept_t<decltype(FuncPtr)>>::instance_type& obj)
        {
            ++m_currentMaxPriority;
            connect<FuncPtr>(obj, m_currentMaxPriority);
        }
        
        template<auto FuncPtr>
        void connect(const size_t& priority)
        {
            const auto fnPtr = FuncPtr;
            
            auto* holder = new HolderT;
            holder->m_priority = priority;
            holder->m_hash = std::hash<const char*>()(static_cast<const char*>(static_cast<const void*>(&fnPtr)));
            holder->m_isLambda = true;
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    listeningEvent->m_callbacks.remove(std::make_pair(holder->m_hash, holder));
                    
                    listeningEvent->sortByPriorities();
                }
            };
            auto funcLambda = [](Args&&... args) {
                (*FuncPtr)(std::forward<Args>(args)...);
            };
            (*holder) = funcLambda;
            
            disconnect(holder->m_hash);
            m_callbacks.emplace_back(holder->m_hash, holder);
            holder->m_listeningEvents.push_back(this->shared_from_this());
            
            sortByPriorities();
        }
        
        template<auto FuncPtr>
        void connect()
        {
            ++m_currentMaxPriority;
            connect<FuncPtr>(m_currentMaxPriority);
        }
        
        template<auto FuncPtr>
        void disconnect(class_function_traits<remove_noexcept_t<decltype(FuncPtr)>>::instance_type& obj)
        {
            const auto fnPtr = FuncPtr;
            
            const size_t hash = std::hash<const char*>()(static_cast<const char*>(static_cast<const void*>(&fnPtr))) ^ *reinterpret_cast<std::intptr_t*>(&obj);
            
            disconnect(hash);
        }
        
        void disconnect(const size_t& funcHash)
        {
            auto it = std::find_if(m_callbacks.begin(), m_callbacks.end(), [&funcHash](const std::pair<long, HolderT*>& a) {
                return a.second->m_priority == funcHash;
            });
            if(it != m_callbacks.end())
            {
                if(it->second->m_isLambda)
                {
                    delete it->second;
                }
                m_callbacks.erase(it);
                sortByPriorities();
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
        size_t m_currentMaxPriority = 0;
        
        void sortByPriorities() noexcept
        {
            m_callbacks.sort([](const std::pair<long, HolderT*>& a, const std::pair<long, HolderT*>& b) {
                return a.second->m_priority < b.second->m_priority;
            });
        }
        
        std::list<std::pair<long, HolderT*>> m_callbacks;
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
