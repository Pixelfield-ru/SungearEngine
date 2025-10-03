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
        auto* cameraFrameReceiver = registry->tryGet<LayeredFrameReceiver>(*uiComponent.m_attachedToCamera.m_referencedEntity);
        if(!cameraFrameReceiver) return;

        bool stopToken = false;
        uiComponent.m_transformTree.iterate([&](UITransformTreeElement* parentTransform, UITransformTreeElement& currentTransform, UIElement* parentUIElement, UIElement& currentUIElement) {
            auto& currentElementCache = currentTransform.m_elementCurrentCache;

            currentElementCache.m_currentFrameStyles.clear();

            for(const auto& eventHandler : UIEventHandlersRegistry::getHandlers())
            {
                bool handled = eventHandler->testElement(currentUIElement, *uiComponent.m_document.get(), *getScene(), *cameraFrameReceiver);
                if(handled && eventHandler->isChangesStyle())
                {
                    if(!currentUIElement.m_mainStyle) continue;

                    auto pseudoClassStyle = currentUIElement.m_mainStyle->m_pseudoClassesStyles[eventHandler->getEventNameHash()];
                    if(!pseudoClassStyle) continue;

                    currentElementCache.m_currentFrameStyles.push_back(pseudoClassStyle.get());
                }
            }

            if(currentElementCache.m_currentFrameStyles.empty() && currentUIElement.m_mainStyle)
            {
                currentElementCache.m_currentFrameStyles.push_back(currentUIElement.m_mainStyle.get());
            }
        },
         *uiComponent.m_document.get(),
        stopToken);
    });
}
