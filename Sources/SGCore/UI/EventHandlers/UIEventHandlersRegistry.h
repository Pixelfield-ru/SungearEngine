//
// Created by stuka on 29.09.2025.
//

#pragma once

#include <vector>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::UI
{
    struct UIEventHandler;

    struct UIEventHandlersRegistry
    {
    private:
        static inline std::vector<Ref<UIEventHandler>> s_nodesProcessors;

        static void initializeCoreProcessors() noexcept;

        static inline bool s_staticInit = []() {
            UIEventHandlersRegistry::initializeCoreProcessors();
            return true;
        }();
    };
}
