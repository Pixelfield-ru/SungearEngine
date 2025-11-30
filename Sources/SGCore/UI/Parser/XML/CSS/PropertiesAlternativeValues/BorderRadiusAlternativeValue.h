//
// Created by stuka on 22.02.2025.
//

#ifndef BORDERRADIUSALTERNATIVEVALUE_H
#define BORDERRADIUSALTERNATIVEVALUE_H

#include "SGCore/UI/Math/CSSMathNode.h"

namespace SGCore::UI
{
    struct BorderRadiusAlternativeValue final
    {
        Ref<StyleMathNode> m_radiusX;
        Ref<StyleMathNode> m_radiusY;
    };
}

#endif //BORDERRADIUSALTERNATIVEVALUE_H
