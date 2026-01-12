//
// Created by ilya on 15.02.24.
//

#ifndef SUNGEARENGINE_MATHUTILS_H
#define SUNGEARENGINE_MATHUTILS_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <functional>

#define PI 3.141592

namespace SGCore
{
    struct MathUtils
    {
        constexpr const static glm::vec3 left3 = glm::vec3(1, 0, 0); // unused by TU PW
        constexpr const static glm::vec3 right3 = glm::vec3(-1, 0, 0);
        constexpr const static glm::vec3 forward3 = glm::vec3(0, 0, -1);
        constexpr const static glm::vec3 up3 = glm::vec3(0, 1, 0);

        static glm::vec2 raySphereIntersection(const glm::vec3& ro, const glm::vec3& rd, const float& sr) noexcept;
        
        template<glm::length_t N, typename T, glm::qualifier Q>
        static size_t hashVector(const glm::vec<N, T, Q>& vec) noexcept
        {
            return std::hash<T>()(vec.x) ^ std::hash<T>()(vec.y);
        }
        
        template<typename VecT>
        struct GLMVectorHash;
        
        template<glm::length_t N, typename T, glm::qualifier Q>
        struct GLMVectorHash<glm::vec<N, T, Q>>
        {
            size_t operator()(const glm::vec<N, T, Q>& v) const
            {
                return hashVector(v);
            }
        };

        template<typename T>
        static T quinticCurve(const T& t) noexcept
        {
            return t * t * t * (t * (t * 6 - 15) + 10);
        }

        template<typename T>
        static T lerp(const T& a, const T& b, const T& t)
        {
            return a + (b - a) * t;
        }

        template<typename T>
        static T quinticLerp(const T& a, const T& b, const T& t) noexcept
        {
            return lerp(a, b, quinticCurve(t));
        }
    };
}

#endif //SUNGEARENGINE_MATHUTILS_H
