//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_TRANSFORMCOMPONENT_H
#define NATIVECORE_TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

#include "SGCore/ECS/IComponent.h"

namespace Core::ECS
{
    class TransformComponent : public IComponent
    {
        friend class TransformationsSystem;

    private:
        void init() noexcept final { }

        glm::vec3 m_lastPosition;
        glm::vec3 m_lastRotation;
        glm::vec3 m_lastScale = glm::vec3(0);

        glm::vec3 m_lastCenter;

    public:
        glm::vec3 m_position { 0.0, 0.0, 0.0 };
        glm::vec3 m_rotation { 0.0, 0.0, 0.0 };
        glm::vec3 m_scale = glm::vec3(1);

        glm::mat4 m_translationMatrix = glm::mat4(1);
        glm::mat4 m_rotationMatrix = glm::mat4(1);
        glm::mat4 m_scaleMatrix = glm::mat4(1);

        glm::mat4 m_modelMatrix = glm::mat4(1);
    };
}

#endif //NATIVECORE_TRANSFORMCOMPONENT_H
