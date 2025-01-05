//
// Created by stuka on 04.01.2025.
//

#ifndef SUNGEARENGINE_VERTEX_H
#define SUNGEARENGINE_VERTEX_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace SGCore
{
    // 92
    struct Vertex
    {
        glm::vec3 m_position;
        glm::vec3 m_uv;
        glm::vec3 m_normal;
        glm::vec3 m_tangent;
        glm::vec3 m_bitangent;

        std::array<std::int32_t, 4> m_bonesIDs { -1, -1, -1, -1 };
        std::array<float, 4> m_bonesWeights { 0.0, 0.0, 0.0, 0.0 };

        void setBonesDataToDefault() noexcept
        {
            m_bonesIDs = { -1, -1, -1, -1 };
            m_bonesWeights =  { 0.0, 0.0, 0.0, 0.0 };
        }
    };
}

#endif //SUNGEARENGINE_VERTEX_H
