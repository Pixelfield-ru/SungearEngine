//
// Created by ilya on 03.04.24.
//

#include "Thread.h"

#include "ThreadsManager.h"

std::shared_ptr<SGCore::Threading::Thread> SGCore::Threading::Thread::create() noexcept
{
    std::lock_guard threadsAccessGuard(ThreadsManager::m_threads);
    return ThreadsManager::m_threads.getWrapped().emplace_back(std::move(std::shared_ptr<Thread>(new Thread)));
}

SGCore::Threading::Thread::~Thread()
{
    join();
}

void SGCore::Threading::Thread::processTasks() noexcept
{
    using namespace std::chrono_literals;
    
    auto t0 = now();
    
    {
        std::lock_guard copyGuard(m_threadProcessMutex);
        
        onTasksProcessCopy = onTasksProcess;
        m_tasksCopy = m_tasks;
        
        onUpdateCopy = onUpdate;
    }
    
    onTasksProcessCopy();
    onUpdateCopy();
    
    size_t tasksCount = 0;
    size_t onUpdateEventListenersCount = 0;
    
    {
        std::lock_guard copyGuard(m_threadProcessMutex);
        
        onTasksProcess.exclude(onTasksProcessCopy);
        
        // exclude from vector
        {
            size_t curIdx = 0;
            for(const auto& task : m_tasksCopy)
            {
                if(curIdx == m_tasks.size()) break;
                
                std::erase(m_tasks, task);
                ++curIdx;
            }
        }
        
        m_tasksCopy.clear();
        
        tasksCount = m_tasks.size();
        onUpdateEventListenersCount = onUpdate.listenersCount();
    }
    
    processFinishedTasks();
    
    // THREAD IS FULL FREE
    if(tasksCount == 0 && onUpdateEventListenersCount == 0)
    {
        if(m_autoJoinIfNotBusy)
        {
            auto joinThisThreadTask = ThreadsManager::getMainThread()->createTask();
            joinThisThreadTask->setOnExecuteCallback([threadToJoin = shared_from_this()]() {
                threadToJoin->join();
            });
            ThreadsManager::getMainThread()->addTask(joinThisThreadTask);
        }
        
        if(m_sleepIfNotBusy)
        {
            std::this_thread::sleep_for(100ms);
        }
    }
    
    auto t1 = now();
    
    m_executionTime = timeDiff<double, std::milli>(t0, t1);
}

void SGCore::Threading::Thread::start() noexcept
{
    if(m_isRunning || m_hasJoinRequest) return;
    
    auto internalFunc = [this]() {
        while(m_isAlive && m_isRunning)
        {
            processTasks();
        }
    };
    
    m_isRunning = true;
    m_thread = std::thread(internalFunc);
    
    m_nativeThreadID = m_thread.get_id();
}

void SGCore::Threading::Thread::join() noexcept
{
    if(!m_isRunning) return;

    m_isRunning = false;

    std::cout << "joining thread..." << std::endl;

    // PREVENT FROM CALLING start() WHEN THREAD IS NOT JOINED YET BUT NOT RUNNING
    m_hasJoinRequest = true;
    m_thread.join();
    m_hasJoinRequest = false;
}

std::shared_ptr<SGCore::Threading::Task> SGCore::Threading::Thread::createTask
(const SGCore::Threading::TaskSingletonGuard taskSingletonGuard)
{
    const size_t taskGuardHash = hashObject(taskSingletonGuard);
    
    std::lock_guard guard(m_threadProcessMutex);
    
    if(!onTasksProcess.contains(taskGuardHash))
    {
        auto task = std::make_shared<Task>();
        task->useSingletonGuard(taskSingletonGuard);
        
        return task;
    }
    
    return nullptr;
}

std::shared_ptr<SGCore::Threading::Task> SGCore::Threading::Thread::createTask() const noexcept
{
    return std::make_shared<Task>();
}

void SGCore::Threading::Thread::addTask(std::shared_ptr<Task> task)
{
    const size_t taskGuardHash = hashObject(task->m_parentTaskGuard);
    
    std::lock_guard guard(m_threadProcessMutex);
    
    if(!onTasksProcess.contains(taskGuardHash))
    {
        m_tasks.push_back(task);
        
        onTasksProcess += task->m_onExecuteListener;
    }
}

void SGCore::Threading::Thread::removeTask(std::shared_ptr<Task> task)
{
    if(!task) return;
    
    std::lock_guard guard(m_threadProcessMutex);
    
    std::erase_if(m_tasks, [&task](std::shared_ptr<Task> w) {
        return w == task;
    });
    
    onTasksProcess -= task->m_onExecuteListener;
}

size_t SGCore::Threading::Thread::tasksCount() noexcept
{
    std::lock_guard guard(m_threadProcessMutex);
    
    return m_tasks.size();
}

void SGCore::Threading::Thread::processFinishedTasks() noexcept
{
    std::vector<std::shared_ptr<Task>> finishedTasksCopy;
    
    {
        std::lock_guard guard(m_tasksEndCopyMutex);
        
        finishedTasksCopy = m_finishedTasksToExecute;
    }
    
    for(const auto& task : finishedTasksCopy)
    {
        task->m_onExecutedCallback();
    }
    
    {
        std::lock_guard guard(m_tasksEndCopyMutex);
        
        exclude(finishedTasksCopy, m_finishedTasksToExecute);
    }
}

double SGCore::Threading::Thread::getExecutionTime() const noexcept
{
    return m_executionTime.load();
}

std::thread::id SGCore::Threading::Thread::getNativeID() const noexcept
{
    return m_nativeThreadID.load();
}

bool SGCore::Threading::Thread::isRunning() const noexcept
{
    return m_isRunning;
}
