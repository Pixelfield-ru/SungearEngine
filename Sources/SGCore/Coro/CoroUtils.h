//
// Created by stuka on 04.08.2025.
//

#ifndef SUNGEARENGINE_COROUTILS_H
#define SUNGEARENGINE_COROUTILS_H

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Threading/ThreadsManager.h"

namespace SGCore::Coro
{
    struct CoroUtils
    {
        static Ref<Threading::Thread> assumeCurrentThread() noexcept;
    };
}

#endif // SUNGEARENGINE_COROUTILS_H
