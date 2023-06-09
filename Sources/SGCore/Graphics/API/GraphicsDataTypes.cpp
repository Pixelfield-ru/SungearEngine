#include "GraphicsDataTypes.h"

std::uint16_t getSGGDataTypeSizeInBytes(const SGGDataType& dataType) noexcept
{
    int size;

    switch(dataType)
    {
        case SGG_NONE: size = 0; break;

        case SGG_INT: size = 4; break;
        case SGG_INT2: size = 4 * 2; break;
        case SGG_INT3: size = 4 * 3; break;
        case SGG_INT4: size = 4 * 4; break;

        case SGG_FLOAT: size = 4; break;
        case SGG_FLOAT2: size = 4 * 2; break;
        case SGG_FLOAT3: size = 4 * 3; break;
        case SGG_FLOAT4: size = 4 * 4; break;

        case SGG_MAT2: size = 4 * 2 * 2; break;
        case SGG_MAT3: size = 4 * 3 * 3; break;
        case SGG_MAT4: size = 4 * 4 * 4; break;

        case SGG_BOOL: size = 4; break;

        case SGG_STRUCT_BEGIN: size = 0; break;
        case SGG_STRUCT_END: size = 0; break;

        default: size = 0; break;
    }

    return size;
}