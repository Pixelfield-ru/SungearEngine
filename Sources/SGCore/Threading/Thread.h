//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_THREAD_H
#define THREADINGAPI_THREAD_H

#include <thread>
#include <functional>
#include <mutex>

#include "IWorker.h"

#include "SGUtils/Event.h"
#include "SGUtils/Utils.h"

namespace SGCore::Threading
{
    struct Thread : public std::enable_shared_from_this<Thread>
    {
        friend struct IWorker;
        friend struct ThreadsManager;
        
        Thread();
        ~Thread();
        
        virtual void start() noexcept;
        
        virtual void join() noexcept;
        
        void processWorkers() noexcept;
        
        std::shared_ptr<IWorker> createWorker(const WorkerSingletonGuard workerSingletonGuard)
        {
            const size_t workerGuardHash = hashObject(workerSingletonGuard);
            
            std::lock_guard guard(m_threadProcessMutex);
            
            if(!onWorkersProcess.contains(workerGuardHash))
            {
                auto worker = std::make_shared<IWorker>();
                worker->useSingletonGuard(workerSingletonGuard);
                
                return worker;
            }
            
            return nullptr;
        }
        
        std::shared_ptr<IWorker> createWorker() const noexcept
        {
            return std::make_shared<IWorker>();
        }
        
        void addWorker(std::shared_ptr<IWorker> worker)
        {
            const size_t workerGuardHash = hashObject(worker->m_parentWorkerGuard);
            
            std::lock_guard guard(m_threadProcessMutex);
            
            if(!onWorkersProcess.contains(workerGuardHash))
            {
                m_workers.push_back(worker);
                
                onWorkersProcess += worker->m_onExecuteListener;
            }
        }
        
        void removeWorker(std::shared_ptr<IWorker> worker)
        {
            if(!worker) return;
            
            std::lock_guard guard(m_threadProcessMutex);
            
            std::erase_if(m_workers, [&worker](std::shared_ptr<IWorker> w) {
                return w == worker;
            });
            
            onWorkersProcess -= worker->m_onExecuteListener;
        }
        
        void processFinishedWorkers() noexcept
        {
            std::vector<std::shared_ptr<IWorker>> finishedWorkersCopy;
            
            {
                std::lock_guard guard(m_workersEndCopyMutex);
                
                finishedWorkersCopy = m_finishedWorkersToExecute;
            }
            
            for(const auto& worker : finishedWorkersCopy)
            {
                worker->m_onExecutedCallback();
            }
            
            {
                std::lock_guard guard(m_workersEndCopyMutex);
                
                exclude(finishedWorkersCopy, m_finishedWorkersToExecute);
            }
        }
        
        template<typename Func>
        requires(std::is_invocable_v<Func, Event<void()>&>)
        void editOnUpdateEvent(const Func& func)
        {
            std::lock_guard guard(m_threadProcessMutex);
            
            func(onUpdate);
        }
        
        [[nodiscard]] inline auto getExecutionTime() const noexcept
        {
            return m_executionTime.load();
        }
        
        inline auto getNativeID() const noexcept
        {
            return m_nativeThreadID.load();
        }
    
    private:
        std::atomic<std::thread::id> m_nativeThreadID;
        
        std::mutex m_workersEndCopyMutex;
        std::mutex m_threadProcessMutex;
        
        std::vector<std::shared_ptr<IWorker>> m_finishedWorkersToExecute;
        
        std::atomic<double> m_executionTime;
        
        std::vector<std::shared_ptr<IWorker>> m_workers;
        Event<void()> onWorkersProcess;
        
        std::vector<std::shared_ptr<IWorker>> m_workersCopy;
        Event<void()> onWorkersProcessCopy;
        
        Event<void()> onUpdate;
        Event<void()> onUpdateCopy;
        
        std::atomic<bool> m_isBusy = false;
        std::atomic<bool> m_isAlive = false;
        
        std::thread m_thread;
    };
    
    struct MainThread : Thread
    {
        void start() noexcept final { }
        
        void join() noexcept final { }
    };
}

#endif //THREADINGAPI_THREAD_H
