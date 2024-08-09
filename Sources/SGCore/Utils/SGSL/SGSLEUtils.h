//
// Created by ilya on 09.02.24.
//

#ifndef SGSLETRANSLATOR_SGSLEUTILS_H
#define SGSLETRANSLATOR_SGSLEUTILS_H

#include <SGCore/pch.h>

namespace SGCore::SGSLEUtils
{
    static inline std::string sgsleTypeToGLSL(const std::string& sgsleType) noexcept
    {
        if(sgsleType == "SGSampler2D")
        {
            return "sampler2D";
        }
        else if(sgsleType == "SGSamplerCube")
        {
            return "samplerCube";
        }
        
        return "SG_UNKNOWN_TYPE";
    }
}

#endif //SGSLETRANSLATOR_SGSLEUTILS_H
