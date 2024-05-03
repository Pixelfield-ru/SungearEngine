//
// Created by ilya on 02.05.24.
//

#include "ThreadsPool.h"

SGCore::Threading::ThreadsPool::ThreadsPool(SGCore::Threading::ThreadCreatePolicy threadCreatePolicy) noexcept
{
    m_defaultThreadsCreatePolicy = threadCreatePolicy;
}

SGCore::Threading::ThreadsPool::ThreadsPool
(SGCore::Threading::ThreadCreatePolicy threadCreatePolicy, const double& maxThreadsCycleExecutionPassTime) noexcept
{
    m_defaultThreadsCreatePolicy = threadCreatePolicy;
    m_maxThreadsCycleExecutionPassTime = maxThreadsCycleExecutionPassTime;
}

std::shared_ptr<SGCore::Threading::Thread> SGCore::Threading::ThreadsPool::getThread() noexcept
{
    return getThread(m_defaultThreadsCreatePolicy);
}

std::shared_ptr<SGCore::Threading::Thread> SGCore::Threading::ThreadsPool::getThread
(SGCore::Threading::ThreadCreatePolicy threadCreatePolicy) noexcept
{
    std::lock_guard guard(m_mutex);
    
    std::shared_ptr<Thread> foundThread;
    
    auto threadsIt = m_threads.begin();
    while(threadsIt != m_threads.end())
    {
        if(auto lockedThread = threadsIt->lock())
        {
            switch(threadCreatePolicy)
            {
                case IF_ALL_THREADS_VERY_BUSY:
                {
                    if(lockedThread->getExecutionTime() < m_maxThreadsCycleExecutionPassTime)
                    {
                        foundThread = lockedThread;
                    }
                    
                    break;
                }
                case IF_NO_FREE_THREADS:
                {
                    if(lockedThread->tasksCount() == 0)
                    {
                        foundThread = lockedThread;
                    }
                    
                    break;
                }
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
