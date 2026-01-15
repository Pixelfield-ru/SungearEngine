//
// Created by stuka on 12.01.2026.
//

#include "SGCore/Coro/CoroScheduler.h"
#include "SGCore/Coro/Task.h"
#include "SGCore/Main/CoreMain.h"

SGCore::Coro::Task<> spawn()
{
    co_await SGCore::Coro::returnToCaller();

    std::cout << "spawn" << std::endl;

    for(size_t i = 0; i < 1'000'000; ++i)
    {

    }
}

SGCore::Coro::Task<> runSpawners()
{
    std::cout << "runSpawners" << std::endl;

    for(int i = 0; i < 3; ++i)
    {
        std::cout << "running spawn..." << std::endl;
        spawn();
    }

    co_return;
}

void coreInit()
{
    runSpawners();
    // runSpawners();

    std::cout << "coreInit" << std::endl;
}

void onUpdate(double dt, double fixedDt)
{

}

void onFixedUpdate(double dt, double fixedDt)
{

}

int main()
{
    SGCore::CoreMain::onInit.connect<&coreInit>();
    SGCore::CoreMain::getRenderTimer().onUpdate.connect<&onUpdate>();
    SGCore::CoreMain::getFixedTimer().onUpdate.connect<&onFixedUpdate>();

    SGCore::CoreMain::init();
    SGCore::CoreMain::startCycle();

    return 0;
}
