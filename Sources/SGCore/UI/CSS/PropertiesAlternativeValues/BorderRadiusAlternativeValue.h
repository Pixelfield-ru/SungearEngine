//
// Created by stuka on 22.02.2025.
//

#pragma once

#include <sgcore_export.h>

#include "SGCore/UI/CSS/Math/CSSMathNode.h"

namespace SGCore::UI
{
    struct SGCORE_EXPORT BorderRadiusAlternativeValue final
    {
        Ref<CSSMathNode> m_radiusX;
        Ref<CSSMathNode> m_radiusY;
    };
}
