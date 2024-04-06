//
// Created by ilya on 03.04.24.
//

#include "Thread.h"

void SGCore::Thread::start() noexcept
{
    if(m_isBusy) return;
    
    auto internalFunc = [this]() {
        while(m_isAlive)
        {
            auto t0 = now();
            
            {
                std::lock_guard copyGuard(m_workersProcessMutex);
                
                *m_workersProcessCopy = *onWorkersProcess;
                m_workersCopy = m_workers;
            }
            
            (*m_workersProcessCopy)();
            
            {
                std::lock_guard copyGuard(m_workersProcessMutex);
                
                
                
                /*onWorkersProcess->excludeIf(workersProcessCopy, [](const EventImpl<void()>::holder_t* holder) {
                    return false;
                });*/
                onWorkersProcess->exclude(*m_workersProcessCopy);
                
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
    };
    
    m_isAlive = true;
    m_isBusy = true;
    m_thread = std::thread(internalFunc);
}

void SGCore::Thread::join() noexcept
{
    bool lastAlive = m_isAlive;
    m_isAlive = false;
    if(lastAlive)
    {
        m_thread.join();
    }
}
