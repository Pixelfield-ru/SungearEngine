#include <iostream>
#include <memory>
#include "Thread.h"

void executableFunc()
{
    // for(size_t i = 0; i < 1'000'000'000; ++i)
    {
        // std::cout << "worker exec" << std::endl;
    }
}

void onWorkerDone(std::shared_ptr<SGCore::IWorker> worker)
{
    std::cout << "worker done : " << worker << "" << std::endl;
}

int main()
{
    std::cout << "dfdfdf" << std::endl;
    
    SGCore::Thread thread;

    static const SGCore::WorkerGuard workerGuard = SGCore::MakeWorkerGuard();
    static const SGCore::WorkerGuard workerGuard0 = SGCore::MakeWorkerGuard();

    auto work0 = thread.addWorker<&executableFunc, &onWorkerDone>(workerGuard);
    auto work1 = thread.addWorker(workerGuard0, []()
    {
        std::cout << "df" << std::endl;
    }, onWorkerDone);

    std::cout << "worker0: " << work0 << std::endl;
    std::cout << "worker1: " << work1 << std::endl;
    
    thread.start();
    
    thread.removeWorker(work1);
    
    std::cout << "aga" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::duration<float>(3.0));
    
    std::cout << "Hello, World! " << std::endl;
    return 0;
}
