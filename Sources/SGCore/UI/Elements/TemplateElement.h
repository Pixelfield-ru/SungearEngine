//
// Created by stuka on 08.10.2025.
//

#pragma once
#include "SGCore/UI/UIElement.h"

namespace SGCore::UI
{
    struct TemplateElement : UIElement
    {
        SG_DECLARE_UI_ELEMENT_TYPE(template)

        TemplateElement() noexcept = default;

        copy_constructor(TemplateElement) = default;
        move_constructor(TemplateElement) = default;

        copy_operator(TemplateElement) = default;
        move_operator(TemplateElement) = default;

    protected:
        /**
         * Does nothing.
         * @param parentSelectorCache
         * @param thisSelectorCache
         * @param parentTransform
         * @param ownTransform
         */
        void doCalculateLayout(const UIElementCache* parentSelectorCache, UIElementCache& thisSelectorCache,
                               const Transform* parentTransform, Transform& ownTransform) noexcept final;

        /**
         * Does nothing.
         * @param parentElementCache
         * @param thisElementCache
         */
        void doGenerateMesh(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept final;
    };
}
