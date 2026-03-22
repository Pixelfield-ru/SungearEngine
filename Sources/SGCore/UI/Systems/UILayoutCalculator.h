//
// Created by stuka on 03.03.2025.
//

#pragma once

#include "SGCore/Scene/ISystem.h"
#include "SGCore/UI/UIComponent.h"

namespace SGCore::UI
{
    struct SGCORE_EXPORT UILayoutCalculator : ISystem
    {
        sg_implement_type_id(SGCore::UI::UILayoutCalculator)

        void fixedUpdate(double dt, double fixedDt) final;

    private:
        /**
         *
         * @param uiComponent
         * @param currentUIElement
         * @param isTreeFormed Indicates if transform tree was formed. False if some new node was created.
         * @return New child transform node index. Returns -1 if new node was created. Else returns bigger then zero number.
         */
        std::int64_t processUIElement(const std::int64_t& parentUITreeNodeIdx,
                                      UIComponent::reg_t& uiComponent,
                                      const Ref<UIElement>& currentUIElement,
                                      const Ref<UIElement>& parentUIElement,
                                      bool& isTreeFormed) noexcept;

        static void calculateElementLayout(bool isFirstChildElement,
                                           const Ref<UIElement>& parentUIElement,
                                           const Ref<UIElement>& currentUIElement,
                                           UITransformTreeElement& parentElementTransform,
                                           UITransformTreeElement& currentElementTransform) noexcept;

        std::int64_t m_currentProceedUIElements = 0;
    };
}
