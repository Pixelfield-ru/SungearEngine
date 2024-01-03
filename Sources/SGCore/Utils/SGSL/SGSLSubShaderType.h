//
// Created by stuka on 28.12.2023.
//

#ifndef SGSLINTERPRETER_SGSLSUBSHADERTYPE_H
#define SGSLINTERPRETER_SGSLSUBSHADERTYPE_H

namespace SGCore
{
    enum SGSLSubShaderType
    {
        SST_NONE,
        SST_VERTEX,
        SST_FRAGMENT,
        SST_GEOMETRY,
        SST_COMPUTE,
        SST_TESS_CONTROL,
        SST_TESS_EVALUATION
    };
}

#endif //SGSLINTERPRETER_SGSLSUBSHADERTYPE_H
