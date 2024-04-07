//
// Created by ilya on 03.04.24.
//

#include "IWorker.h"

#include "Thread.h"

void SGCore::Threading::IWorker::attachToThread(std::shared_ptr<Thread> thread)
{
    /*auto lockedThread = m_parentThread.lock();
    if(lockedThread)
    {
        lockedThread->removeWorker(shared_from_this());
    }

    m_parentThread = thread;*/
}

void SGCore::Threading::IWorker::execute() noexcept
{
    std::lock_guard guard(m_listenerMutex);

    if(m_executableCallback)
    {
        m_executableCallback();
    }
    if(m_onExecutedCallback)
    {
        if(m_processFinishInOwnerThread)
        {
            m_onExecutedCallback();
        }
        else
        {
            auto lockedParentThread = m_onExecutedCallbackParentThread.lock();

            std::cout << lockedParentThread << std::endl;

            if(lockedParentThread)
            {
                std::lock_guard executedCallbackGuard(lockedParentThread->m_workersEndCopyMutex);

                lockedParentThread->m_finishedWorkersToExecute.push_back(shared_from_this());
            }
        }
    }
}
