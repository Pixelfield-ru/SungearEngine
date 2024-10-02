//
// Created by ilya on 03.04.24.
//

#include "ThreadsManager.h"

std::shared_ptr<SGCore::Threading::Thread> SGCore::Threading::ThreadsManager::currentThread() noexcept
{
    std::lock_guard guard(m_threads);

    const std::thread::id& currId = std::this_thread::get_id();
    auto& wrapped = m_threads.getWrapped();

    auto it = std::find_if(wrapped.begin(), wrapped.end(), [&currId](const std::shared_ptr<Thread>& thread) {
        return thread->getNativeID() == currId;
       });

    return (it == wrapped.end()) ? nullptr : *it;
}

std::shared_ptr<SGCore::Threading::Thread> SGCore::Threading::ThreadsManager::getMainThread() noexcept
{
    return m_mainThread;
}

SGCore::Threading::ThreadsPool& SGCore::Threading::ThreadsManager::getGlobalPool() noexcept
{
    return m_globalThreadsPool;
}
