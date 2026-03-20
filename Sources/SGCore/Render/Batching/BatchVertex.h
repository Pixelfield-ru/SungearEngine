//
// Created by stuka on 16.06.2025.
//

#pragma once

#include <glm/vec3.hpp>
#include <sgcore_export.h>

namespace SGCore
{
#pragma pack(push, 1)
    struct SGCORE_EXPORT BatchVertex
    {
        // 5 rgb elements in struct BatchVertex
        static constexpr std::uint32_t components_count = 5;

        glm::vec3 m_position;
        glm::vec3 m_uv;
        glm::vec3 m_normal;
        glm::vec3 m_tangent;
        glm::vec3 m_bitangent;
    };
#pragma pack(pop)
}
