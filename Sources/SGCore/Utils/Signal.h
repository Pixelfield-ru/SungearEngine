//
// Created by stuka on 27.03.2025.
//

#ifndef EVENT_H
#define EVENT_H

#include <list>
#include <shared_mutex>
#include <vector>
#include <unordered_set>
#include <atomic>

namespace SGCore
{
    template<typename>
    struct Slot;

    template<typename>
    struct Signal;

    template<typename ReturnT, typename... Args>
    struct Signal<ReturnT(Args...)>
    {
        friend struct Slot<ReturnT(Args...)>;

        using slot_t = Slot<ReturnT(Args...)>;

        Signal() = default;
        Signal(const Signal&) noexcept;
        Signal(Signal&&) noexcept;

        ~Signal() noexcept;

        Signal& operator+=(slot_t& slot) noexcept;

        template<typename Func>
        requires(std::is_invocable_v<Func, Args...>)
        [[nodiscard]] slot_t& operator+=(Func&& func) noexcept;

        Signal& operator-=(slot_t& slot) noexcept;

        template<auto FuncPtr>
        [[nodiscard]] slot_t& connect() noexcept;

        void disconnect(slot_t& slot) noexcept;

        [[nodiscard]] bool contains(const slot_t& slot) const noexcept;

        [[nodiscard]] std::int64_t slotsCount() const noexcept
        {
            return m_slotsCount;
        }

        [[nodiscard]] bool empty() const noexcept
        {
            return m_slotsCount == 0;
        }

        void clear() noexcept;

        template<typename... Args0>
        void operator()(Args0&&... args) const noexcept;

        Signal& operator=(const Signal&) noexcept;
        Signal& operator=(Signal&&) noexcept;

    private:
        std::list<slot_t*> m_slots;
        std::unordered_set<slot_t*> m_uniqueSlots;

        std::atomic<std::int64_t> m_slotsCount;

        mutable std::shared_mutex m_mutex;
    };
}

#include "Slot.h"

template<typename ReturnT, typename ... Args>
SGCore::Signal<ReturnT(Args...)>::Signal(const Signal& other) noexcept
{
    m_slotsCount = other.m_slotsCount.load();

    std::shared_lock otherSignalLock(other.m_mutex);

    for(auto* slot : other.m_slots)
    {
        m_uniqueSlots.insert(slot);
        m_slots.push_back(slot);

        {
            std::lock_guard slotLock(slot->m_mutex);
            slot->m_listenedSignals.push_back(this);
            ++slot->m_listenedSignalsCount;
        }
    }
}

template<typename ReturnT, typename ... Args>
SGCore::Signal<ReturnT(Args...)>::Signal(Signal&& other) noexcept
{
    m_slotsCount = other.m_slotsCount.load();

    {
        std::shared_lock otherSignalLock(other.m_mutex);

        for(auto* slot : other.m_slots)
        {
            m_uniqueSlots.insert(slot);
            m_slots.push_back(slot);

            {
                std::lock_guard slotLock(slot->m_mutex);
                slot->m_listenedSignals.remove(&other);
                slot->m_listenedSignals.push_back(this);
            }
        }
    }

    {
        std::lock_guard otherSignalLock(other.m_mutex);

        other.m_slots.clear();
    }
}

template<typename ReturnT, typename... Args>
SGCore::Signal<ReturnT(Args...)>::~Signal() noexcept
{
    clear();
}

template<typename ReturnT, typename... Args>
SGCore::Signal<ReturnT(Args...)>& SGCore::Signal<ReturnT(Args...)>::operator+=(slot_t& slot) noexcept
{
    std::lock_guard signalLock(m_mutex);
    if(m_uniqueSlots.contains(&slot)) return *this;

    ++m_slotsCount;

    {
        std::lock_guard slotLock(slot.m_mutex);
        slot.m_listenedSignals.push_back(this);
        ++slot.m_listenedSignalsCount;
    }

    m_slots.push_back(&slot);
    m_uniqueSlots.insert(&slot);

    return *this;
}

template<typename ReturnT, typename... Args>
template<typename Func>
requires(std::is_invocable_v<Func, Args...>)
[[nodiscard]] SGCore::Slot<ReturnT(Args...)>& SGCore::Signal<ReturnT(Args...)>::operator+=(Func&& func) noexcept
{
    auto* newSlot = new Slot<ReturnT(Args...)>();
    newSlot->m_listenedSignals.push_back(this);
    newSlot->m_isAllocatedBySignal = true;
    newSlot->m_function = std::forward<Func>(func);
    newSlot->m_listenedSignalsCount = 1;

    ++m_slotsCount;

    {
        std::lock_guard lock(m_mutex);
        m_slots.push_back(newSlot);
        m_uniqueSlots.insert(newSlot);
    }

    return *newSlot;
}

