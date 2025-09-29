//
// Created by stuka on 27.09.2025.
//

#include "UIInputListener.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Input/InputManager.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/UI/UIComponent.h"

void SGCore::UI::UIInputListener::update(const double& dt, const double& fixedDt)
{
    auto scene = getScene();

    auto uiComponentsView = scene->getECSRegistry()->view<UIComponent>();

    uiComponentsView.each([&](UIComponent::reg_t& uiComponent) {
        if(!uiComponent.m_attachedToCamera.m_referencedEntity) return;

        auto* cameraReceiver = scene->getECSRegistry()->tryGet<LayeredFrameReceiver>(*uiComponent.m_attachedToCamera);

        if(!cameraReceiver) return;

        // reading from attachment with picking color
        const auto pickedColor = cameraReceiver->m_pickingColorUnderMouse;

        bool breakFind = false;
        uiComponent.m_document->iterate([&](UIElement* parent, UIElement* current) {
            if((glm::vec3) current->m_uniqueColor.color() == pickedColor)
            {
                std::cout << "hovering element: " << current->m_name << std::endl;
                breakFind = true;
            }
        }, breakFind);
    });
}
