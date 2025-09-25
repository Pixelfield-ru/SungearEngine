//
// Created by stuka on 25.09.2025.
//

#pragma once

#include <list>
#include <atomic>
#include <mutex>

namespace SGCore
{
    struct UniqueID;

    struct UniqueIDGenerator
    {
        friend struct UniqueID;

        static void generate(UniqueID& id) noexcept;

    private:
        static void pushFreeID(size_t id) noexcept;
        static size_t getFreeID() noexcept;

        static inline std::atomic<size_t> m_maxID { 0 };
        static inline std::mutex m_freeIDsMutex;
        static inline std::list<size_t> m_freeIDs { };
    };
}
