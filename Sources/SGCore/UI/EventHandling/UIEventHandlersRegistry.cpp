//
// Created by stuka on 29.09.2025.
//

#include "UIEventHandlersRegistry.h"

#include "UIHoverEventHandler.h"

const std::vector<SGCore::Ref<SGCore::UI::UIEventHandler>>& SGCore::UI::UIEventHandlersRegistry::getHandlers() noexcept
{
    return s_handlers;
}

void SGCore::UI::UIEventHandlersRegistry::initializeCoreHandlers() noexcept
{
    auto hoverEventHandler = MakeRef<UIHoverEventHandler>();
    s_handlers.push_back(hoverEventHandler);
}
