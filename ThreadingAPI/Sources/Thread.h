//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_THREAD_H
#define THREADINGAPI_THREAD_H

#include <thread>
#include <functional>
#include "Event.h"
#include "IWorker.h"

namespace SGCore
{
    struct Thread
    {
        ~Thread()
        {
            join();
        }
        
        void start() noexcept
        {
            auto internalFunc = [this]() {
                while(m_isAlive)
                {
                    {
                        std::lock_guard guard(m_workersProcessMutex);
                        
                        (*onWorkersProcess)();
                        
                        m_workers.clear();
                    }
                }
            };
            
            m_isAlive = true;
            m_thread = std::thread(internalFunc);
        }
        
        void join() noexcept;
        
        template<auto ExecFuncT, auto ExecFuncListenerT>
        std::shared_ptr<IWorker> addWorker()
        {
            std::lock_guard guard(m_workersProcessMutex);
            
            std::shared_ptr<IWorker> worker;
            
            if(!onWorkersProcess->contains<ExecFuncT>())
            {
                worker = std::make_shared<IWorker>();
                worker->setExecutableFunction<ExecFuncT>();
                worker->m_onExecutedFunction = ExecFuncListenerT;
                
                m_workers.push_back(worker);
                
                (*onWorkersProcess) += worker->m_onExecuteListener;
            }
            
            return worker;
        }
        
    private:
        std::mutex m_workersProcessMutex;
        
        std::vector<std::shared_ptr<IWorker>> m_workers;
        Event<void()> onWorkersProcess = MakeEvent<void()>();
        
        std::atomic<bool> m_isAlive = false;
        
        std::thread m_thread;
    };
}

#endif //THREADINGAPI_THREAD_H
