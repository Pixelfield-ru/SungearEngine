//
// Created by ilya on 11.04.24.
//

#ifndef SUNGEARENGINE_MATHPRIMITIVESUTILS_H
#define SUNGEARENGINE_MATHPRIMITIVESUTILS_H

#include <glm/glm.hpp>

#include "AABB.h"

namespace SGCore::MathPrimitivesUtils
{
    template<typename ScalarT = float>
    requires(std::is_scalar_v<ScalarT>)
    struct RayIntersectionInfo
    {
        ScalarT m_hitDistance = std::numeric_limits<ScalarT>::max();
        bool m_isIntersected = false;
    };
    
    template<typename ScalarT = float>
    requires(std::is_scalar_v<ScalarT>)
    struct Line3D
    {
        glm::vec<3, ScalarT, glm::defaultp> m_origin = { 0, 0, 0 };
        glm::vec<3, ScalarT, glm::defaultp> m_direction = { 0, 0, 0 };
        ScalarT m_length = 0;
    };
    
    template<typename ScalarT = float>
    requires(std::is_scalar_v<ScalarT>)
    struct Ray3D
    {
        glm::vec<3, ScalarT, glm::defaultp> m_origin = { 0, 0, 0 };
        glm::vec<3, ScalarT, glm::defaultp> m_direction = { 0, 0, 0 };
    };
    
    template<typename ScalarT = float>
    requires(std::is_scalar_v<ScalarT>)
    struct Plane3D
    {
        glm::vec<3, ScalarT, glm::defaultp> m_origin = { 0, 0, 0 };
        glm::vec<3, ScalarT, glm::defaultp> m_normal = { 0, 0, 0 };
    };

    template<typename ScalarT = float>
    requires(std::is_scalar_v<ScalarT>)
    struct Triangle
    {
        using vec3_t = glm::vec<3, ScalarT, glm::defaultp>;
        using vec2_t = glm::vec<2, ScalarT, glm::defaultp>;

        std::array<vec3_t, 3> m_vertices {};
        vec3_t m_normal {};

        void calculateNormal() noexcept
        {
            const auto edge1 = m_vertices[1] - m_vertices[0];
            const auto edge2 = m_vertices[2] - m_vertices[0];
            m_normal = glm::normalize(glm::cross(edge1, edge2));
        }

        vec3_t center() const noexcept
        {
            return (m_vertices[0] + m_vertices[1] + m_vertices[2]) / 3.0f;
        }

        vec2_t minXZ() const noexcept
        {
            return glm::min(glm::min(vec2_t(m_vertices[0].x, m_vertices[0].z),
                                     vec2_t(m_vertices[1].x, m_vertices[1].z)),
                                     vec2_t(m_vertices[2].x, m_vertices[2].z));
        }

        vec2_t maxXZ() const noexcept
        {
            return glm::max(glm::max(vec2_t(m_vertices[0].x, m_vertices[0].z),
                                     vec2_t(m_vertices[1].x, m_vertices[1].z)),
                                     vec2_t(m_vertices[2].x, m_vertices[2].z));
        }
    };
    
    template<typename ScalarT = float>
    static void rayAABBIntersection(const glm::vec<3, ScalarT, glm::defaultp>& rayOrigin,
                                    const glm::vec<3, ScalarT, glm::defaultp>& rayDirection,
                                    const glm::vec<3, ScalarT, glm::defaultp>& aabbMin,
                                    const glm::vec<3, ScalarT, glm::defaultp>& aabbMax,
                                    RayIntersectionInfo<ScalarT>& intersectionInfo) noexcept
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
        // if tmin > tmax, ray doesn't intersect AABB
        if (tmax < 0 || tmin > tmax)
        {
            intersectionInfo.m_hitDistance = tmax;
            intersectionInfo.m_isIntersected = false;
            return;
        }
        
