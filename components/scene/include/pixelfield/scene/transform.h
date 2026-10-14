//
// Created by stuka on 14.10.2026.
//

#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

#include "pixelfield/core/math/constants.h"

namespace pf::scene
{
    struct Transform final
    {
        glm::vec3 position {};
        glm::quat rotation {};
        glm::vec3 scale {};

        glm::vec3 right = core::right3;
        glm::vec3 forward = core::forward3;
        glm::vec3 up = core::up3;

        glm::mat4 model {};
    };

    struct TransformComponent final
    {
        Transform local;
        Transform world;
    };
}