//
// Created by ilya on 10.02.24.
//

#ifndef SGSLETRANSLATOR_SGSLESUBPASS_H
#define SGSLETRANSLATOR_SGSLESUBPASS_H

#include <string>
#include <unordered_map>
#include "SGSLESubShaderType.h"
#include "SGSLESubShader.h"
#include "SGSLEBracketsListener.h"

namespace SGCore
{
    struct SGSLESubPass : SGSLEBracketsListener
    {
        std::string m_name;
        std::string m_globalCode;
        std::unordered_map<SGSLESubShaderType, SGSLESubShader> m_subShaders;
        
        bool isSubShaderExists(SGSLESubShaderType subShaderType) const noexcept
        {
            return m_subShaders.find(subShaderType) != m_subShaders.end();
        }
    };
}

#endif //SGSLETRANSLATOR_SGSLESUBPASS_H
