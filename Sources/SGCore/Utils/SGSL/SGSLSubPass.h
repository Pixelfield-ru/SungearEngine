//
// Created by stuka on 28.12.2023.
//

#ifndef SGSLINTERPRETER_SGSLSUBPASS_H
#define SGSLINTERPRETER_SGSLSUBPASS_H

#include "SGSLSubShaderType.h"
#include "SGSLSubShader.h"
#include "SGSLVariable.h"
#include <set>

namespace SGCore
{
    struct SGSLSubPass : SGSLBracketsListener
    {
        std::string m_name;
        std::string m_globalCode;
        std::unordered_map<SGSLSubShaderType, SGSLSubShader> m_subShaders;
        std::set<SGSLVariable> m_variables;

        bool isSubShaderExists(SGSLSubShaderType subShaderType) const noexcept
        {
            return m_subShaders.find(subShaderType) != m_subShaders.end();
        }
    };
}

#endif //SGSLINTERPRETER_SGSLSUBPASS_H
