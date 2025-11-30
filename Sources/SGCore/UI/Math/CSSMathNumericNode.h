//
// Created by stuka on 20.01.2025.
//

#ifndef SUNGEARENGINE_CSSMATHNUMERICNODE_H
#define SUNGEARENGINE_CSSMATHNUMERICNODE_H

#include "CSSMathNode.h"

namespace SGCore::UI
{
    struct CSSMathNumericNode : StyleMathNode
    {
        float m_value = 0.0f;

        [[nodiscard]] float calculate(const float* parentSelectorValue) const noexcept final;
    };
}

#endif //SUNGEARENGINE_CSSMATHNUMERICNODE_H
