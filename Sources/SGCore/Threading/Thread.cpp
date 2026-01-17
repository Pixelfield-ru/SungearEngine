    //
// Created by ilya on 03.04.24.
//

#include "SGCore/Main/CoreGlobals.h"
#include "Thread.h"

#include <iostream>

#include "ThreadsManager.h"
#include "SGCore/Utils/Utils.h"
#include "Task.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Utils/Time.h"

    std::shared_ptr<SGCore::Threading::Thread> SGCore::Threading::Thread::create(const std::chrono::milliseconds& sleepTime) noexcept
{
    std::lock_guard threadsAccessGuard(ThreadsManager::m_threadAccessMutex);
    
    auto thread = std::shared_ptr<Thread>(new Thread);
    thread->m_sleepTime = sleepTime;
    ThreadsManager::m_threads.push_back(thread);
    
    return thread;
}

SGCore::Threading::Thread::~Thread()
{
    join();
}

void SGCore::Threading::Thread::processTasks() noexcept
{
    using namespace std::chrono_literals;

    auto t0 = now();

    m_coroScheduler.process();

    {
        std::lock_guard copyGuard(m_threadProcessMutex);

        m_tasksCopy = m_tasks;
    }

    for(size_t i = 0; i < m_tasks.size(); i++)
    {
        m_tasks[i]->execute();
    }
    
    size_t tasksCount = 0;
    
    {
        std::lock_guard copyGuard(m_threadProcessMutex);
        
        // exclude from vector
        {
            size_t curIdx = 0;
            // #TODO optimization!
            for(const auto& task : m_tasksCopy)
            {
                if(task->m_isStatic.load()) continue;
                if(curIdx == m_tasks.size()) break;
                
                std::erase(m_tasks, task);
                ++curIdx;
            }
        }
        
        tasksCount = m_tasks.size();
    }
    
    processFinishedTasks();

    onUpdate();

    // THREAD IS FULL FREE
    if(tasksCount == 0 && onUpdate.empty() && m_autoJoinIfNotBusy)
    {
        auto joinThisThreadTask = MakeRef<Threading::Task>();
        joinThisThreadTask->setOnExecuteCallback([threadToJoin = shared_from_this()]() {
             threadToJoin->join();
        });
        ThreadsManager::getMainThread()->addTask(joinThisThreadTask);
    }

    if (m_sleepIfNotBusy)
    {
        std::this_thread::sleep_for(m_sleepTime);
    }

    m_executionTime = timeDiff<double, std::milli>(t0, now());
}

void SGCore::Threading::Thread::start() noexcept
{
    if(m_isRunning || m_hasJoinRequest) return;

    auto internalFunc = [this]() {
        // todo: not thread-safe!
        ThreadsManager::m_threadsMap[m_thread.get_id()] = weak_from_this();

        while(m_isAlive && m_isRunning)
        {
            double lastTime = m_currentTime;
            m_currentTime = (double) Utils::getTimeMilliseconds();

            m_deltaTime = (m_currentTime - lastTime) / 1000.0;

            m_isTasksProcessing = true;
            processTasks();
            m_isTasksProcessing = false;
        }
    };
    
    m_isRunning = true;
    m_thread = std::thread(internalFunc);
    
    m_nativeThreadID = m_thread.get_id();
}

void SGCore::Threading::Thread::join() noexcept
{
    if(!m_isRunning) return;

    // marking that thread must stop from task processing
    m_isRunning = false;

    // PREVENT FROM CALLING start() WHEN THREAD IS NOT JOINED YET BUT NOT RUNNING
    m_hasJoinRequest = true;

    // waiting for tasks process ending...
    while(m_isTasksProcessing) { }

    std::cout << "joining thread..." << std::endl;

    m_thread.join();
    m_hasJoinRequest = false;
}

void SGCore::Threading::Thread::addTask(std::shared_ptr<Task> task)
{
    std::lock_guard guard(m_threadProcessMutex);

    m_tasks.push_back(task);
}

void SGCore::Threading::Thread::removeTask(std::shared_ptr<Task> task)
{
    if(!task) return;
    
    std::lock_guard guard(m_threadProcessMutex);
    
    std::erase_if(m_tasks, [&task](std::shared_ptr<Task> w) {
        return w == task;
    });
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

 double SGCore::Threading::Thread::getDeltaTime() const noexcept
 {
    return m_deltaTime.load();
 }

std::thread::id SGCore::Threading::Thread::getNativeID() const noexcept
{
    return m_nativeThreadID.load();
}

bool SGCore::Threading::Thread::isRunning() const noexcept
{
    return m_isRunning;
}

double SGCore::Threading::MainThread::getDeltaTime() const noexcept
{
    return CoreMain::getRenderTimer().getRawDeltaTime();
}
