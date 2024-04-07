#include <iostream>
#include <memory>

#include "Thread.h"
#include "ThreadsManager.h"

std::shared_ptr<SGCore::Threading::Thread> thread = std::make_shared<SGCore::Threading::Thread>();
std::shared_ptr<SGCore::Threading::Thread> thread1 = std::make_shared<SGCore::Threading::Thread>();

void executableFunc(std::shared_ptr<SGCore::Threading::IWorker> worker)
{
    std::this_thread::sleep_for(std::chrono::duration<float>(1));
    /*for(size_t i = 0; i < 1'000'000'000; ++i)
    {
        // std::cout << "worker exec" << std::endl;
    }*/
}

void onWorkerDone(std::shared_ptr<SGCore::Threading::IWorker> worker)
{
    std::cout << "worker done : " << worker << ", thread: " << SGCore::Threading::ThreadsManager::currentThread() << std::endl;
}

void loop0()
{
    auto t0 = SGCore::now();
    for(size_t i = 0; i < 1'000'000'000; ++i)
    {
        static const SGCore::Threading::WorkerSingletonGuard workerGuard = SGCore::Threading::MakeWorkerSingletonGuard();

        auto worker0 = thread->createWorker(workerGuard);
        if(worker0)
        {
            worker0->setOnExecuteCallback(executableFunc, worker0);
            worker0->setOnExecutedCallback(onWorkerDone, SGCore::Threading::ThreadsManager::currentThread(), worker0);

            thread->addWorker(worker0);
        }
    }
    auto t1 = SGCore::now();

    std::cout << SGCore::timeDiff<double, std::milli>(t0, t1) << std::endl;
}

void loop1()
{
    while(true)
    {
        SGCore::Threading::ThreadsManager::currentThread()->processFinishedWorkers();
    }
}

int main()
{
    std::cout << "MAIN THREAD: " << SGCore::Threading::ThreadsManager::currentThread() << ", thread0: " << thread << std::endl;

    thread->start();
    thread1->start();

    loop0();
    loop1();

    std::cout << "aga" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::duration<float>(3.0));
    
    std::cout << "Hello, World! " << std::endl;

    return 0;
}
