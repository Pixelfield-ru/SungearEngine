//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_IWORKER_H
#define THREADINGAPI_IWORKER_H

#include "Event.h"

namespace SGCore
{
    struct Thread;

    struct WorkerGuardImpl { };
    using WorkerGuard = WorkerGuardImpl*;
    static WorkerGuard MakeWorkerGuard()
    {
        return new WorkerGuardImpl;
    }

    struct IWorker : public std::enable_shared_from_this<IWorker>
    {
        friend struct Thread;
        
        template<auto F>
        void setExecutableFunction(const WorkerGuard workerGuard)
        {
            const size_t hash = std::hash<const char*>()(static_cast<const char*>(static_cast<const void*>(workerGuard)));;
            
            m_onExecuteListener->m_hash = hash;
            
            m_executableFunction = F;
        }

        template<typename F>
        void setExecutableFunction(const WorkerGuard workerGuard, F&& func)
        {
            const size_t hash = std::hash<const char*>()(static_cast<const char*>(static_cast<const void*>(workerGuard)));;

            m_onExecuteListener->m_hash = hash;

            m_executableFunction = func;
        }

        void attachToThread(std::shared_ptr<Thread> thread);

        std::function<void(std::shared_ptr<IWorker> worker)> m_onExecutedFunction;
        
    private:
        WorkerGuard m_parentWorkerGuard = nullptr;
        std::weak_ptr<Thread> m_parentThread;

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
