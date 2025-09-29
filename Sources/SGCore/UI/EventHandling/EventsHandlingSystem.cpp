//
// Created by stuka on 29.09.2025.
//

#include "EventsHandlingSystem.h"

#include "UIEventHandler.h"
#include "UIEventHandlersRegistry.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/UI/UIComponent.h"

void SGCore::UI::EventsHandlingSystem::update(const double& dt, const double& fixedDt) noexcept
{
    auto registry = getScene()->getECSRegistry();

    auto uiView = registry->view<UIComponent>();

    uiView.each([&](UIComponent& uiComponent) {
        bool stopToken = false;
        uiComponent.m_document->iterate([&](UIElement* parent, UIElement* current) {
            current->m_currentFrameStyles.clear();

            for(const auto& eventHandler : UIEventHandlersRegistry::getHandlers())
            {
                bool handled = eventHandler->testElement(*current);
                if(handled && eventHandler->isChangesStyle())
                {
                    if(!current->m_mainStyle) continue;

                    auto pseudoClassStyle = current->m_mainStyle->m_pseudoClassesStyles[eventHandler->getEventNameHash()];
                    if(pseudoClassStyle) continue;

                    current->m_currentFrameStyles.push_back(pseudoClassStyle.get());
                    // eventHandler->applyStyle(*current, current->getStyleForPseudoClass(eventHandler->getEventNameHash()));
                }
            }

            if(current->m_currentFrameStyles.empty())
            {
                current->m_currentFrameStyles.push_back(current->m_mainStyle.get());
            }
        }, stopToken);
    });
}
