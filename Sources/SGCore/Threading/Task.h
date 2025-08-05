//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_TASK_H
#define THREADINGAPI_TASK_H

#include <SGCore/pch.h>

#include "SGCore/Utils/Signal.h"

namespace SGCore::Threading
{
    struct Thread;
    
    struct TaskSingletonGuardImpl { };
    /**
     * It is used to wait for the completion of the task.
     */
    using TaskSingletonGuard = TaskSingletonGuardImpl*;
    static TaskSingletonGuard MakeTaskSingletonGuard()
    {
        return new TaskSingletonGuardImpl;
    }
    
    struct Task : public std::enable_shared_from_this<Task>
    {
        friend struct Thread;

        std::atomic<bool> m_isStatic = false;
        
        template<typename F>
        void setOnExecuteCallback(F&& func)
        {
            std::lock_guard guard(m_listenerMutex);
            
            m_executableCallback = [func]() {
                func();
            };
        }
        
        template<typename F>
        void setOnExecutedCallback(F&& func)
        {
            std::lock_guard guard(m_listenerMutex);
            
            m_onExecutedCallback = [func]() {
                func();
            };
        }
        
        template<typename F>
        void setOnExecutedCallback(F&& func, const std::shared_ptr<Thread>& inThread)
        {
            std::lock_guard guard(m_listenerMutex);
            
            m_onExecutedCallbackParentThread = inThread;
            m_processFinishInOwnerThread = false;
            
            m_onExecutedCallback = [func]() {
                func();
            };
        }
        
        // TODO:
        // void attachToThread(std::shared_ptr<Thread> thread);
    
    private:
        std::mutex m_listenerMutex;
        
        std::atomic<bool> m_processFinishInOwnerThread = true;
        
        std::function<void()> m_executableCallback;
        std::function<void()> m_onExecutedCallback;
        
        TaskSingletonGuard m_parentTaskGuard = nullptr;
        
        std::weak_ptr<Thread> m_onExecutedCallbackParentThread;
        
        void execute() noexcept;
    };
}

#endif // THREADINGAPI_TASK_H
