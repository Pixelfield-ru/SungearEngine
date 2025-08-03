//
// Created by ilya on 03.04.24.
//

#include "ThreadsManager.h"

std::shared_ptr<SGCore::Threading::Thread> SGCore::Threading::ThreadsManager::currentThread() noexcept
{
    std::lock_guard guard(m_threadAccessMutex);
    
    const auto it = m_threadsMap.find(std::this_thread::get_id());
    
    return it != m_threadsMap.end() ? it->second.lock() : nullptr;
}

std::shared_ptr<SGCore::Threading::Thread> SGCore::Threading::ThreadsManager::getMainThread() noexcept
{
    return m_mainThread;
}

SGCore::Threading::ThreadsPool& SGCore::Threading::ThreadsManager::getGlobalPool() noexcept
{
    return m_globalThreadsPool;
}
