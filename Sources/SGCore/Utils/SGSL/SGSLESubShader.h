//
// Created by ilya on 10.02.24.
//

#ifndef SGSLETRANSLATOR_SGSLESUBSHADER_H
#define SGSLETRANSLATOR_SGSLESUBSHADER_H

#include <SGCore/pch.h>

#include "SGSLESubShaderType.h"
#include "SGSLEVariable.h"
#include "SGSLEBracketsListener.h"
#include "SGSLEStruct.h"

namespace SGCore
{
    struct SGSLESubShader : SGSLEBracketsListener
    {
        std::string m_name;
        std::string m_code;
        SGSLESubShaderType m_type = SGSLESubShaderType::SST_NONE;
        std::vector<SGSLEVariable> m_variables;
        
        std::vector<SGSLEStruct> m_structs;
        
        SGSLEStruct* tryGetStruct(const std::string& structName) noexcept
        {
            auto it = std::find(m_structs.begin(), m_structs.end(), SGSLEStruct { structName });
            return it == m_structs.end() ? nullptr : &*it;
        }
        
        SGSLEVariable* tryGetVariable(const std::string& varName) noexcept
        {
            auto it = std::find(m_variables.begin(), m_variables.end(), SGSLEVariable { varName });
            return it == m_variables.end() ? nullptr : &*it;
        }
    };
}

#endif //SGSLETRANSLATOR_SGSLESUBSHADER_H
