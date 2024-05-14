//
// Created by ilya on 02.05.24.
//

#ifndef SUNGEARENGINE_THREADSPOOL_H
#define SUNGEARENGINE_THREADSPOOL_H

#include <memory>

#include "Thread.h"

namespace SGCore::Threading
{
    struct ThreadsPoolSelectionPolicy
    {
        /**
         * Operator to check whether a thread is eligible for thread sampling as free or busy.\n Should return true if the thread is free and false if the thread is busy.
         * @param[in] thread Thread to check.
         * @return <b><em> True </em></b> if the thread is free and <b><em> False </em></b> if the thread is busy.
         */
        virtual bool operator()(std::weak_ptr<Thread> thread) noexcept = 0;
    };

    struct ThreadsGroupSelectionPolicy { };

    struct ThreadWithTimeSmallerThan final : ThreadsPoolSelectionPolicy, ThreadsGroupSelectionPolicy
    {
        bool operator()(std::weak_ptr<Thread> thread) noexcept final
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

    struct ThreadWithNoTasks final : ThreadsPoolSelectionPolicy
    {
        bool operator()(std::weak_ptr<Thread> thread) noexcept final
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

    struct ThreadWithTasksCountLessThan final : ThreadsPoolSelectionPolicy, ThreadsGroupSelectionPolicy
    {
        bool operator()(std::weak_ptr<Thread> thread) noexcept final
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

    template<typename SelectionPolicyT>
    requires(std::is_base_of_v<ThreadsPoolSelectionPolicy, SelectionPolicyT>)
    struct BaseThreadsPool
    {
        BaseThreadsPool() noexcept = default;

        std::shared_ptr<Thread> getThread() noexcept
        {
            std::lock_guard guard(m_mutex);

            std::shared_ptr<Thread> foundThread;

            auto threadsIt = m_threads.begin();
            while(threadsIt != m_threads.end())
            {
                if(auto lockedThread = threadsIt->lock())
                {
                    if(m_threadsSelector(lockedThread))
                    {
                        foundThread = lockedThread;
                    }

                    if(foundThread)
                    {
                        break;
                    }

                    ++threadsIt;
                }
                else
                {
                    threadsIt = m_threads.erase(threadsIt);
                }
            }

            if(!foundThread)
            {
                foundThread = Thread::create();
                m_threads.push_back(foundThread);
            }

            return foundThread;
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

    private:
        std::mutex m_mutex;

        SelectionPolicyT m_threadsSelector { };
        
        std::vector<std::weak_ptr<Thread>> m_threads;
    };

    using ThreadsPool = BaseThreadsPool<ThreadWithTasksCountLessThan>;
}

#endif //SUNGEARENGINE_THREADSPOOL_H
