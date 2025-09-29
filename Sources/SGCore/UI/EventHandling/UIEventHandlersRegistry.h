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
        static const std::vector<Ref<UIEventHandler>>& getHandlers() noexcept;

    private:
        static inline std::vector<Ref<UIEventHandler>> s_handlers;

        static void initializeCoreHandlers() noexcept;

        static inline bool s_staticInit = []() {
            UIEventHandlersRegistry::initializeCoreHandlers();
            return true;
        }();
    };
}
