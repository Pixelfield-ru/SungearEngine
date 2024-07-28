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

std::shared_ptr<SGCore::Threading::Thread> SGCore::Threading::ThreadsManager::getMainThread() noexcept
{
    return m_mainThread;
}

SGCore::Threading::ThreadsPool& SGCore::Threading::ThreadsManager::getGlobalPool() noexcept
{
    return m_globalThreadsPool;
}
