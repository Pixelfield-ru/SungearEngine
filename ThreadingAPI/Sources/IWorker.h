//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_IWORKER_H
#define THREADINGAPI_IWORKER_H

#include "Event.h"
#include "Utils.h"

namespace SGCore
{
    struct Thread;

    struct WorkerSingletonGuardImpl { };
    using WorkerSingletonGuard = WorkerSingletonGuardImpl*;
    static WorkerSingletonGuard MakeWorkerSingletonGuard()
    {
        return new WorkerSingletonGuardImpl;
    }

    struct IWorker : public std::enable_shared_from_this<IWorker>
    {
        friend struct Thread;
        
        bool m_isStatic = false;
        
        template<auto F>
        void setExecutableFunction(const WorkerSingletonGuard workerSingletonGuard)
        {
            const size_t hash = hashPointer(workerSingletonGuard);
            
            m_onExecuteListener->m_hash = hash;
            
            m_executableFunction = F;
        }

        template<typename F>
        void setExecutableFunction(const WorkerSingletonGuard workerSingletonGuard, F&& func)
        {
            const size_t hash = hashPointer(workerSingletonGuard);

            m_onExecuteListener->m_hash = hash;

            m_executableFunction = func;
        }

        // TODO:
        void attachToThread(std::shared_ptr<Thread> thread);

        std::function<void(std::shared_ptr<IWorker> worker)> m_onExecutedFunction;
        
    private:
        // WorkerGuard m_parentWorkerGuard = nullptr;
        // std::weak_ptr<Thread> m_parentThread;

        std::function<void()> m_executableFunction;
        
        EventListener<void()> m_onExecuteListener = MakeEventListener<void()>([this]() {
            if(m_executableFunction)
            {
                m_executableFunction();
            }
            if(m_onExecutedFunction)
            {
                m_onExecutedFunction(shared_from_this());
            }
        });
    };
}

#endif //THREADINGAPI_IWORKER_H
