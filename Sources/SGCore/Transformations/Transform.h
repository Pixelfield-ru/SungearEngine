//
// Created by stuka on 03.02.2024.
//

#ifndef SUNGEARENGINE_TRANSFORM_H
#define SUNGEARENGINE_TRANSFORM_H

#include "TransformBase.h"

namespace SGCore
{
    struct Transform
    {
        TransformBase m_finalTransform;
        TransformBase m_ownTransform;

        // will transform follow parent entity`s translation, rotation and scale
        // x - follow translation
        // y - follow rotation
        // z - follow scale
        glm::bvec3 m_followParentTRS { true };
        // glm::bvec3 m_lastFollowParentTRS = glm::vec3 { false };
    };
}

#endif //SUNGEARENGINE_TRANSFORM_H
