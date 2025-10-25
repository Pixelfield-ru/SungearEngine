//
// Created by ilya on 10.03.24.
//

#include "Frustum.h"

#include <glm/fwd.hpp>
#include <glm/matrix.hpp>

Frustum::Frustum(glm::mat4 m)
{
    m = glm::transpose(m);
    m_planes[Left]   = m[3] + m[0];
    m_planes[Right]  = m[3] - m[0];
    m_planes[Bottom] = m[3] + m[1];
    m_planes[Top]    = m[3] - m[1];
    m_planes[Near]   = m[3] + m[2];
    m_planes[Far]    = m[3] - m[2];
    
    glm::vec3 crosses[Combinations] = {
            glm::cross(glm::vec3(m_planes[Left]),   glm::vec3(m_planes[Right])),
            glm::cross(glm::vec3(m_planes[Left]),   glm::vec3(m_planes[Bottom])),
            glm::cross(glm::vec3(m_planes[Left]),   glm::vec3(m_planes[Top])),
            glm::cross(glm::vec3(m_planes[Left]),   glm::vec3(m_planes[Near])),
            glm::cross(glm::vec3(m_planes[Left]),   glm::vec3(m_planes[Far])),
            glm::cross(glm::vec3(m_planes[Right]),  glm::vec3(m_planes[Bottom])),
            glm::cross(glm::vec3(m_planes[Right]),  glm::vec3(m_planes[Top])),
            glm::cross(glm::vec3(m_planes[Right]),  glm::vec3(m_planes[Near])),
            glm::cross(glm::vec3(m_planes[Right]),  glm::vec3(m_planes[Far])),
            glm::cross(glm::vec3(m_planes[Bottom]), glm::vec3(m_planes[Top])),
            glm::cross(glm::vec3(m_planes[Bottom]), glm::vec3(m_planes[Near])),
            glm::cross(glm::vec3(m_planes[Bottom]), glm::vec3(m_planes[Far])),
            glm::cross(glm::vec3(m_planes[Top]),    glm::vec3(m_planes[Near])),
            glm::cross(glm::vec3(m_planes[Top]),    glm::vec3(m_planes[Far])),
            glm::cross(glm::vec3(m_planes[Near]),   glm::vec3(m_planes[Far]))
    };
    
    m_points[0] = intersection<Left,  Bottom, Near>(crosses);
    m_points[1] = intersection<Left,  Top,    Near>(crosses);
    m_points[2] = intersection<Right, Bottom, Near>(crosses);
    m_points[3] = intersection<Right, Top,    Near>(crosses);
    m_points[4] = intersection<Left,  Bottom, Far>(crosses);
    m_points[5] = intersection<Left,  Top,    Far>(crosses);
    m_points[6] = intersection<Right, Bottom, Far>(crosses);
    m_points[7] = intersection<Right, Top,    Far>(crosses);

    for(const auto& v : m_points)
    {
        m_center += v;
    }

    m_center /= 8.0f;
}

bool Frustum::testAABB(const glm::vec3& aabbMin, const glm::vec3& aabbMax) const
{
    // check box outside/inside of frustum
    for(int i = 0; i < Count; i++)
    {
        if ((glm::dot(m_planes[i], glm::vec4(aabbMin.x, aabbMin.y, aabbMin.z, 1.0f)) < 0.0) &&
            (glm::dot(m_planes[i], glm::vec4(aabbMax.x, aabbMin.y, aabbMin.z, 1.0f)) < 0.0) &&
            (glm::dot(m_planes[i], glm::vec4(aabbMin.x, aabbMax.y, aabbMin.z, 1.0f)) < 0.0) &&
            (glm::dot(m_planes[i], glm::vec4(aabbMax.x, aabbMax.y, aabbMin.z, 1.0f)) < 0.0) &&
            (glm::dot(m_planes[i], glm::vec4(aabbMin.x, aabbMin.y, aabbMax.z, 1.0f)) < 0.0) &&
            (glm::dot(m_planes[i], glm::vec4(aabbMax.x, aabbMin.y, aabbMax.z, 1.0f)) < 0.0) &&
            (glm::dot(m_planes[i], glm::vec4(aabbMin.x, aabbMax.y, aabbMax.z, 1.0f)) < 0.0) &&
            (glm::dot(m_planes[i], glm::vec4(aabbMax.x, aabbMax.y, aabbMax.z, 1.0f)) < 0.0))
        {
            return false;
        }
    }
    
    // check frustum outside/inside box
    int out;
    out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].x > aabbMax.x) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].x < aabbMin.x) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].y > aabbMax.y) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].y < aabbMin.y) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].z > aabbMax.z) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].z < aabbMin.z) ? 1 : 0); if (out == 8) return false;
    
    return true;
}

glm::vec3 Frustum::getCenter() const noexcept
{
    return m_center;
}

template<Frustum::Planes a, Frustum::Planes b, Frustum::Planes c>
inline glm::vec3 Frustum::intersection(const glm::vec3* crosses) const
{
    float D = glm::dot(glm::vec3(m_planes[a]), crosses[ij2k<b, c>::k]);
    glm::vec3 res = glm::mat3(crosses[ij2k<b, c>::k], -crosses[ij2k<a, c>::k], crosses[ij2k<a, b>::k]) *
                    glm::vec3(m_planes[a].w, m_planes[b].w, m_planes[c].w);
    return res * (-1.0f / D);
}
