#include <iostream>
#include <memory>
#include "Thread.h"

void executableFunc()
{
    for(size_t i = 0; i < 1'000; ++i)
    {
        // std::cout << "worker exec" << std::endl;
    }
}

void onWorkerDone(std::shared_ptr<SGCore::IWorker> worker)
{
    std::cout << "worker done" << std::endl;
}

int main()
{
    std::cout << "dfdfdf" << std::endl;
    
    SGCore::Thread thread;
    
    std::cout << "worker0: " << thread.addWorker<&executableFunc, &onWorkerDone>() << std::endl;
    std::cout << "worker1: " << thread.addWorker<&executableFunc, &onWorkerDone>() << std::endl;
    
    thread.start();
    
    sleep(3);
    
    std::cout << "Hello, World! " << std::endl;
    return 0;
}
