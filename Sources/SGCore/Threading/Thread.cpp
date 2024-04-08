//
// Created by ilya on 03.04.24.
//

#include "Thread.h"

#include "ThreadsManager.h"

SGCore::Threading::Thread::Thread()
{
    std::lock_guard threadsAccessGuard(ThreadsManager::m_threadAccessMutex);
    
    ThreadsManager::m_threads.push_back(this);
}

SGCore::Threading::Thread::~Thread()
{
    join();
    
    std::lock_guard threadsAccessGuard(ThreadsManager::m_threadAccessMutex);
    
    std::erase_if(ThreadsManager::m_threads, [this](const Thread* thread) {
        return thread->m_nativeThreadID == m_nativeThreadID;
    });
}

void SGCore::Threading::Thread::processWorkers() noexcept
{
    auto t0 = now();
    
    {
        std::lock_guard copyGuard(m_threadProcessMutex);
        
        onWorkersProcessCopy = onWorkersProcess;
        m_workersCopy = m_workers;
        
        onUpdateCopy = onUpdate;
    }
    
    onWorkersProcessCopy();
    onUpdateCopy();
    
    {
        std::lock_guard copyGuard(m_threadProcessMutex);
        
        onWorkersProcess.exclude(onWorkersProcessCopy);
        
        // exclude from vector
        {
            size_t curIdx = 0;
            for(const auto& worker : m_workersCopy)
            {
                if(curIdx == m_workers.size()) break;
                
                std::erase(m_workers, worker);
                ++curIdx;
            }
        }
        
        m_workersCopy.clear();
    }
    
    auto t1 = now();
    
    m_executionTime = timeDiff<double, std::milli>(t0, t1);
}

void SGCore::Threading::Thread::start() noexcept
{
    if(m_isBusy) return;
    
    auto internalFunc = [this]() {
        while(m_isAlive)
        {
            processWorkers();
        }
    };
    
    m_isAlive = true;
    m_isBusy = true;
    m_thread = std::thread(internalFunc);
    
    m_nativeThreadID = m_thread.get_id();
}

void SGCore::Threading::Thread::join() noexcept
{
    bool lastAlive = m_isAlive;
    m_isAlive = false;
    if(lastAlive)
    {
        m_thread.join();
        m_isBusy = false;
    }
}
