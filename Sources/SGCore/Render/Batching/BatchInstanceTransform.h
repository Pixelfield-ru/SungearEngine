//
// Created by stuka on 15.06.2025.
//

#ifndef SUNGEARENGINE_BATCHINSTANCETRANSFORM_H
#define SUNGEARENGINE_BATCHINSTANCETRANSFORM_H

#include <glm/mat4x4.hpp>

namespace SGCore
{
#pragma pack(push, 1)
    struct BatchInstanceTransform
    {
        glm::mat4 m_modelMatrix = glm::mat4(1.0f);
        glm::vec3 m_position { 0.0f };
        float padding0;
        glm::vec3 m_rotation { 0.0f };
        float padding1;
        glm::vec3 m_scale { 1.0f };
        float padding2;
    };
#pragma pack(pop)
}

#endif // SUNGEARENGINE_BATCHINSTANCETRANSFORM_H
