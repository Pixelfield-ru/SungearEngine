//
// Created by Ilya on 09.10.2023.
//

#ifndef SUNGEARENGINE_ASSIMPUTILS_H
#define SUNGEARENGINE_ASSIMPUTILS_H

#include <SGCore/pch.h>

namespace SGCore::AssimpUtils
{
    static glm::vec4 aiToGLM(const aiColor4D& vec) noexcept
    {
        return glm::vec4 { vec.r, vec.g, vec.b, vec.a };
    }

    static glm::vec3 aiToGLM(const aiColor3D& vec) noexcept
    {
        return glm::vec3 { vec.r, vec.g, vec.b };
    }

    static glm::vec3 aiToGLM(const aiVector3D& vec) noexcept
    {
        return glm::vec3 { vec.x, vec.y, vec.z };
    }

    static glm::vec2 aiToGLM(const aiVector2D& vec) noexcept
    {
        return glm::vec2 { vec.x, vec.y };
    }

    static glm::mat4 aiToGLM(const aiMatrix4x4& aiMat) noexcept
    {
        return glm::mat4(
                aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
                aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
                aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
                aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
        );
    }

    static glm::quat aiToGLM(const aiQuaternion& quaternion) noexcept
    {
        return glm::quat { quaternion.w, quaternion.x, quaternion.y, quaternion.z };
    }
}

#endif //SUNGEARENGINE_ASSIMPUTILS_H
