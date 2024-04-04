//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_THREADSMANAGER_H
#define THREADINGAPI_THREADSMANAGER_H

#include <vector>
#include <memory>

#include "Thread.h"

namespace SGCore
{
    struct ThreadsManager
    {
        static auto createThread()
        {
            auto thread = std::make_shared<Thread>();
            m_threads.push_back(thread);
            return thread;
        }

    
    private:
        static inline std::vector<std::shared_ptr<Thread>> m_threads;
    };
}

#endif //THREADINGAPI_THREADSMANAGER_H
