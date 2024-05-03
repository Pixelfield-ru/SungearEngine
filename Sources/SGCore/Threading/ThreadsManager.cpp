//
// Created by ilya on 03.04.24.
//

#include "ThreadsManager.h"

std::shared_ptr<SGCore::Threading::Thread> SGCore::Threading::ThreadsManager::currentThread() noexcept
{
    std::lock_guard guard(m_threadAccessMutex);
    
    auto it = std::find_if(m_threads.begin(), m_threads.end(), [](const std::shared_ptr<Thread>& thread) {
        return thread->m_nativeThreadID == std::this_thread::get_id();
    });
    
    return it != m_threads.end() ? *it : nullptr;
}

std::shared_ptr<SGCore::Threading::Thread> SGCore::Threading::ThreadsManager::getLessLoadedThread() noexcept
{
    std::shared_ptr<Thread> lessLoadedThread;
    
    double minExecutionTime = std::numeric_limits<double>::max();
    
    for(const auto& thread : m_threads)
    {
        if(thread->getExecutionTime() < minExecutionTime)
        {
            minExecutionTime = thread->getExecutionTime();
            lessLoadedThread = thread;
        }
    }
    
    return lessLoadedThread;
}

std::shared_ptr<SGCore::Threading::Thread> SGCore::Threading::ThreadsManager::getMainThread() noexcept
{
    return m_mainThread;
}
