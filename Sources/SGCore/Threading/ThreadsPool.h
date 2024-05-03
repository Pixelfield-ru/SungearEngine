//
// Created by ilya on 02.05.24.
//

#ifndef SUNGEARENGINE_THREADSPOOL_H
#define SUNGEARENGINE_THREADSPOOL_H

#include <memory>

#include "Thread.h"

namespace SGCore::Threading
{
    enum ThreadCreatePolicy
    {
        /// If there are no threads with execution time less than time that was set in thread pool.
        IF_ALL_THREADS_VERY_BUSY,
        /// If there are no threads with zero tasks.
        IF_NO_FREE_THREADS
    };
    
    struct ThreadsPool
    {
        ThreadsPool() noexcept = default;
        explicit ThreadsPool(ThreadCreatePolicy threadCreatePolicy) noexcept;
        explicit ThreadsPool(ThreadCreatePolicy threadCreatePolicy, const double& maxThreadsCycleExecutionPassTime) noexcept;
        ThreadsPool(const ThreadsPool&) noexcept = default;
        ThreadsPool(ThreadsPool&&) noexcept = default;
        
        ThreadCreatePolicy m_defaultThreadsCreatePolicy = ThreadCreatePolicy::IF_NO_FREE_THREADS;
        /// The maximum time of the update cycle for threads.\n
        /// Used when the IF_ALL_THREADS_VERY_BUSY policy is selected. \n
        /// Thus, if no thread in the pool satisfies the time frame when calling getThread() function, then a new free thread is created.\n
        /// Measured in seconds.
        double m_maxThreadsCycleExecutionPassTime = 0.01f;
        
        std::shared_ptr<Thread> getThread() noexcept;
        std::shared_ptr<Thread> getThread(ThreadCreatePolicy threadCreatePolicy) noexcept;
        
    private:
        std::mutex m_mutex;
        
        std::vector<std::weak_ptr<Thread>> m_threads;
    };
}

#endif //SUNGEARENGINE_THREADSPOOL_H
