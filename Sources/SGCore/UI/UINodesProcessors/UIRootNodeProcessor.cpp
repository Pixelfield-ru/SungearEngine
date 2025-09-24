//
// Created by stuka on 24.09.2025.
//

#include "UIRootNodeProcessor.h"

#include "SGCore/UI/Elements/UIRoot.h"

SGCore::Ref<SGCore::UI::UIElement> SGCore::UI::UIRootNodeProcessor::allocateElement() noexcept
{
    return MakeRef<UIRoot>();
}
