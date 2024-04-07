//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_IWORKER_H
#define THREADINGAPI_IWORKER_H

#include <mutex>
#include <atomic>

#include "SGUtils/Event.h"
#include "SGUtils/Utils.h"

namespace SGCore::Threading
{
    struct Thread;

    struct WorkerSingletonGuardImpl { };
    /**
     * It is used to wait for the completion of the worker.
     */
    using WorkerSingletonGuard = WorkerSingletonGuardImpl*;
    static WorkerSingletonGuard MakeWorkerSingletonGuard()
    {
        return new WorkerSingletonGuardImpl;
    }

    struct IWorker : public std::enable_shared_from_this<IWorker>
    {
        friend struct Thread;
        
        bool m_isStatic = false;

        void useSingletonGuard(const WorkerSingletonGuard workerSingletonGuard) noexcept
        {
            const size_t hash = hashObject(workerSingletonGuard);

            std::lock_guard guard(m_listenerMutex);

            m_onExecuteListener.m_hash = hash;

            m_parentWorkerGuard = workerSingletonGuard;
        }

        template<typename F, typename... Args>
        void setOnExecuteCallback(F&& func, const Args&... args)
        {
            std::lock_guard guard(m_listenerMutex);

            m_executableCallback = [func, args...]() {
                func(args...);
            };
        }

        template<typename F, typename... Args>
        void setOnExecutedCallback(F&& func, const Args&... args)
        {
            std::lock_guard guard(m_listenerMutex);

            m_onExecutedCallback = [func, args...]() {
                func(args...);
            };
        }

        template<typename F, typename... Args>
        void setOnExecutedCallback(F&& func, const std::shared_ptr<Thread>& inThread, const Args&... args)
        {
            std::lock_guard guard(m_listenerMutex);

            m_onExecutedCallbackParentThread = inThread;
            m_processFinishInOwnerThread = false;

            m_onExecutedCallback = [func, args...]() {
                func(args...);
            };
        }

        // TODO:
        void attachToThread(std::shared_ptr<Thread> thread);
        
    private:
        std::mutex m_listenerMutex;

        std::atomic<bool> m_processFinishInOwnerThread = true;

        std::function<void()> m_executableCallback;
        std::function<void()> m_onExecutedCallback;

        WorkerSingletonGuard m_parentWorkerGuard = nullptr;

        std::weak_ptr<Thread> m_onExecutedCallbackParentThread;

        void execute() noexcept;

        EventListener<void()> m_onExecuteListener = [this]() {
            execute();
        };
    };
}

#endif //THREADINGAPI_IWORKER_H
