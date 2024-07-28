//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_MESHBUILDER_H
#define SUNGEARENGINE_MESHBUILDER_H

#include <SGCore/pch.h>

#include "MeshBase.h"

namespace SGCore
{
    struct MeshBuilder
    {
        static void buildLine(MeshBase& meshBase, const glm::vec3& start, const glm::vec3& end) noexcept;
        static void buildBox3D(MeshBase& meshBase, const glm::vec3& size) noexcept;
        static void buildSphereVariant1(MeshBase& meshBase, const float& radius, const float& angleIncrement) noexcept;
    };
}

#endif //SUNGEARENGINE_MESHBUILDER_H
