//
// Created by stuka on 04.01.2025.
//

#ifndef SUNGEARENGINE_VERTEX_H
#define SUNGEARENGINE_VERTEX_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "SGCore/Utils/Utils.h"

namespace SGCore
{
    // 92
    struct Vertex
    {
        static constexpr std::int32_t max_bones_count = 4;

        glm::vec3 m_position;
        glm::vec3 m_uv;
        glm::vec3 m_normal;
        glm::vec3 m_tangent;
        glm::vec3 m_bitangent;

        std::array<std::int32_t, max_bones_count> m_bonesIDs = makeFilledArray<std::int32_t, max_bones_count>(-1);
        std::array<float, max_bones_count> m_bonesWeights = makeFilledArray<float, max_bones_count>(0.0f);

        void addWeightData(const float& weight, const std::int32_t& boneID) noexcept
        {
            for(std::int32_t i = 0; i < max_bones_count; ++i)
            {
                if(m_bonesIDs[i] == -1 && boneID != -1)
                {
                    m_bonesIDs[i] = boneID;
                    m_bonesWeights[i] = weight;

                    return;
                }
            }

            LOG_W(SGCORE_TAG, "Can not add weight data to vertex: there are no free slots for new bones for vertex!");
        }

        /*void setBonesDataToDefault() noexcept
        {
            m_bonesIDs = { -1, -1, -1, -1 };
            m_bonesWeights =  { 0.0, 0.0, 0.0, 0.0 };
        }*/
    };
}

#endif //SUNGEARENGINE_VERTEX_H
