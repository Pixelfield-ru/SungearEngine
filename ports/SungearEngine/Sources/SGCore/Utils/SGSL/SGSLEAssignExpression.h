//
// Created by ilya on 11.02.24.
//

#ifndef SGSLETRANSLATOR_SGSLEASSIGNEXPRESSION_H
#define SGSLETRANSLATOR_SGSLEASSIGNEXPRESSION_H

#include <cstddef>
#include <vector>
#include <string>

namespace SGCore
{
    struct SGSLEAssignExpression
    {
        std::vector<size_t> m_arrayIndices;
        std::string m_rvalueFunctionName;
        std::vector<std::string> m_rvalueFunctionArgs;
    };
}

#endif //SGSLETRANSLATOR_SGSLEASSIGNEXPRESSION_H
