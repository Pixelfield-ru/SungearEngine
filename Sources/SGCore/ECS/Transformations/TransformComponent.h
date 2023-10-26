//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_TRANSFORMCOMPONENT_H
#define NATIVECORE_TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

#include "SGCore/ECS/IComponent.h"
#include "SGCore/Utils/Math.h"
#include "SGCore/Utils/Utils.h"

namespace Core::ECS
{
    // todo: make quaternion transformations
    class TransformComponent : public IComponent
    {
        friend class TransformationsSystem;
        friend class DirectionalLightsSystem;

    private:
        void init() noexcept final { }

        glm::vec3 m_lastPosition { 0.0, 0.0, 0.0 };
        glm::vec3 m_lastRotation { 0.0, 0.0, 0.0 };
        glm::vec3 m_lastScale = glm::vec3(0);

        glm::vec3 m_lastCenter { 0.0 };

    public:
        glm::vec3 m_position { 0.0, 0.0, 0.0 };
        glm::vec3 m_rotation { 0.0, 0.0, 0.0 };
        glm::vec3 m_scale { 1.0 };

        glm::vec3 m_left = Utils::MathUtils::left3;
        glm::vec3 m_forward = Utils::MathUtils::forward3;
        glm::vec3 m_up = Utils::MathUtils::up3;

        glm::mat4 m_translationMatrix   = glm::mat4(1);
        glm::mat4 m_rotationMatrix = glm::mat4(1);
        glm::mat4 m_scaleMatrix = glm::mat4(1);

        bool m_positionChanged = false;
        bool m_rotationChanged = false;
        bool m_scaleChanged = false;

        glm::mat4 m_modelMatrix = glm::mat4(1);
    };
}

#endif //NATIVECORE_TRANSFORMCOMPONENT_H
