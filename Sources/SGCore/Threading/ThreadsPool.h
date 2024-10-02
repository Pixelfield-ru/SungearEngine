//
// Created by ilya on 02.05.24.
//

#ifndef SUNGEARENGINE_THREADSPOOL_H
#define SUNGEARENGINE_THREADSPOOL_H

#include <SGCore/pch.h>

#include "ThreadPoolPolicies.h"
#include "Thread.h"

namespace SGCore::Threading
{
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
                if(auto lockedThread = threadsIt->lock())
                {
                    if(m_threadsSelector(lockedThread, currentPreferableThread))
                    {
                        currentPreferableThread = lockedThread;
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
        
        std::vector<std::weak_ptr<Thread>> m_threads;
    };

    using ThreadsPool = BaseThreadsPool<TasksCountLessThan>;
}

#endif //SUNGEARENGINE_THREADSPOOL_H
