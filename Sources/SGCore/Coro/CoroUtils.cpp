//
// Created by stuka on 04.08.2025.
//

#include "CoroUtils.h"

#include <iostream>

#include "SGCore/Logger/Logger.h"

SGCore::Ref<SGCore::Threading::Thread> SGCore::Coro::CoroUtils::assumeCurrentThread() noexcept
{
    const auto currentThread = Threading::ThreadsManager::currentThread();
    if(!currentThread)
    {
        const std::string msg = fmt::format("SGCore::Coro : Thread with id '{}' does not exist in Sungear Engine Core Threads Manager. Please, use Sungear Engine Threading.", std::hash<std::thread::id>()(std::this_thread::get_id()));
        LOG_C_UNFORMATTED(SGCORE_TAG, msg);
        std::cerr << msg << std::endl;
        std::terminate();
    }

    return currentThread;
}
