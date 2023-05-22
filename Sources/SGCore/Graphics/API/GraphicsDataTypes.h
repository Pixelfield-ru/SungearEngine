//
// Created by stuka on 22.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GRAPHICSDATATYPES_H
#define SUNGEARENGINE_GRAPHICSDATATYPES_H

    /*
    typedef unsigned int SGGDataType;

    static const SGGDataType SHADER_DATA_TYPE_NONE = 0;

    static const SGGDataType SHADER_DATA_TYPE_T_INT = 1;
    static const SGGDataType SHADER_DATA_TYPE_T_INT2 = 2;
    static const SGGDataType SHADER_DATA_TYPE_T_INT3 = 3;
    static const SGGDataType SHADER_DATA_TYPE_T_INT4 = 4;

    static const SGGDataType SHADER_DATA_TYPE_T_FLOAT = 5;
    static const SGGDataType SHADER_DATA_TYPE_T_FLOAT2 = 6;
    static const SGGDataType SHADER_DATA_TYPE_T_FLOAT3 = 7;
    static const SGGDataType SHADER_DATA_TYPE_T_FLOAT4 = 8;

    static const SGGDataType SHADER_DATA_TYPE_T_MAT2 = 9;
    static const SGGDataType SHADER_DATA_TYPE_T_MAT3 = 10;
    static const SGGDataType SHADER_DATA_TYPE_T_MAT4 = 11;

    static const SGGDataType SHADER_DATA_TYPE_T_BOOLEAN = 12;
     */

    enum SGGDataType
    {
        SGG_DATA_TYPE_NONE,

        SGG_DATA_TYPE_INT,
        SGG_DATA_TYPE_INT2,
        SGG_DATA_TYPE_INT3,
        SGG_DATA_TYPE_INT4,

        SGG_DATA_TYPE_FLOAT,
        SGG_DATA_TYPE_FLOAT2,
        SGG_DATA_TYPE_FLOAT3,
        SGG_DATA_TYPE_FLOAT4,

        SGG_DATA_TYPE_MAT2,
        SGG_DATA_TYPE_MAT3,
        SGG_DATA_TYPE_MAT4,

        SGG_DATA_TYPE_BOOL,
    };

#endif //SUNGEARENGINE_GRAPHICSDATATYPES_H
