//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_THREADSMANAGER_H
#define THREADINGAPI_THREADSMANAGER_H

#include <vector>
#include <memory>

#include "Thread.h"

namespace SGCore::Threading
{
    struct ThreadsManager
    {
        friend struct Thread;

        static std::shared_ptr<Thread> currentThread() noexcept
        {
            std::lock_guard guard(m_threadAccessMutex);

            auto it = m_threads.find(std::this_thread::get_id());
            if(it != m_threads.end())
            {
                return it->second->shared_from_this();
            }

            return nullptr;
        }

    private:
        static inline std::mutex m_threadAccessMutex;

        // static inline std::vector<std::shared_ptr<Thread>> m_threads;
        static inline std::unordered_map<std::thread::id, Thread*> m_threads;

        static inline std::shared_ptr<MainThread> m_mainThread = std::make_shared<MainThread>();

        static inline bool m_staticInit = []() {
            m_threads[std::this_thread::get_id()] = m_mainThread.get();
            return true;
        }();
    };
}

#endif //THREADINGAPI_THREADSMANAGER_H
