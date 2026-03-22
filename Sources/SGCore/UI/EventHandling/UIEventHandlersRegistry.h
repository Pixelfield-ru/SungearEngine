//
// Created by stuka on 29.09.2025.
//

#pragma once

#include <vector>
#include <sgcore_export.h>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::UI
{
    struct UIEventHandler;

    struct SGCORE_EXPORT UIEventHandlersRegistry
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
