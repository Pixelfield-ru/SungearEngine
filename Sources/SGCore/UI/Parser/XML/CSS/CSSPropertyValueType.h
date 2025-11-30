//
// Created by stuka on 17.01.2025.
//

#ifndef SUNGEARENGINE_CSSPROPERTYVALUETYPE_H
#define SUNGEARENGINE_CSSPROPERTYVALUETYPE_H

namespace SGCore::UI
{
    enum class CSSPropertyValueType
    {
        PVT_ENUM,
        PVT_FLOAT,
        PVT_FVEC2,
        PVT_FVEC3,
        PVT_FVEC4,
        PVT_INT,
        PVT_IVEC2,
        PVT_IVEC3,
        PVT_IVEC4,

        PVT_UNKNOWN
    };
}

#endif //SUNGEARENGINE_CSSPROPERTYVALUETYPE_H
