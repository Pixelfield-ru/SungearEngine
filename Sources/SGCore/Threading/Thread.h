//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_THREAD_H
#define THREADINGAPI_THREAD_H

#include <thread>

#include "SGCore/Coro/CoroScheduler.h"
#include "SGCore/Utils/Signal.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Timer.h"

namespace SGCore::Threading
{
    struct Task;

    struct Thread : public std::enable_shared_from_this<Thread>
    {
        friend struct Task;
        friend struct ThreadsManager;

        /**
         * Creates absolute new thread. It is recommended to use ThreadsPool to reuse threads and set a policy for creating new threads.
         * @return Absolute new thread.
         */
        static std::shared_ptr<Thread> create(const std::chrono::milliseconds& sleepTime = std::chrono::milliseconds(100)) noexcept;
        
        std::atomic<bool> m_autoJoinIfNotBusy = false;
        std::atomic<bool> m_sleepIfNotBusy = true;

        Coro::CoroScheduler m_coroScheduler { };

        Signal<void()> onUpdate;

        virtual ~Thread();
        
        virtual void start() noexcept;
        
        virtual void join() noexcept;
        
        void processTasks() noexcept;
        
        void addTask(std::shared_ptr<Task> task);
        void removeTask(std::shared_ptr<Task> task);
        
        [[nodiscard]] size_t tasksCount() noexcept;

        void setSleepTime(const std::chrono::milliseconds& sleepTime) noexcept 
        {
            m_sleepTime = sleepTime;
        }
        
        double getExecutionTime() const noexcept;
        virtual double getDeltaTime() const noexcept;
        
        std::thread::id getNativeID() const noexcept;
        
        bool isRunning() const noexcept;

        const std::chrono::milliseconds& getSleepTime() const noexcept 
        { 
            return m_sleepTime; 
        }
    
    protected:
        Thread() = default;

        void processFinishedTasks() noexcept;
        
        std::atomic<std::thread::id> m_nativeThreadID;
        
        std::mutex m_tasksEndCopyMutex;
        std::mutex m_threadProcessMutex;
        
        std::vector<std::shared_ptr<Task>> m_finishedTasksToExecute;
        
        std::atomic<double> m_executionTime = 0.0;
        std::atomic<double> m_currentTime = 0.0;
        std::atomic<double> m_deltaTime = 0.0;

        std::vector<Ref<Task>> m_tasks;
        std::vector<Ref<Task>> m_tasksCopy;

        std::atomic<bool> m_isRunning = false;
        std::atomic<bool> m_isAlive = true;
        std::atomic<bool> m_hasJoinRequest = false;
        std::atomic<bool> m_isTasksProcessing = false;

        std::thread m_thread;

        std::chrono::milliseconds m_sleepTime;
    };
    
    struct MainThread : public Thread
    {
        friend struct ThreadsManager;
        
        void start() noexcept final { }
        
        void join() noexcept final { }

        double getDeltaTime() const noexcept final;
        
    private:
        MainThread() = default;
    };
}

#endif //THREADINGAPI_THREAD_H
