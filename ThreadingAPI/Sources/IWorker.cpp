//
// Created by ilya on 03.04.24.
//

#include "IWorker.h"

#include "Thread.h"

void SGCore::IWorker::attachToThread(std::shared_ptr<Thread> thread)
{
    auto lockedThread = m_parentThread.lock();
    if(lockedThread)
    {
        lockedThread->removeWorker(shared_from_this());
    }

    m_parentThread = thread;
}
