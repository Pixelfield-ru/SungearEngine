//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_THREAD_H
#define THREADINGAPI_THREAD_H

#include <SGCore/pch.h>

#include "Task.h"

#include "SGCore/Utils/Event.h"
#include "SGCore/Utils/Utils.h"

namespace SGCore::Threading
{
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
        
        ~Thread();
        
        virtual void start() noexcept;
        
        virtual void join() noexcept;
        
        void processTasks() noexcept;
        
        std::shared_ptr<Task> createTask(const TaskSingletonGuard taskSingletonGuard);
        std::shared_ptr<Task> createTask() const noexcept;
        
        void addTask(std::shared_ptr<Task> task);
        void removeTask(std::shared_ptr<Task> task);
        
        [[nodiscard]] size_t tasksCount() noexcept;
        
        void processFinishedTasks() noexcept;
        
        template<typename Func>
        requires(std::is_invocable_v<Func, Event<void()>&>)
        void editOnUpdateEvent(const Func& func)
        {
            std::lock_guard guard(m_threadProcessMutex);
            
            func(onUpdate);
        }

        void setSleepTime(const std::chrono::milliseconds& sleepTime) noexcept 
        {
            m_sleepTime = sleepTime;
        }
        
        const double& getExecutionTime() const noexcept;
        
        const std::thread::id& getNativeID() const noexcept;
        
        const bool& isRunning() const noexcept;

        const std::chrono::milliseconds& getSleepTime() const noexcept 
        { 
            return m_sleepTime; 
        }
    
    protected:
        Thread() = default;
        
        std::atomic<std::thread::id> m_nativeThreadID;
        
        std::mutex m_tasksEndCopyMutex;
        std::mutex m_threadProcessMutex;
        
        std::vector<std::shared_ptr<Task>> m_finishedTasksToExecute;
        
        std::atomic<double> m_executionTime = 0.0;
        
        std::vector<std::shared_ptr<Task>> m_tasks;
        Event<void()> onTasksProcess;
        
        std::vector<std::shared_ptr<Task>> m_tasksCopy;
        Event<void()> onTasksProcessCopy;
        
        Event<void()> onUpdate;
        Event<void()> onUpdateCopy;
        
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
        
    private:
        MainThread() = default;
    };
}

#endif //THREADINGAPI_THREAD_H
