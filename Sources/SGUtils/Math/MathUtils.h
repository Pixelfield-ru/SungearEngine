//
// Created by ilya on 15.02.24.
//

#ifndef SUNGEARENGINE_MATHUTILS_H
#define SUNGEARENGINE_MATHUTILS_H

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <memory>

#define PI 3.141592

namespace SGCore
{
    struct MathUtils
    {
        constexpr const static glm::vec3 left3 = glm::vec3(-1, 0, 0);
        constexpr const static glm::vec3 forward3 = glm::vec3(0, 0, 1);
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
        
        template<typename ScalarT>
        static bool rayAABBIntersection(const glm::vec<3, ScalarT, glm::defaultp>& rayOrigin,
                                        const glm::vec<3, ScalarT, glm::defaultp>& rayDirection,
                                        const glm::vec<3, ScalarT, glm::defaultp>& aabbMin,
                                        const glm::vec<3, ScalarT, glm::defaultp>& aabbMax,
                                        ScalarT& intersectionLength) noexcept
        {
            glm::vec<3, ScalarT, glm::defaultp> dirfrac;
            dirfrac = 1.0f / rayDirection;
            
            // r.org is origin of ray
            float t1 = (aabbMin.x - rayOrigin.x) * dirfrac.x;
            float t2 = (aabbMax.x - rayOrigin.x) * dirfrac.x;
            
            float t3 = (aabbMin.y - rayOrigin.y) * dirfrac.y;
            float t4 = (aabbMax.y - rayOrigin.y) * dirfrac.y;
            
            float t5 = (aabbMin.z - rayOrigin.z) * dirfrac.z;
            float t6 = (aabbMax.z - rayOrigin.z) * dirfrac.z;
            
            float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
            float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

            // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
            if (tmax < 0)
            {
                intersectionLength = tmax;
                return false;
            }

            // if tmin > tmax, ray doesn't intersect AABB
            if (tmin > tmax)
            {
                intersectionLength = tmax;
                return false;
            }
            
            intersectionLength = tmin;
            
            return true;
        }
        
        template<typename ScalarT>
        static bool lineAABBIntersection(const glm::vec<3, ScalarT, glm::defaultp>& rayOrigin,
                                         const glm::vec<3, ScalarT, glm::defaultp>& rayDirection,
                                         const glm::vec<3, ScalarT, glm::defaultp>& aabbMin,
                                         const glm::vec<3, ScalarT, glm::defaultp>& aabbMax,
                                         const ScalarT& lineLength,
                                         ScalarT& intersectionLength) noexcept
        {
            bool rayIntersection = rayAABBIntersection(rayOrigin, rayDirection, aabbMin, aabbMax, intersectionLength);
            
            return rayIntersection && intersectionLength <= lineLength;
        }
    };
}

#endif //SUNGEARENGINE_MATHUTILS_H
