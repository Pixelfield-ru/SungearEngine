//
// Created by ilya on 02.05.24.
//

#ifndef SUNGEARENGINE_THREADSPOOL_H
#define SUNGEARENGINE_THREADSPOOL_H

#include <algorithm>
#include "Thread.h"

namespace SGCore::Threading
{
    struct ThreadsPoolSelectionPolicy
    {
        /**
         * Operator to check whether a thread is eligible for thread sampling as free or busy.\n Should return true if the thread is free and false if the thread is busy.
         * @param[in] thread Thread to check.
         * @param[in] currentPreferableThread Current chosen thread to select.
         * @return <b><em> True </em></b> if the thread is free and <b><em> False </em></b> if the thread is busy.
         */
        virtual bool operator()(std::weak_ptr<Thread> thread, std::weak_ptr<Thread> currentPreferableThread) noexcept = 0;
    };

    struct TimeSmallerThan final : ThreadsPoolSelectionPolicy
    {
        bool operator()(std::weak_ptr<Thread> thread, std::weak_ptr<Thread> currentPreferableThread) noexcept final
        {
            if(auto lockedThread = thread.lock())
            {
                if(lockedThread->getExecutionTime() < m_maxCycleTime)
                {
                    return true;
                }
            }

            return false;
        }

        double m_maxCycleTime = 0.1;
    };

    struct ZeroTasks final : ThreadsPoolSelectionPolicy
    {
        bool operator()(std::weak_ptr<Thread> thread, std::weak_ptr<Thread> currentPreferableThread) noexcept final
        {
            if(auto lockedThread = thread.lock())
            {
                if(lockedThread->tasksCount() == 0)
                {
                    return true;
                }
            }

            return false;
        }
    };

    struct TasksCountLessThan final : ThreadsPoolSelectionPolicy
    {
        bool operator()(std::weak_ptr<Thread> thread, std::weak_ptr<Thread> currentPreferableThread) noexcept final
        {
            if(auto lockedThread = thread.lock())
            {
                if(lockedThread->tasksCount() < m_maxTasksCount)
                {
                    return true;
                }
            }

            return false;
        }

        size_t m_maxTasksCount = 3;
    };
    
    struct LeastTasksCount final : ThreadsPoolSelectionPolicy
    {
        bool operator()(std::weak_ptr<Thread> thread, std::weak_ptr<Thread> currentPreferableThread) noexcept final
        {
            auto lockedThread = thread.lock();
            auto currentLockedThread = currentPreferableThread.lock();
            
            if(!currentLockedThread) return true;
            
            if(lockedThread && currentLockedThread)
            {
                if(lockedThread->tasksCount() < currentLockedThread->tasksCount())
                {
                    return true;
                }
            }
            
            return false;
        }
    };
    
    struct ShortestTime final : ThreadsPoolSelectionPolicy
    {
        bool operator()(std::weak_ptr<Thread> thread, std::weak_ptr<Thread> currentPreferableThread) noexcept final
        {
            auto lockedThread = thread.lock();
            auto currentLockedThread = currentPreferableThread.lock();
            
            if(!currentLockedThread) return true;
            
            if(lockedThread && currentLockedThread)
            {
                if(lockedThread->getExecutionTime() < currentLockedThread->getExecutionTime())
                {
                    return true;
                }
            }
            
            return false;
        }
    };

    template<typename SelectionPolicyT>
    requires(std::is_base_of_v<ThreadsPoolSelectionPolicy, SelectionPolicyT>)
    struct BaseThreadsPool
    {
        bool m_autoResize = true;
        
        BaseThreadsPool() noexcept = default;
        
        BaseThreadsPool(std::int64_t startThreadsCount) noexcept
        {
            resize(startThreadsCount);
        }
        
        BaseThreadsPool(std::int64_t startThreadsCount, bool autoResize) noexcept
        {
            m_autoResize = autoResize;
            resize(startThreadsCount);
        }

