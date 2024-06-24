//
// Created by stuka on 28.12.2023.
//

#ifndef SGSLINTERPRETER_SGSLSUBSHADERTYPE_H
#define SGSLINTERPRETER_SGSLSUBSHADERTYPE_H

#include <SGCore/pch.h>

namespace SGCore
{
    enum SGSLESubShaderType
    {
        SST_NONE,
        SST_VERTEX,
        SST_FRAGMENT,
        SST_GEOMETRY,
        SST_COMPUTE,
        SST_TESS_CONTROL,
        SST_TESS_EVALUATION
    };
    
    static inline std::string sgsleSubShaderTypeToString(SGSLESubShaderType sgsleSubShaderType) noexcept
    {
        switch(sgsleSubShaderType)
        {
            case SST_NONE:
                return "None";
            case SST_VERTEX:
                return "Vertex";
            case SST_FRAGMENT:
                return "Fragment";
            case SST_GEOMETRY:
                return "Geometry";
            case SST_COMPUTE:
                return "Compute";
            case SST_TESS_CONTROL:
                return "TessControl";
            case SST_TESS_EVALUATION:
                return "TessEvaluation";
        }
    }
    
    static inline SGSLESubShaderType stringToSGSLESubShaderType(const std::string& str)
    {
        if(str == "Vertex")
        {
            return SST_VERTEX;
        }
        else if(str == "Fragment")
        {
            return SST_FRAGMENT;
        }
        else if(str == "Geometry")
        {
            return SST_GEOMETRY;
        }
        else if(str == "Compute")
        {
            return SST_COMPUTE;
        }
        else if(str == "TessControl")
        {
            return SST_TESS_CONTROL;
        }
        else if(str == "TessEvaluation")
        {
            return SST_TESS_EVALUATION;
        }
        
        return SGSLESubShaderType::SST_NONE;
    }
}

#endif //SGSLINTERPRETER_SGSLSUBSHADERTYPE_H
