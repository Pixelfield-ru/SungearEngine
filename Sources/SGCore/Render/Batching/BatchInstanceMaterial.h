//
// Created by stuka on 15.06.2025.
//

#ifndef SUNGEARENGINE_BATCHINSTANCEMATERIAL_H
#define SUNGEARENGINE_BATCHINSTANCEMATERIAL_H

#include <entt/entity/entity.hpp>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
#pragma pack(push, 1)
    struct BatchInstanceMaterial
    {
        // 7 is rgba elements count in BatchInstanceTransform
        static constexpr std::uint32_t components_count = 6;

        glm::vec4 m_diffuseColor        = glm::vec4(1.0f);
        glm::vec4 m_specularColor       = glm::vec4(1.0f);
        glm::vec4 m_ambientColor        = glm::vec4(0.0f);
        glm::vec4 m_emissionColor       = glm::vec4(1.0f);
        glm::vec4 m_transparentColor    = glm::vec4(1.0f);
        glm::vec3 m_shininessMetallicRoughness = glm::vec3(32.0f, 0.5f, 0.2f);
        float padding0;
    };
#pragma pack(pop)
}

#endif // SUNGEARENGINE_BATCHINSTANCEMATERIAL_H