        intersectionInfo.m_hitDistance = tmin;
        intersectionInfo.m_isIntersected = true;
    }
    
    template<typename ScalarT = float>
    static void lineAABBIntersection(const glm::vec<3, ScalarT, glm::defaultp>& rayOrigin,
                                     const glm::vec<3, ScalarT, glm::defaultp>& rayDirection,
                                     const glm::vec<3, ScalarT, glm::defaultp>& aabbMin,
                                     const glm::vec<3, ScalarT, glm::defaultp>& aabbMax,
                                     const ScalarT& lineLength,
                                     RayIntersectionInfo<ScalarT>& intersectionInfo) noexcept
    {
        rayAABBIntersection(rayOrigin, rayDirection, aabbMin, aabbMax, intersectionInfo);
        
        intersectionInfo.m_isIntersected = intersectionInfo.m_isIntersected && intersectionInfo.m_hitDistance <= lineLength;
    }
    
    template<typename ScalarT = float>
    static void rayAABBIntersection(const Ray3D<ScalarT>& ray,
                                    const AABB<ScalarT>& aabb,
                                    RayIntersectionInfo<ScalarT>& intersectionInfo) noexcept
    {
        rayAABBIntersection(ray.m_origin, ray.m_direction, aabb.m_min, aabb.m_max, intersectionInfo);
    }
    
    template<typename ScalarT = float>
    static void lineAABBIntersection(const Line3D<ScalarT>& line,
                                     const AABB<ScalarT>& aabb,
                                     RayIntersectionInfo<ScalarT>& intersectionInfo) noexcept
    {
        rayAABBIntersection(line.m_origin, line.m_direction, aabb.m_min, aabb.m_max, line.m_length, intersectionInfo);
    }
    
    template<typename ScalarT = float>
    static void rayPlaneIntersection(const glm::vec<3, ScalarT, glm::defaultp>& rayOrigin,
                                     const glm::vec<3, ScalarT, glm::defaultp>& rayDirection,
                                     const glm::vec<3, ScalarT, glm::defaultp>& planeOrigin,
                                     const glm::vec<3, ScalarT, glm::defaultp>& planeNormal,
                                     RayIntersectionInfo<ScalarT>& intersectionInfo,
                                     const ScalarT& epsilon = 1e-8)
    {
        ScalarT denom = glm::dot(planeNormal, rayDirection);
        
        if(std::abs(denom) > epsilon)
        {
            ScalarT dist = glm::dot(planeOrigin - rayOrigin, planeNormal) / denom;
            if(dist > 0)
            {
                intersectionInfo.m_hitDistance = dist;
                intersectionInfo.m_isIntersected = true;
                return;
            }
        }
        
        intersectionInfo.m_hitDistance = std::numeric_limits<float>::max();
        intersectionInfo.m_isIntersected = false;
    }

    template<typename ScalarT = float>
    static void rayTriangleIntersection(const glm::vec<3, ScalarT, glm::defaultp>& rayOrigin,
                                        const glm::vec<3, ScalarT, glm::defaultp>& rayDirection,
                                        const glm::vec<3, ScalarT, glm::defaultp>& vertex0,
                                        const glm::vec<3, ScalarT, glm::defaultp>& vertex1,
                                        const glm::vec<3, ScalarT, glm::defaultp>& vertex2,
                                        RayIntersectionInfo<ScalarT>& intersectionInfo,
                                        const ScalarT& epsilon = 1e-8)
    {
        intersectionInfo.m_hitDistance = std::numeric_limits<float>::max();
        intersectionInfo.m_isIntersected = false;

        const auto edge1 = vertex1 - vertex0;
        const auto edge2 = vertex2 - vertex0;
        const auto h = glm::cross(rayDirection, edge2);
        float a = glm::dot(edge1, h);

        if (a > -epsilon && a < epsilon)
        {
            return;
        }

        float f = 1.0f / a;
        const auto s = rayOrigin - vertex0;
        float u = f * glm::dot(s, h);

        if (u < 0.0f || u > 1.0f)
        {
            return;
        }

        const auto q = glm::cross(s, edge1);
        const float v = f * glm::dot(rayDirection, q);

        if (v < 0.0f || u + v > 1.0f)
        {
            return;
        }

        float t = f * glm::dot(edge2, q);
        if(t > epsilon)
        {
            intersectionInfo.m_hitDistance = t;
            intersectionInfo.m_isIntersected = true;
        }
    }

    template<typename ScalarT = float>
    static void rayQuadIntersection(const glm::vec<3, ScalarT, glm::defaultp>& rayOrigin,
                                    const glm::vec<3, ScalarT, glm::defaultp>& rayDirection,
                                    const glm::vec<3, ScalarT, glm::defaultp>& vertex0,
                                    const glm::vec<3, ScalarT, glm::defaultp>& vertex1,
                                    const glm::vec<3, ScalarT, glm::defaultp>& vertex2,
                                    const glm::vec<3, ScalarT, glm::defaultp>& vertex3,
                                    RayIntersectionInfo<ScalarT>& intersectionInfo,
                                    const ScalarT& epsilon = 1e-8)
    {
        intersectionInfo.m_hitDistance = std::numeric_limits<float>::max();
        intersectionInfo.m_isIntersected = false;

        RayIntersectionInfo<> triangleIntersectionInfo0;
        RayIntersectionInfo<> triangleIntersectionInfo1;

        rayTriangleIntersection(rayOrigin, rayDirection, vertex0, vertex1, vertex2, triangleIntersectionInfo0);
        rayTriangleIntersection(rayOrigin, rayDirection, vertex0, vertex2, vertex3, triangleIntersectionInfo1);

        if(triangleIntersectionInfo0.m_hitDistance < triangleIntersectionInfo1.m_hitDistance)
        {
            intersectionInfo = triangleIntersectionInfo0;
            return;
        }

        if(triangleIntersectionInfo1.m_hitDistance < triangleIntersectionInfo0.m_hitDistance)
        {
            intersectionInfo = triangleIntersectionInfo1;
        }
    }
    
    template<typename ScalarT = float>
    static void linePlaneIntersection(const glm::vec<3, ScalarT, glm::defaultp>& rayOrigin,
                                      const glm::vec<3, ScalarT, glm::defaultp>& rayDirection,
                                      const glm::vec<3, ScalarT, glm::defaultp>& planeOrigin,
                                      const glm::vec<3, ScalarT, glm::defaultp>& planeNormal,
                                      const ScalarT& lineLength,
                                      RayIntersectionInfo<ScalarT>& intersectionInfo,
                                      const ScalarT& epsilon = 1e-8)
    {
        rayPlaneIntersection(rayOrigin, rayDirection, planeOrigin, planeNormal,
                             intersectionInfo, epsilon);
        
        intersectionInfo.m_isIntersected = intersectionInfo.m_isIntersected && intersectionInfo.m_hitDistance <= lineLength;
    }
    
    template<typename ScalarT = float>
    static void rayPlaneIntersection(const Ray3D<ScalarT>& ray,
                                     const Plane3D<ScalarT>& plane,
                                     RayIntersectionInfo<ScalarT>& intersectionInfo,
                                     const ScalarT& epsilon = 1e-8)
    {
        rayPlaneIntersection(ray.m_origin, ray.m_direction, plane.m_origin, plane.m_normal,
                             intersectionInfo, epsilon);
    }
    
    template<typename ScalarT = float>
    static void linePlaneIntersection(const Line3D<ScalarT>& line,
                                      const Plane3D<ScalarT>& plane,
                                      RayIntersectionInfo<ScalarT>& intersectionInfo,
                                      const ScalarT& epsilon = 1e-8)
    {
        linePlaneIntersection(line.m_origin, line.m_direction, plane.m_origin, plane.m_normal,
                              line.m_length, intersectionInfo, epsilon);
    }
}

#endif // SUNGEARENGINE_MATHPRIMITIVESUTILS_H
