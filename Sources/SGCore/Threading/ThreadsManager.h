//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_THREADSMANAGER_H
#define THREADINGAPI_THREADSMANAGER_H

#include <SGCore/pch.h>

#include "Thread.h"
#include "ThreadsPool.h"
#include "SGCore/Utils/Utils.h"

namespace SGCore::Threading
{
    struct ThreadsManager
    {
        friend struct Thread;
        
        static std::shared_ptr<Thread> currentThread() noexcept;
        
        static std::shared_ptr<Thread> getMainThread() noexcept;

        SG_NOINLINE static ThreadsPool& getGlobalPool() noexcept;
        
    private:
        static inline ThreadsPool m_globalThreadsPool;
        
        static inline std::mutex m_threadAccessMutex;
        
        static inline std::vector<std::weak_ptr<Thread>> m_threads;
        static inline std::unordered_map<std::thread::id, std::weak_ptr<Thread>> m_threadsMap;

        static inline std::shared_ptr<MainThread> m_mainThread;

        static inline bool m_staticInit = []() {
            m_mainThread = std::shared_ptr<MainThread>(new MainThread);
            m_mainThread->m_nativeThreadID = std::this_thread::get_id();
            m_mainThread->m_sleepIfNotBusy = false;
            m_threads.push_back(m_mainThread);
            m_threadsMap[m_mainThread->m_nativeThreadID] = m_mainThread;
            
            return true;
        }();
    };
}

#endif //THREADINGAPI_THREADSMANAGER_H
