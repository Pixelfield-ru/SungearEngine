//
// Created by ilya on 10.02.24.
//

#ifndef SGSLETRANSLATOR_SGSLEEXPR_H
#define SGSLETRANSLATOR_SGSLEEXPR_H

#include <string>
#include <vector>
#include <functional>
#include "SGSLEAssignExpression.h"

namespace SGCore
{
    struct SGSLEVariable
    {
        std::string m_lValueVarName;
        std::string m_lValueVarType;
        bool m_isLValueArray = false;
        int m_lValueArraySize = 0;
        std::vector<SGSLEAssignExpression> m_assignExpressions;
        
        bool operator==(const SGSLEVariable& other) const noexcept
        {
            return m_lValueVarName == other.m_lValueVarName;
        }
        
        bool operator!=(const SGSLEVariable& other) const noexcept
        {
            return !(*this == other);
        }
    };
}

#endif //SGSLETRANSLATOR_SGSLEEXPR_H
