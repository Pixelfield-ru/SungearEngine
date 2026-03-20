//
// Created by stuka on 04.08.2025.
//

#pragma once

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Threading/ThreadsManager.h"

namespace SGCore::Coro
{
    struct SGCORE_EXPORT CoroUtils
    {
        static Ref<Threading::Thread> assumeCurrentThread() noexcept;
    };
}
