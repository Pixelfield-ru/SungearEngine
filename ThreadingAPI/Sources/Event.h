//
// Created by stuka on 28.11.2023.
//

#ifndef SUNGEARENGINE_EVENT_H
#define SUNGEARENGINE_EVENT_H

#include <list>
#include <functional>
#include <map>
#include <utility>
#include <iostream>

#include "EventListener.h"
#include "TypeTraits.h"
#include "Utils.h"

namespace SGCore
{
    template<typename Return>
    struct EventImpl;
    
    template<typename Return, typename... Args>
    struct EventImpl<Return(Args...)> : public std::enable_shared_from_this<EventImpl<Return(Args...)>>
    {
        using holder_t = EventListenerImpl<Return(Args...)>;
        using holders_container = std::list<holder_t*>;
        
        ~EventImpl()
        {
            for(auto& holder : m_listeners)
            {
                if(holder->m_isLambda)
                {
                    delete holder;
                }
            }
        }
        
        EventImpl& operator+=(holder_t* holder)
        {
            m_currentMaxPriority = std::max<size_t>(m_currentMaxPriority, holder->m_priority);
            
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    listeningEvent->m_listeners.remove(holder);
                    
                    listeningEvent->sortByPriorities();
                }
            };
            disconnect(holder->m_hash);
            m_listeners.emplace_back(holder->m_hash, holder);
            holder->m_listeningEvents.push_back(this->shared_from_this());
            
            sortByPriorities();
            
