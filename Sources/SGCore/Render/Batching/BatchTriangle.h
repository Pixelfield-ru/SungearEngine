//
// Created by stuka on 29.06.2025.
//

#ifndef SUNGEARENGINE_BATCHTRIANGLE_H
#define SUNGEARENGINE_BATCHTRIANGLE_H

#include <array>
#include <glm/vec2.hpp>

#include "SGCore/Graphics/API/GraphicsDataTypes.h"

namespace SGCore
{
#pragma pack(push, 1)
    struct BatchTriangle
    {
        std::int32_t m_meshInstanceID = 0;
        std::int32_t m_triangleID = 0;
        std::array<glm::u32vec2, texture_types_count> m_atlasesUVsOffset { };
        glm::vec2 padding0;
    };
#pragma pack(pop)
}

#endif // SUNGEARENGINE_BATCHTRIANGLE_H
