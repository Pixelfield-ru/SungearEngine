//
// Created by stuka on 01.10.2025.
//

#include "UITransformTree.h"

#include "SGCore/UI/UIDocument.h"

SGCore::UI::UIRoot& SGCore::UI::UITransformTree::Impl::getRootFromDocument(const UIDocument& inDocument) noexcept
{
    return *inDocument.m_rootElement;
}
