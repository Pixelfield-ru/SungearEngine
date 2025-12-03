#pragma once

#include "Div.h"

namespace SGCore::UI
{
    struct Button : Div
    {
        SG_DECLARE_UI_ELEMENT_TYPE(button)

        [[nodiscard]] Ref<UIElement> copy() const noexcept override;
    };
}
