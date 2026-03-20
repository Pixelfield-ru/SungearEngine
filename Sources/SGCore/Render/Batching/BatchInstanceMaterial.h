//
// Created by stuka on 15.06.2025.
//

#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <sgcore_export.h>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
#pragma pack(push, 1)
    struct SGCORE_EXPORT BatchInstanceMaterial
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
