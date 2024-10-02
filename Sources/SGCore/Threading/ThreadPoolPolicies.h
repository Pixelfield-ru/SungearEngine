#pragma once

#include <memory>
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
            if (auto lockedThread = thread.lock())
            {
                if (lockedThread->getExecutionTime() < m_maxCycleTime)
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
            if (auto lockedThread = thread.lock())
            {
                if (lockedThread->tasksCount() == 0)
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
            if (auto lockedThread = thread.lock())
            {
                if (lockedThread->tasksCount() < m_maxTasksCount)
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

            if (!currentLockedThread) return true;

            if (lockedThread && currentLockedThread)
            {
                if (lockedThread->tasksCount() < currentLockedThread->tasksCount())
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

            if (!currentLockedThread) return true;

            if (lockedThread && currentLockedThread)
            {
                if (lockedThread->getExecutionTime() < currentLockedThread->getExecutionTime())
                {
                    return true;
                }
            }

            return false;
        }
    };

}