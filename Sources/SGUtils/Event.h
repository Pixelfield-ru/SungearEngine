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
    struct Event;
    
    template<typename Return, typename... Args>
    struct Event<Return(Args...)>
    {
        using holder_t = EventListener<Return(Args...)>;
        using holders_container = std::list<holder_t*>;
        
        Event() = default;
        Event(const Event& other)
        {
            *this = std::forward<Event>(other);
        }
        Event(Event&& other) noexcept
        {
            *this = std::forward<Event>(other);
        }
        
        ~Event()
        {
            for(auto& holder : m_listeners)
            {
                holder->m_listeningEvents.remove(this);
            }
            
            for(auto& holder : m_listeners)
            {
                if(holder->m_isOwnedByEvent)
                {
                    delete holder;
                    holder = nullptr;
                }
            }
        }
        
        Event& operator+=(holder_t& holder)
        {
            m_currentMaxPriority = std::max<size_t>(m_currentMaxPriority, holder.m_priority);
            
            setHolderUnsubscribeFunction(&holder);
            
            disconnect(holder.m_hash);
            m_listeners.emplace_back(&holder);
            holder.m_listeningEvents.push_back(this);
            
            sortByPriorities();
            
            return *this;
        }
        
        template<typename Func>
        requires(std::is_invocable_v<Func, Args...>)
        Event& operator+=(Func&& l)
        {
            ++m_currentMaxPriority;
            
            auto* holder = new holder_t(l);
            holder->m_priority = m_currentMaxPriority;
            holder->m_isOwnedByEvent = true;
            // std::cout << "hash : " << holder->m_hash << std::endl;
            
            setHolderUnsubscribeFunction(holder);
            
            disconnect(holder->m_hash);
            m_listeners.emplace_back(holder);
            holder->m_listeningEvents.push_back(this);
            
            sortByPriorities();
            
            return *this;
        }
        
        template<auto FuncPtr>
        void connect(class_function_traits<remove_noexcept_t<decltype(FuncPtr)>>::instance_type& obj, const size_t& priority)
        {
            auto* holder = new holder_t;
            holder->m_priority = priority;
            holder->m_hash = hashMemberFunction<FuncPtr>(obj);
            holder->m_isOwnedByEvent = true;
            setHolderUnsubscribeFunction(holder);
            
            auto funcLambda = [&obj](Args&&... args) {
                (obj.*FuncPtr)(std::forward<Args>(args)...);
            };
            (*holder) = funcLambda;
            
            disconnect(holder->m_hash);
            m_listeners.emplace_back(holder);
            holder->m_listeningEvents.push_back(this);
            
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
            holder->m_hash = hashConstexprObject<FuncPtr>();
            // std::cout << "hash ::: " << holder->m_hash << ", " << (std::hash<const char*>()(static_cast<const char*>(reinterpret_cast<const void*>(FuncPtr)))) << std::endl;
            holder->m_isOwnedByEvent = true;
            setHolderUnsubscribeFunction(holder);
            
            auto funcLambda = [](Args&&... args) {
                (*FuncPtr)(std::forward<Args>(args)...);
            };
            (*holder) = funcLambda;
            
            disconnect(holder->m_hash);
            m_listeners.push_back(holder);
            holder->m_listeningEvents.push_back(this);
            
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
            const size_t hash = hashMemberFunction<FuncPtr>(obj);
            
            disconnect(hash);
        }
        
        void disconnect(const size_t& funcHash)
        {
            auto it = std::find_if(m_listeners.begin(), m_listeners.end(), [&funcHash](const holder_t* a) {
                return a->m_hash == funcHash;
            });
            
            if(it != m_listeners.end())
            {
                auto ptr = *it;
                
                if(!ptr->m_isOwnedByEvent)
                {
                    ptr->m_listeningEvents.remove(this);
                }
                
                m_listeners.erase(it);
                
                if(ptr->m_isOwnedByEvent)
                {
                    delete ptr;
                }
                
                // m_listeners.erase(it);
                sortByPriorities();
            }
        }
        
        Event& operator-=(const holder_t& eventHolder)
        {
            disconnect(eventHolder.m_hash);
            
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
                if(holder->m_isOwnedByEvent)
                {
                    delete holder;
                }
                else
                {
                    holder->m_listeningEvents.remove(this);
                }
            }
            
            m_listeners.clear();
        }
        
        template<auto FuncPtr>
        bool contains()
        {
            const size_t hash = hashConstexprObject<FuncPtr>();
            
            return contains(hash);
        }
        
        template<auto FuncPtr>
        bool contains(class_function_traits<remove_noexcept_t<decltype(FuncPtr)>>::instance_type& obj)
        {
            const size_t hash = hashMemberFunction<FuncPtr>(obj);
            
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
        
        void exclude(const Event& other)
        {
            for(const auto& e : other.m_listeners)
            {
                typename holders_container::iterator it = std::find_if(m_listeners.begin(), m_listeners.end(), [&e](const holder_t* a) {
                    return a->m_hash == e->m_hash;
                });
                
                if(it != m_listeners.end())
                {
                    auto ptr = *it;
                    
                    if(!ptr->m_isOwnedByEvent)
                    {
                        ptr->m_listeningEvents.remove(this);
                    }
                    
                    m_listeners.erase(it);
                    
                    if(ptr->m_isOwnedByEvent)
                    {
                        delete ptr;
                    }
                }
            }
            
            sortByPriorities();
        }
        
        Event& operator=(const Event& other) noexcept
        {
            if(this == std::addressof(other)) return *this;
            
            clear();
            
            m_currentMaxPriority = other.m_currentMaxPriority;
            
            for(holder_t* listener : other.m_listeners)
            {
                holder_t* holder = listener;
                
                if(listener->m_isOwnedByEvent)
                {
                    holder = new holder_t(*listener);
                }
                else
                {
                    holder->m_listeningEvents.push_back(this);
                }
                
                m_listeners.push_back(holder);
            }
            
            sortByPriorities();
            
            return *this;
        }
        
        Event& operator=(Event&& other) noexcept
        {
            if(this == std::addressof(other)) return *this;
            
            clear();
            
            m_currentMaxPriority = other.m_currentMaxPriority;
            
            for(holder_t* listener : other.m_listeners)
            {
                holder_t* holder = listener;
                
                if(listener->m_isOwnedByEvent)
                {
                    holder = new holder_t(*listener);
                }
                else
                {
                    holder->m_listeningEvents.push_back(this);
                }
                
                m_listeners.push_back(holder);
            }
            
            sortByPriorities();
            
            other.clear();
            
            return *this;
        }
    
    private:
        size_t m_currentMaxPriority = 0;
        
        void setHolderUnsubscribeFunction(holder_t* holder) noexcept
        {
            holder->m_unsubscribeFunc = [](holder_t* thisHolder)
            {
                for(auto& listeningEvent : thisHolder->m_listeningEvents)
                {
                    listeningEvent->m_listeners.remove(thisHolder);
                    
                    listeningEvent->sortByPriorities();
                }
            };

            holder->m_copyToEventsFunc = [](const holder_t* from, holder_t* thisHolder)
            {
                for(auto& listeningEvent : thisHolder->m_listeningEvents)
                {
                    *listeningEvent -= *thisHolder;
                }

                for(auto& listeningEvent : from->m_listeningEvents)
                {
                    *listeningEvent += *thisHolder;
                }
            };
        }
        
        void sortByPriorities() noexcept
        {
            m_listeners.sort([](const holder_t* a, const holder_t* b) {
                return a->m_priority < b->m_priority;
            });
        }
        
        holders_container m_listeners;
    };
}

#endif //SUNGEARENGINE_EVENT_H
