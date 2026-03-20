//
// Created by stuka on 03.02.2024.
//

#pragma once

#include <glm/mat4x4.hpp>

#include "TransformBase.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    /**
     *
     */
    struct SGCORE_EXPORT Transform : ECS::Component<Ref<Transform>, Ref<const Transform>>
    {
        friend class Node;

        TransformBase m_finalTransform;

        TransformBase m_ownTransform;

        /// Can be glm::identity or boneAnimatedMatrix * boneOffsetMatrix.
        glm::mat4 m_boneMatrix = glm::identity<glm::mat4>();
        glm::mat4 m_boneFinalOffsetMatrix = glm::identity<glm::mat4>();

        // will transform follow parent entity`s translation, rotation and scale
        // x - follow translation
        // y - follow rotation
        // z - follow scale
        glm::vec<3, bool, glm::highp> m_followParentTRS { true, true, true };
        // glm::bvec3 m_lastFollowParentTRS = glm::vec3 { false };

        glm::vec3 m_physicalDeltaTranslation {};
        glm::quat m_physicalDeltaRotation = glm::identity<glm::quat>();
        
        bool m_transformChanged = false;

        bool m_isAnimated = false;

        const auto& getInitialModelMatrix() const noexcept
        {
            return m_initialModelMatrix;
        }

    private:
        // initial model matrix of node in asset
        glm::mat4 m_initialModelMatrix = glm::identity<glm::mat4>();
    };
}
