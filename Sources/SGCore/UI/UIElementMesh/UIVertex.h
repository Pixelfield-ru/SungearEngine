
//
// Created by stuka on 15.02.2025.
//

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <sgcore_export.h>

namespace SGCore::UI
{
#pragma pack(push, 1)
    struct SGCORE_EXPORT UIVertex
    {
        glm::vec3 m_position;
        glm::vec2 m_uv;

        // for 9-slicing
        std::int32_t m_sliceIndex = 0;
    };
#pragma pack(pop)
}
