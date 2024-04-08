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

            auto it = std::find_if(m_threads.begin(), m_threads.end(), [](const Thread* thread) {
               return thread->m_nativeThreadID == std::this_thread::get_id();
            });

            if(it != m_threads.end())
            {
                return (*it)->shared_from_this();
            }

            return nullptr;
        }

    private:
        static inline std::mutex m_threadAccessMutex;

        static inline std::vector<Thread*> m_threads;

        static inline std::shared_ptr<MainThread> m_mainThread;

        static inline bool m_staticInit = []() {
            m_mainThread = std::make_shared<MainThread>();
            m_mainThread->m_nativeThreadID = std::this_thread::get_id();
            
            return true;
        }();
    };
}

#endif //THREADINGAPI_THREADSMANAGER_H
