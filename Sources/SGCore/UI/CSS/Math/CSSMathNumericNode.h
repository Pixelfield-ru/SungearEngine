//
// Created by stuka on 20.01.2025.
//

#pragma once

#include <sgcore_export.h>

#include "CSSMathNode.h"

namespace SGCore::UI
{
    struct CSSMathNumericNode : CSSMathNode
    {
        float m_value = 0.0f;

        [[nodiscard]] SGCORE_EXPORT float calculate(const float* parentSelectorValue) const noexcept final;
    };
}
