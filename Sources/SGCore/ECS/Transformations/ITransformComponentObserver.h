//
// Created by stuka on 26.10.2023.
//

#ifndef SUNGEARENGINE_ITRANSFORMCOMPONENTOBSERVER_H
#define SUNGEARENGINE_ITRANSFORMCOMPONENTOBSERVER_H

#include "glm/vec3.hpp"

namespace Core::ECS
{
    struct ITransformComponentObserver
    {
        glm::vec3 m_transformLastPosition { 0.0 };
        glm::vec3 m_transformLastRotation { 0.0 };
        glm::vec3 m_transformLastScale { 0.0 };
    };
}

#endif //SUNGEARENGINE_ITRANSFORMCOMPONENTOBSERVER_H
