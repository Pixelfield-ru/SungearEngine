#include "Button.h"

std::unique_ptr<SGCore::UI::UIElement> SGCore::UI::Button::copy() const noexcept
{
    auto element = MakeScope<Button>();
    Div::doCopy(*element);

    return element;
}
