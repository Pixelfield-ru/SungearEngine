//
// Created by stuka on 05.10.2025.
//

#include "Button.h"

SGCore::Ref<SGCore::UI::UIElement> SGCore::UI::Button::copy() const noexcept
{
    auto element = MakeRef<Button>();
    Div::doCopy(element);

    return element;
}
