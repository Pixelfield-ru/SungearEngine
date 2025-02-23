//
// Created by stuka on 22.02.2025.
//

#ifndef BORDERRADIUSALTERNATIVEVALUE_H
#define BORDERRADIUSALTERNATIVEVALUE_H

#include "SGCore/UI/CSS/Math/CSSMathNode.h"

namespace SGCore::UI
{
    struct BorderRadiusAlternativeValue final
    {
        Ref<CSSMathNode> m_radiusX;
        Ref<CSSMathNode> m_radiusY;

        bool m_isElliptical = false;
    };
}

#endif //BORDERRADIUSALTERNATIVEVALUE_H
