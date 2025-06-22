//
// Created by stuka on 16.06.2025.
//

#ifndef SUNGEARENGINE_BATCHVERTEX_H
#define SUNGEARENGINE_BATCHVERTEX_H

#include <glm/vec3.hpp>

namespace SGCore
{
#pragma pack(push, 1)
    struct BatchVertex
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

#endif // SUNGEARENGINE_BATCHVERTEX_H
