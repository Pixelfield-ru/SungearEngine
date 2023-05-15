//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_TRANSFORMCOMPONENT_H
#define NATIVECORE_TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

#include "../IComponent.h"
#include "TransformationsSystem.h"

namespace Core::ECS::Transformations
{
    class TransformComponent : IComponent
    {
        friend class TransformationsSystem;

    private:
        glm::vec3 m_lastPosition;
        glm::vec3 m_lastRotation;
        glm::vec3 m_lastScale;

        glm::vec3 m_lastCenter;

    public:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

        glm::mat4 m_translationMatrix;
        glm::mat4 m_rotationMatrix;
        glm::mat4 m_scaleMatrix;

        glm::mat4 m_modelMatrix;
    };
}

#endif //NATIVECORE_TRANSFORMCOMPONENT_H