template<typename ReturnT, typename ... Args>
SGCore::Signal<ReturnT(Args...)>& SGCore::Signal<ReturnT(Args...)>::operator-=(slot_t& slot) noexcept
{
    disconnect(slot);

    return *this;
}

template<typename ReturnT, typename ... Args>
template<auto FuncPtr>
[[nodiscard]] SGCore::Slot<ReturnT(Args...)>& SGCore::Signal<ReturnT(Args...)>::connect() noexcept
{
    return operator+=(FuncPtr);
}

template<typename ReturnT, typename ... Args>
void SGCore::Signal<ReturnT(Args...)>::disconnect(slot_t& slot) noexcept
{
    {
        std::shared_lock signalLock(m_mutex);
        if(!m_uniqueSlots.contains(&slot)) return;
    }

    --m_slotsCount;

    bool isSlotBound = false;

    {
        std::lock_guard slotLock(slot.m_mutex);
        slot.m_listenedSignals.remove(this);
        --slot.m_listenedSignalsCount;

        isSlotBound = slot.bound();
    }

    {
        std::lock_guard signalLock(m_mutex);
        m_slots.remove(&slot);
        m_uniqueSlots.erase(&slot);
    }

    if(slot.m_isAllocatedBySignal && !isSlotBound)
    {
        delete &slot;
    }
}

template<typename ReturnT, typename ... Args>
bool SGCore::Signal<ReturnT(Args...)>::contains(const slot_t& slot) const noexcept
{
    std::shared_lock lock(m_mutex);
    return m_uniqueSlots.contains(const_cast<slot_t*>(&slot));
}

template<typename ReturnT, typename ... Args>
void SGCore::Signal<ReturnT(Args...)>::clear() noexcept
{
    std::lock_guard signalLock(m_mutex);

    for(auto* slot : m_slots)
    {
        bool isSlotBound = false;

        {
            std::lock_guard slotLock(slot->m_mutex);
            slot->m_listenedSignals.remove(this);
            --slot->m_listenedSignalsCount;

            isSlotBound = slot->bound();
        }

        if(slot->m_isAllocatedBySignal && !isSlotBound)
        {
            delete slot;
        }
    }

    m_slotsCount = 0;

    m_slots.clear();
    m_uniqueSlots.clear();
}

template<typename ReturnT, typename ... Args>
template<typename ... Args0>
void SGCore::Signal<ReturnT(Args...)>::operator()(Args0&&... args) const noexcept
{
    std::shared_lock lock(m_mutex);

    for(const auto* slot : m_slots)
    {
        if(slot->m_function)
        {
            slot->m_function(std::forward<Args0>(args)...);
        }
    }
}

template<typename ReturnT, typename ... Args>
SGCore::Signal<ReturnT(Args...)>& SGCore::Signal<ReturnT(Args...)>::operator=(const Signal& other) noexcept
{
    if(std::addressof(other) == this) return *this;

    m_slotsCount = other.m_slotsCount.load();

    std::scoped_lock signalsLock(other.m_mutex, m_mutex);

    for(auto* lastSlot : m_slots)
    {
        bool isSlotBound = false;

        {
            std::lock_guard slotLock(lastSlot->m_mutex);
            lastSlot->m_listenedSignals.remove(this);
            --lastSlot->m_listenedSignalsCount;

            isSlotBound = lastSlot->bound();
        }

        if(lastSlot->m_isAllocatedBySignal && !isSlotBound)
        {
            delete lastSlot;
        }
    }

    for(auto* slot : other.m_slots)
    {
        std::lock_guard slotLock(slot->m_mutex);
        slot->m_listenedSignals.push_back(this);
        ++slot->m_listenedSignalsCount;
    }

    m_uniqueSlots = other.m_uniqueSlots;
    m_slots = other.m_slots;

    return *this;
}

template<typename ReturnT, typename ... Args>
SGCore::Signal<ReturnT(Args...)>& SGCore::Signal<ReturnT(Args...)>::operator=(Signal&& other) noexcept
{
    if(std::addressof(other) == this) return *this;

    m_slotsCount = other.m_slotsCount.load();

    std::scoped_lock signalsLock(other.m_mutex, m_mutex);

    for(auto* lastSlot : m_slots)
    {
        bool isSlotBound = false;

        {
            std::lock_guard slotLock(lastSlot->m_mutex);
            lastSlot->m_listenedSignals.remove(this);
            --lastSlot->m_listenedSignalsCount;

            isSlotBound = lastSlot->bound();
        }

        if(lastSlot->m_isAllocatedBySignal && !isSlotBound)
        {
            delete lastSlot;
        }
    }

    for(auto* slot : other.m_slots)
    {
        {
            std::lock_guard slotLock(slot->m_mutex);
            slot->m_listenedSignals.remove(&other);
            slot->m_listenedSignals.push_back(this);
        }
    }

    m_uniqueSlots = std::move(other.m_uniqueSlots);
    m_slots = std::move(other.m_slots);

    return *this;
}

#endif //EVENT_H
