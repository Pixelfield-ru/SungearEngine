//
// Created by Ilya on 09.10.2023.
//

#ifndef SUNGEARENGINE_ASSIMPUTILS_H
#define SUNGEARENGINE_ASSIMPUTILS_H

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <assimp/types.h>

namespace SGUtils::AssimpUtils
{
    static glm::vec4 aiVectorToGLM(const aiColor4D& vec)
    {
        return glm::vec4 { vec.r, vec.g, vec.b, vec.a };
    }

    static glm::vec3 aiVectorToGLM(const aiColor3D& vec)
    {
        return glm::vec3 { vec.r, vec.g, vec.b };
    }

    static glm::vec3 aiVectorToGLM(const aiVector3D& vec)
    {
        return glm::vec3 { vec.x, vec.y, vec.z };
    }

    static glm::vec2 aiVectorToGLM(const aiVector2D& vec)
    {
        return glm::vec2 { vec.x, vec.y };
    }
}

#endif //SUNGEARENGINE_ASSIMPUTILS_H
