//
// Created by stuka on 28.01.2025.
//
#include "UITransformTreeElement.h"

#include "UITransformTree.h"
#include "SGCore/UI/UIElement.h"

SGCore::UI::UITransformTreeElement& SGCore::UI::UITransformTreeElement::Impl::getElementFromTransformTree(
    UITransformTree& transformTree,
    std::int64_t index) noexcept
{
    return transformTree.m_elements[index];
}

size_t SGCore::UI::UITransformTreeElement::Impl::getTransformTreeSize(const UITransformTree& transformTree) noexcept
{
    return transformTree.m_elements.size();
}

size_t SGCore::UI::UITransformTreeElement::Impl::getUIElementChildrenCount(const UIElement& element) noexcept
{
    return element.m_children.size();
}

SGCore::UI::UIElement& SGCore::UI::UITransformTreeElement::Impl::getUIElementChild(const UIElement& element, size_t index) noexcept
{
    return *element.m_children[index];
}
