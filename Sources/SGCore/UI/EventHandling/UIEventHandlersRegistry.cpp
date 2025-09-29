//
// Created by stuka on 29.09.2025.
//

#include "UIEventHandlersRegistry.h"

const std::vector<SGCore::Ref<SGCore::UI::UIEventHandler>>& SGCore::UI::UIEventHandlersRegistry::getHandlers() noexcept
{
    return s_handlers;
}

void SGCore::UI::UIEventHandlersRegistry::initializeCoreHandlers() noexcept
{

}
