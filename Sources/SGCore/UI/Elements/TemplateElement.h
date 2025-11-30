//
// Created by stuka on 08.10.2025.
//

#pragma once

#include "SGCore/UI/UIElement.h"
#include "TemplateAttribute.h"

namespace SGCore::UI
{
    struct TemplateElement : UIElement
    {
        SG_DECLARE_UI_ELEMENT_TYPE(template)

        std::unordered_map<std::string, TemplateAttribute> m_attributes;

        [[nodiscard]] Ref<UIElement> copy() const noexcept override;

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
