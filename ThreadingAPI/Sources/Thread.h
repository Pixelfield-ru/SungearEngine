//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_THREAD_H
#define THREADINGAPI_THREAD_H

#include <thread>
#include <functional>
#include <mutex>

#include "Event.h"
#include "IWorker.h"
#include "Utils.h"

namespace SGCore
{
    struct Thread
    {
        ~Thread()
        {
            join();
        }
        
        void start() noexcept;
        
        void join() noexcept;
        
        template<auto ExecFuncT, auto ExecFuncListenerT>
        std::shared_ptr<IWorker> addWorker(const WorkerSingletonGuard workerGuard)
        {
            std::lock_guard guard(m_workersProcessMutex);
            
            std::shared_ptr<IWorker> worker = nullptr;

            const size_t workerGuardHash = hashPointer(workerGuard);

            if(!onWorkersProcess->contains(workerGuardHash))
            {
                worker = std::make_shared<IWorker>();
                worker->setExecutableFunction<ExecFuncT>(workerGuard);
                worker->m_onExecutedFunction = ExecFuncListenerT;
                
                m_workers.push_back(worker);

                (*onWorkersProcess) += worker->m_onExecuteListener;
            }
            
            if(worker)
            {
                start();
            }
            
            return worker;
        }

        template<typename ExecFuncT, typename ExecFuncListenerT>
        std::shared_ptr<IWorker> addWorker(const WorkerSingletonGuard workerGuard, ExecFuncT&& execFunc, ExecFuncListenerT&& execFuncListener)
        {
            std::lock_guard guard(m_workersProcessMutex);

            std::shared_ptr<IWorker> worker = nullptr;

            const size_t workerGuardHash = hashPointer(workerGuard);

            if(!onWorkersProcess->contains(workerGuardHash))
            {
                worker = std::make_shared<IWorker>();
                worker->setExecutableFunction<>(workerGuard, execFunc);
                worker->m_onExecutedFunction = execFuncListener;

                m_workers.push_back(worker);

                (*onWorkersProcess) += worker->m_onExecuteListener;
            }
            
            if(worker)
            {
                start();
            }

            return worker;
        }

        void removeWorker(std::shared_ptr<IWorker> worker)
        {
            std::lock_guard guard(m_workersProcessMutex);

            std::erase_if(m_workers, [&worker](std::shared_ptr<IWorker> w) {
                return w == worker;
            });

            (*onWorkersProcess) -= worker->m_onExecuteListener;
        }

        [[nodiscard]] inline auto getExecutionTime() const noexcept
        {
            return m_executionTime.load();
        }
        
    private:
        std::atomic<double> m_executionTime;

        std::mutex m_workersProcessMutex;
        
        std::vector<std::shared_ptr<IWorker>> m_workers;
        Event<void()> onWorkersProcess = MakeEvent<void()>();
        
        Event<void()> m_workersProcessCopy = MakeEvent<void()>();
        std::vector<std::shared_ptr<IWorker>> m_workersCopy;
        
        std::atomic<bool> m_isBusy = false;
        std::atomic<bool> m_isAlive = false;
        
        std::thread m_thread;
    };
}

#endif //THREADINGAPI_THREAD_H
