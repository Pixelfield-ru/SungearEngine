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
        glm::vec3 lastPosition;
        glm::vec3 lastRotation;
        glm::vec3 lastScale;

        glm::vec3 lastCenter;

    public:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        glm::mat4 translationMatrix;
        glm::mat4 rotationMatrix;
        glm::mat4 scaleMatrix;

        glm::mat4 modelMatrix;
    };
}

#endif //NATIVECORE_TRANSFORMCOMPONENT_H
