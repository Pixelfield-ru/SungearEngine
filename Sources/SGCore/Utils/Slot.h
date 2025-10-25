//
// Created by stuka on 27.03.2025.
//

#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include <functional>
#include <list>
#include <atomic>
#include <mutex>
#include <shared_mutex>

namespace SGCore
{
    template<typename>
    struct Signal;

    template<typename>
    struct Slot;

    template<typename ReturnT, typename... Args>
    struct Slot<ReturnT(Args...)>
    {
        friend struct Signal<ReturnT(Args...)>;

        using return_t = ReturnT;
        using func_t = ReturnT(Args...);

        Slot() noexcept = default;
        ~Slot() noexcept;
        Slot(const Slot&) noexcept = delete;
        Slot(Slot&&) noexcept = delete;
        template<typename Func>
        requires(std::is_invocable_v<Func, Args...>)
        Slot(Func&& func) noexcept;

        Slot(std::function<func_t> func) noexcept;

        Slot& operator=(const Slot&) noexcept = delete;
        Slot& operator=(Slot&&) noexcept = delete;

        template<typename Func>
        Slot& operator=(Func&& func) noexcept;

        Slot& operator=(std::function<func_t> func) noexcept;

        [[nodiscard]] std::int64_t listenedSignalsCount() const noexcept
        {
            return m_listenedSignalsCount;
        }

        [[nodiscard]] bool bound() const noexcept
        {
            return m_listenedSignalsCount != 0;
        }

        void connectToSignals(const Slot& slot, bool removeOldConnections = false) noexcept;

        void disconnectFromAll() noexcept;

    private:
        std::function<ReturnT(Args...)> m_function;

        std::list<Signal<ReturnT(Args...)>*> m_listenedSignals;
        std::atomic<std::int64_t> m_listenedSignalsCount;

        mutable std::shared_mutex m_mutex;

        bool m_isAllocatedBySignal = false;
    };
}

#include "Signal.h"

template<typename ReturnT, typename ... Args>
SGCore::Slot<ReturnT(Args...)>::~Slot() noexcept
{
    std::shared_lock slotLock(m_mutex);

    for(auto* signal : m_listenedSignals)
    {
        std::lock_guard signalLock(signal->m_mutex);

        signal->m_slots.remove(this);
        signal->m_uniqueSlots.erase(this);
    }
}

template<typename ReturnT, typename ... Args>
template<typename Func>
requires(std::is_invocable_v<Func, Args...>)
SGCore::Slot<ReturnT(Args...)>::Slot(Func&& func) noexcept
{
    m_function = std::forward<Func>(func);
}

template<typename ReturnT, typename ... Args>
SGCore::Slot<ReturnT(Args...)>::Slot(std::function<ReturnT(Args...)> func) noexcept
{
    m_function = std::move(func);
}

template<typename ReturnT, typename ... Args>
template<typename Func>
SGCore::Slot<ReturnT(Args...)>& SGCore::Slot<ReturnT(Args...)>::operator=(Func&& func) noexcept
{
    std::lock_guard lock(m_mutex);
    m_function = std::forward<Func>(func);

    return *this;
}

template<typename ReturnT, typename ... Args>
SGCore::Slot<ReturnT(Args...)>& SGCore::Slot<ReturnT(Args...)>::operator=(std::function<func_t> func) noexcept
{
    std::lock_guard lock(m_mutex);
    m_function = std::move(func);

    return *this;
}

template<typename ReturnT, typename ... Args>
void SGCore::Slot<ReturnT(Args...)>::connectToSignals(const Slot& slot, bool removeOldConnections) noexcept
{
    std::lock_guard slotLock(m_mutex);

    if(removeOldConnections)
    {
        for(auto* signal : m_listenedSignals)
        {
            std::lock_guard signalLock(signal->m_mutex);
            if(!signal->m_uniqueSlots.contains(this)) continue;

            signal->m_uniqueSlots.erase(this);
            signal->m_slots.remove(this);
        }

        m_listenedSignals.clear();
        m_listenedSignalsCount = 0;
    }

    for(auto* signal : slot.m_listenedSignals)
    {
        std::lock_guard signalLock(signal->m_mutex);
        if(signal->m_uniqueSlots.contains(this)) continue;

        signal->m_uniqueSlots.insert(this);
        signal->m_slots.push_back(this);

        m_listenedSignals.push_back(signal);
        ++m_listenedSignalsCount;
    }
}

template<typename ReturnT, typename ... Args>
void SGCore::Slot<ReturnT(Args...)>::disconnectFromAll() noexcept
{
    std::lock_guard slotLock(m_mutex);

    for(auto* signal : m_listenedSignals)
    {
        std::lock_guard signalLock(signal->m_mutex);
        if(!signal->m_uniqueSlots.contains(this)) continue;

        signal->m_uniqueSlots.erase(this);
        signal->m_slots.remove(this);
    }

    m_listenedSignals.clear();
    m_listenedSignalsCount = 0;
}

#endif //EVENTLISTENER_H
