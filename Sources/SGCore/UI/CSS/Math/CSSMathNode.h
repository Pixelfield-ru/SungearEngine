//
// Created by stuka on 20.01.2025.
//

#ifndef SUNGEARENGINE_CSSMATHNODE_H
#define SUNGEARENGINE_CSSMATHNODE_H

#include "SGCore/Main/CoreGlobals.h"
#include "CSSMathSign.h"
#include "SGCore/UI/CSS/CSSDimensionQualifier.h"

namespace SGCore::UI
{
    struct CSSSelector;

    struct CSSMathNode
    {
        std::vector<Ref<CSSMathNode>> m_operands;
        CSSMathSign m_sign = CSSMathSign::MS_NO_SIGN;
        /// Can be only PLUS or MINUS.
        CSSMathSign m_unarySign = CSSMathSign::MS_PLUS;
        CSSDimensionQualifier m_dimensionQualifier = CSSDimensionQualifier::DQ_NUMBER;

        [[nodiscard]] virtual float calculate(const float* parentSelectorValue) const noexcept;

        /**
         * Iterates through current (and child node if \p recursed flag is enabled) and groups all continuous sequences of numeric nodes with
         * priority sign (\p * or \p /) into one CSSMathNode.
         * @param recursed - Is recursively run through the children is needed.
         */
        void resolvePriorities(bool recursed = true) noexcept;
    };
}

#endif //SUNGEARENGINE_CSSMATHNODE_H
