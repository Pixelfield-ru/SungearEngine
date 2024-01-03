//
// Created by stuka on 28.12.2023.
//

#ifndef SGSLINTERPRETER_SGSLSUBSHADER_H
#define SGSLINTERPRETER_SGSLSUBSHADER_H

#include "SGSLSubShaderType.h"
#include "SGSLBracketsListener.h"

namespace SGCore
{
    struct SGSLSubShader : SGSLBracketsListener
    {
        std::string m_name;
        std::string m_code;
        SGSLSubShaderType m_type = SGSLSubShaderType::SST_NONE;
    };
}

#endif //SGSLINTERPRETER_SGSLSUBSHADER_H
