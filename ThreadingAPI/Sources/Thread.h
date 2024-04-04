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
        
        void start() noexcept
        {
            auto internalFunc = [this]() {
                while(m_isAlive)
                {
                    auto t0 = now();

                    EventImpl<void()> workersProcessCopy;
                    std::vector<std::shared_ptr<IWorker>> workersCopy;

                    {
                        std::lock_guard copyGuard(m_workersProcessMutex);

                        workersProcessCopy = *onWorkersProcess;
                        workersCopy = m_workers;
                    }

                    workersProcessCopy();

                    {
                        std::lock_guard copyGuard(m_workersProcessMutex);

                        size_t endElementIdx = std::min(workersCopy.size(), m_workers.size());
                        m_workers.erase(m_workers.begin(), m_workers.begin() + endElementIdx);
                    }

                    auto t1 = now();

                    m_executionTime = timeDiff<double, std::milli>(t0, t1);
                }
            };
            
            m_isAlive = true;
            m_thread = std::thread(internalFunc);
        }
        
        void join() noexcept;
        
        template<auto ExecFuncT, auto ExecFuncListenerT>
        std::shared_ptr<IWorker> addWorker(const WorkerGuard workerGuard)
        {
            std::lock_guard guard(m_workersProcessMutex);
            
            std::shared_ptr<IWorker> worker = nullptr;

            const size_t workerGuardHash =
                    std::hash<const char*>()(static_cast<const char*>(static_cast<const void*>(workerGuard)));;

            if(!onWorkersProcess->contains(workerGuardHash))
            {
                worker = std::make_shared<IWorker>();
                worker->setExecutableFunction<ExecFuncT>(workerGuard);
                worker->m_onExecutedFunction = ExecFuncListenerT;
                
                m_workers.push_back(worker);

                (*onWorkersProcess) += worker->m_onExecuteListener;
            }
            
            return worker;
        }

        template<typename ExecFuncT, typename ExecFuncListenerT>
        std::shared_ptr<IWorker> addWorker(const WorkerGuard  workerGuard, ExecFuncT&& execFunc, ExecFuncListenerT&& execFuncListener)
        {
            std::lock_guard guard(m_workersProcessMutex);

            std::shared_ptr<IWorker> worker = nullptr;

            const size_t workerGuardHash =
                    std::hash<const char*>()(static_cast<const char*>(static_cast<const void*>(workerGuard)));;

            if(!onWorkersProcess->contains(workerGuardHash))
            {
                worker = std::make_shared<IWorker>();
                worker->setExecutableFunction<>(workerGuard, execFunc);
                worker->m_onExecutedFunction = execFuncListener;

                m_workers.push_back(worker);

                (*onWorkersProcess) += worker->m_onExecuteListener;
            }

            return worker;
        }

        void removeWorker(std::shared_ptr<IWorker> worker)
        {
            std::lock_guard guard(m_workersProcessMutex);

            std::erase_if(m_workers, [&worker](std::shared_ptr<IWorker> w) {
                return w == worker; // w.get() == worker.get(); // w.owner_before(worker);
            });

            (*onWorkersProcess) -= worker->m_onExecuteListener;
        }

        inline auto getExecutionTime() const noexcept
        {
            return m_executionTime.load();
        }
        
    private:
        std::atomic<double> m_executionTime;

        std::mutex m_workersProcessMutex;
        
        std::vector<std::shared_ptr<IWorker>> m_workers;
        Event<void()> onWorkersProcess = MakeEvent<void()>();
        
        std::atomic<bool> m_isAlive = false;
        
        std::thread m_thread;
    };
}

#endif //THREADINGAPI_THREAD_H
