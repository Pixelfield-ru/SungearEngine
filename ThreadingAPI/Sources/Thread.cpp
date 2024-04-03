//
// Created by ilya on 03.04.24.
//

#include "Thread.h"

void SGCore::Thread::join() noexcept
{
    bool lastAlive = m_isAlive;
    m_isAlive = false;
    if(lastAlive)
    {
        m_thread.join();
    }
}
