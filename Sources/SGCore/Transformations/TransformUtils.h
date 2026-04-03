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
         * Calculates TRS matrices in childTransform for m_localTransform and for m_worldTransform
         * @param childTransform
         * @param parentTransform Parent transform of childTransform. Can be nullptr (child does not have parent)
         * @return Is m_localTransform of childTransform has been changed.
         */
        static bool calculateTransform(Transform& childTransform, const Transform* parentTransform) noexcept;

        [[nodiscard]] static glm::mat4 calculateModelMatrix(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) noexcept;

        [[nodiscard]] static glm::vec3 calculateLocalPosition(const Transform& parentTransform, const glm::vec3& childWorldPosition) noexcept;
        [[nodiscard]] static glm::quat calculateLocalRotation(const Transform& parentTransform, const glm::quat& childWorldRotation) noexcept;

        [[nodiscard]] static glm::vec3 calculateWorldPosition(const Transform& parentTransform, const glm::vec3& childLocalPosition) noexcept;
        [[nodiscard]] static glm::quat calculateWorldRotation(const Transform& parentTransform, const glm::quat& childLocalRotation) noexcept;

        [[nodiscard]] static glm::vec3 screenToWorld(const glm::vec2& screenPosition,
                                                     const glm::vec2& screenSize,
                                                     const glm::mat4& cameraProjection,
                                                     const glm::mat4& cameraView,
                                                     float defaultZ = 0.5f) noexcept;
    };
}