        std::shared_ptr<Thread> getThread() noexcept
        {
            std::lock_guard guard(m_mutex);

            if(m_threads.empty()) return nullptr;
            
            std::shared_ptr<Thread> currentPreferableThread;

            auto threadsIt = m_threads.begin();
            while(threadsIt != m_threads.end())
            {
                auto thread = *threadsIt;
                if(thread)
                {
                    if(m_threadsSelector(thread, currentPreferableThread))
                    {
                        currentPreferableThread = thread;
                    }

                    ++threadsIt;
                }
                else
                {
                    threadsIt = m_threads.erase(threadsIt);
                }
            }

            if(!currentPreferableThread && m_autoResize)
            {
                currentPreferableThread = Thread::create();
                m_threads.push_back(currentPreferableThread);
            }

            return currentPreferableThread;
        }

        // m_threads.size()
        [[nodiscard]] size_t totalThreadsCount() const noexcept
        {
            return m_threads.size();
        }

        // ВОЗВРАЩАЕТ КОЛИЧЕСТВО ПОТОКОВ, КОТОРЫЕ В ПРИЦИПЕ ЗАПУЩЕНЫ
        [[nodiscard]] size_t runningThreadsCount() noexcept
        {
            std::lock_guard guard(m_mutex);

            size_t cnt = std::count_if(m_threads.begin(), m_threads.end(), [](const std::weak_ptr<Thread>& weakThread) {
                if(auto lockedThread = weakThread.lock())
                {
                    if(lockedThread->isRunning())
                    {
                        return true;
                    }
                }

                return false;
            });

            return cnt;
        }

        // ВОЗВРАЩАЕТ КОЛИЧЕСТВО ПОТОКОВ, КОТОРЫЕ ВООБЩЕ НЕ ЗАПУЩЕНЫ
        [[nodiscard]] size_t idleThreadsCount() noexcept
        {
            return totalThreadsCount() - runningThreadsCount();
        }

        // ВОЗВРАЩАЕТ КОЛИЧЕСТВО ПОТОКОВ, КОТОРЫЕ ЗАНЯТЫ (ТЕСТ ВЫПОЛНЯЕТ m_threadsSelector)
        [[nodiscard]] size_t busyThreadsCount() noexcept
        {
            std::lock_guard guard(m_mutex);

            size_t cnt = std::count_if(m_threads.begin(), m_threads.end(), [this](const std::weak_ptr<Thread>& weakThread) {
                return !m_threadsSelector(weakThread);
            });

            return cnt;
        }
        
        // ВОЗВРАЩАЕТ КОЛИЧЕСТВО ПОТОКОВ, КОТОРЫЕ НЕ ЗАНЯТЫ (ТЕСТ ВЫПОЛНЯЕТ m_threadsSelector)
        [[nodiscard]] size_t notBusyThreadsCount() noexcept
        {
            return totalThreadsCount() - busyThreadsCount();
        }

        SelectionPolicyT& getSelector() noexcept
        {
            return m_threadsSelector;
        }
        
        void resize(std::int64_t numThreads) noexcept
        {
            size_t currentSize = m_threads.size();
            
            if(numThreads < currentSize)
            {
                m_threads.resize(numThreads);
            }
            else if(numThreads > currentSize)
            {
                for(std::int32_t i = 0; i < numThreads - currentSize; ++i)
                {
                    auto thread = Thread::create();
                    m_threads.push_back(thread);
                }
            }
        }
        
        [[nodiscard]] std::int64_t size() const noexcept
        {
            return std::ssize(m_threads);
        }

    private:
        std::mutex m_mutex;

        SelectionPolicyT m_threadsSelector { };
        
        std::vector<std::shared_ptr<Thread>> m_threads;
    };

    using ThreadsPool = BaseThreadsPool<TasksCountLessThan>;
}

#endif //SUNGEARENGINE_THREADSPOOL_H
