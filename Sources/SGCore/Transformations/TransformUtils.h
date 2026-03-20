//
// Created by stuka on 06.02.2025.
//

#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <sgcore_export.h>

namespace SGCore
{
    struct Transform;
    struct Rigidbody3D;

    struct SGCORE_EXPORT TransformUtils
    {
        /**
         * Calculates TRS matrices in childTransform for m_ownTransform and for m_finalTransform
         * @param childTransform
         * @param parentTransform Parent transform of childTransform. Can be nullptr (child does not have parent)
         * @return Is m_ownTransform of childTransform has been changed.
         */
        static bool calculateTransform(Transform& childTransform, const Transform* parentTransform) noexcept;

        static glm::mat4 calculateModelMatrix(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) noexcept;
    };
}
