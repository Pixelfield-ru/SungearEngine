//
// Created by ilya on 15.02.24.
//

#ifndef SUNGEARENGINE_MATHUTILS_H
#define SUNGEARENGINE_MATHUTILS_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>

#define PI 3.141592

namespace SGCore
{
    struct MathUtils
    {
        static glm::vec2 raySphereIntersection(const glm::vec3& ro, const glm::vec3& rd, const float& sr) noexcept;
        
        template<glm::length_t N, typename T, glm::qualifier Q>
        static size_t hashVector(const glm::vec<N, T, Q>& vec) noexcept
        {
            return std::hash<T>()(vec.x) ^ std::hash<T>()(vec.y);
        }
    };
}

#endif //SUNGEARENGINE_MATHUTILS_H
