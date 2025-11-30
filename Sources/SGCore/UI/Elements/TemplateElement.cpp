//
// Created by stuka on 08.10.2025.
//

#include "TemplateElement.h"

SGCore::Ref<SGCore::UI::UIElement> SGCore::UI::TemplateElement::copy() const noexcept
{
    auto element = MakeRef<TemplateElement>();
    UIElement::doCopy(element);

    element->m_attributes = m_attributes;

    return element;
}

void SGCore::UI::TemplateElement::doCalculateLayout(const UIElementCache* parentSelectorCache,
                                                    UIElementCache& thisSelectorCache,
                                                    const Transform* parentTransform,
                                                    Transform& ownTransform) noexcept
{
}

void SGCore::UI::TemplateElement::doGenerateMesh(const UIElementCache* parentElementCache,
                                                 UIElementCache& thisElementCache) noexcept
{
}