            return *this;
        }
        
        EventImpl& operator+=(const std::unique_ptr<holder_t>& eventHolder)
        {
            auto* holder = eventHolder.get();
            
            m_currentMaxPriority = std::max<size_t>(m_currentMaxPriority, holder->m_priority);
            
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    listeningEvent->m_listeners.remove(holder);
                    
                    listeningEvent->sortByPriorities();
                }
            };
            disconnect(holder->m_hash);
            m_listeners.emplace_back(holder);
            holder->m_listeningEvents.push_back(this->shared_from_this());
            
            sortByPriorities();
            
            return *this;
        }
        
        EventImpl& operator+=(const holder_t* holder)
        {
            m_currentMaxPriority = std::max<size_t>(m_currentMaxPriority, holder->m_priority);
            
            // m_callbacks.contains(holder->m_hash)
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    listeningEvent->m_listeners.remove(holder);
                    
                    listeningEvent->sortByPriorities();
                }
            };
            disconnect(holder->m_hash);
            m_listeners.emplace_back(holder);
            holder->m_listeningEvents.push_back(this->shared_from_this());
            
            sortByPriorities();
            
            return *this;
        }
        
        template<typename Func>
        requires(std::is_invocable_v<Func, Args...>)
        EventImpl& operator+=(Func&& l)
        {
            ++m_currentMaxPriority;
            
            auto* holder = new holder_t(l);
            holder->m_priority = m_currentMaxPriority;
            holder->m_isLambda = true;
            std::cout << "hash : " << holder->m_hash << std::endl;
            
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    listeningEvent->m_listeners.remove(holder);
                    
                    listeningEvent->sortByPriorities();
                }
            };
            disconnect(holder->m_hash);
            m_listeners.emplace_back(holder);
            holder->m_listeningEvents.push_back(this->shared_from_this());
            
            sortByPriorities();
            
            return *this;
        }
        
        template<auto FuncPtr>
        void connect(class_function_traits<remove_noexcept_t<decltype(FuncPtr)>>::instance_type& obj, const size_t& priority)
        {
            auto* holder = new holder_t;
            holder->m_priority = priority;
            holder->m_hash = hashPointer(FuncPtr) ^ reinterpret_cast<std::intptr_t>(&obj);
            holder->m_isLambda = true;
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    listeningEvent->m_listeners.remove(holder);
                    
                    listeningEvent->sortByPriorities();
                }
            };
            auto funcLambda = [&obj](Args&&... args) {
                (obj.*FuncPtr)(std::forward<Args>(args)...);
            };
            (*holder) = funcLambda;
            
            disconnect(holder->m_hash);
            m_listeners.emplace_back(holder);
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
            auto* holder = new holder_t;
            holder->m_priority = priority;
            holder->m_hash = hashPointer(FuncPtr);
            // std::cout << "hash ::: " << holder->m_hash << ", " << (std::hash<const char*>()(static_cast<const char*>(reinterpret_cast<const void*>(FuncPtr)))) << std::endl;
            holder->m_isLambda = true;
            holder->m_unsubscribeFunc = [holder]()
            {
                for(auto& listeningEvent : holder->m_listeningEvents)
                {
                    listeningEvent->m_listeners.remove(holder);
                    
                    listeningEvent->sortByPriorities();
                }
            };
            auto funcLambda = [](Args&&... args) {
                (*FuncPtr)(std::forward<Args>(args)...);
            };
            (*holder) = funcLambda;
            
            disconnect(holder->m_hash);
            m_listeners.push_back(holder);
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
            const size_t hash = hashPointer(FuncPtr) ^ reinterpret_cast<std::intptr_t>(&obj);
            
            disconnect(hash);
        }
        
        void disconnect(const size_t& funcHash)
        {
            auto it = std::find_if(m_listeners.begin(), m_listeners.end(), [&funcHash](const holder_t* a) {
                return a->m_hash == funcHash;
            });
            if(it != m_listeners.end())
            {
                if((*it)->m_isLambda)
                {
                    delete *it;
                }
                m_listeners.erase(it);
                sortByPriorities();
            }
        }
        
        EventImpl& operator-=(const std::unique_ptr<holder_t>& eventHolder)
        {
            disconnect(eventHolder->m_hash);
            
            return *this;
        }
        
        EventImpl& operator-=(const holder_t* eventHolder)
        {
            disconnect(eventHolder->m_hash);
            
            return *this;
        }
        
        template<typename... Args0>
        void operator()(Args0&&... args)
        {
            for(auto& callback : m_listeners)
            {
                (*(callback))(std::forward<Args0>(args)...);
            }
        }
        
        size_t listenersCount() const noexcept
        {
            return m_listeners.size();
        }
        
        void clear() noexcept
        {
            for(auto& holder : m_listeners)
            {
                if(holder.second->m_isLambda)
                {
                    delete holder.second;
                }
            }
            
            m_listeners.clear();
        }
        
        template<auto FuncPtr>
        bool contains()
        {
            const size_t hash = hashPointer(FuncPtr);
            
            return contains(hash);
        }
        
        template<auto FuncPtr>
        bool contains(class_function_traits<remove_noexcept_t<decltype(FuncPtr)>>::instance_type& obj)
        {
            const size_t hash = hashPointer(FuncPtr) ^ reinterpret_cast<std::intptr_t>(&obj);
            
            return contains(hash);
        }
        
        bool contains(const std::unique_ptr<holder_t>& eventHolder)
        {
            return contains(eventHolder->m_hash);
        }
        
        bool contains(const holder_t* eventHolder)
        {
            return contains(eventHolder->m_hash);
        }
        
        bool contains(const size_t& hash)
        {
            auto it = std::find_if(m_listeners.begin(), m_listeners.end(), [&hash](const holder_t* a) {
                return a->m_hash == hash;
            });
            
            return it != m_listeners.end();
        }
        
        void exclude(const EventImpl& other)
        {
            for(const auto& e : other.m_listeners)
            {
                typename holders_container::iterator it = std::find_if(m_listeners.begin(), m_listeners.end(), [&e](const holder_t* a) {
                    return a->m_hash == e->m_hash;
                });
                
                if(it != m_listeners.end())
                {
                    if((*it)->m_isLambda)
                    {
                        delete *it;
                    }
                    m_listeners.erase(it);
                }
            }
            
            sortByPriorities();
        }
        
        template<typename Func>
        requires(std::is_invocable_r_v<bool, Func, const holder_t*>)
        void excludeIf(const EventImpl& other, Func&& predicate)
        {
            for(const auto& e : other.m_listeners)
            {
                typename holders_container::iterator it = std::find_if(m_listeners.begin(), m_listeners.end(), [&e](const holder_t* a) {
                    return a->m_hash == e->m_hash;
                });
                
                if(it != m_listeners.end() && predicate(*it))
                {
                    if((*it)->m_isLambda)
                    {
                        delete *it;
                    }
                    m_listeners.erase(it);
                }
            }
            
            sortByPriorities();
        }
    
    private:
        size_t m_currentMaxPriority = 0;
        
        void sortByPriorities() noexcept
        {
            m_listeners.sort([](const holder_t* a, const holder_t* b) {
                return a->m_priority < b->m_priority;
            });
        }
        
        holders_container m_listeners;
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
