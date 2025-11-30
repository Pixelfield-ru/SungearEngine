//
// Created by stuka on 20.01.2025.
//
#include "CSSMathNumericNode.h"

#include "SGCore/UI/Style/Style.h"

float SGCore::UI::CSSMathNumericNode::calculate(const float* parentSelectorValue) const noexcept
{
    switch(m_dimensionQualifier)
    {
        case CSSDimensionQualifier::DQ_PX: return m_value;
        case CSSDimensionQualifier::DQ_CM: break;
        case CSSDimensionQualifier::DQ_MM: break;
        case CSSDimensionQualifier::DQ_IN: break;
        case CSSDimensionQualifier::DQ_PT: break;
        case CSSDimensionQualifier::DQ_PC: break;
        case CSSDimensionQualifier::DQ_Q: break;
        case CSSDimensionQualifier::DQ_DEG: break;
        case CSSDimensionQualifier::DQ_RAD: break;
        case CSSDimensionQualifier::DQ_GRAD: break;
        case CSSDimensionQualifier::DQ_TURN: break;
        case CSSDimensionQualifier::DQ_MS: break;
        case CSSDimensionQualifier::DQ_S: break;
        case CSSDimensionQualifier::DQ_HZ: break;
        case CSSDimensionQualifier::DQ_KHZ: break;
        case CSSDimensionQualifier::DQ_EM: break;
        case CSSDimensionQualifier::DQ_EX: break;
        case CSSDimensionQualifier::DQ_CH: break;
        case CSSDimensionQualifier::DQ_REM: break;
        case CSSDimensionQualifier::DQ_VW: break;
        case CSSDimensionQualifier::DQ_VH: break;
        case CSSDimensionQualifier::DQ_VMIN: break;
        case CSSDimensionQualifier::DQ_VMAX: break;
        case CSSDimensionQualifier::DQ_DPI: break;
        case CSSDimensionQualifier::DQ_DPCM: break;
        case CSSDimensionQualifier::DQ_DPPX: break;
        case CSSDimensionQualifier::DQ_PERCENT:
            return parentSelectorValue ? *parentSelectorValue / 100.0f * m_value : m_value;
        case CSSDimensionQualifier::DQ_NUMBER: return m_value;
        case CSSDimensionQualifier::DQ_ANY: break;
    }

    return m_value;
}

