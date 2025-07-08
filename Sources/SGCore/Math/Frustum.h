//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_FRUSTUM_H
#define SUNGEARENGINE_FRUSTUM_H

#include "SGCore/pch.h"

class Frustum
{
public:
    Frustum() = default;
    
    // m = ProjectionMatrix * ViewMatrix
    Frustum(glm::mat4 m);

    [[nodiscard]] bool testAABB(const glm::vec3& aabbMin, const glm::vec3& aabbMax) const;

    glm::vec3 getCenter() const noexcept;

private:
    enum Planes
    {
        Left = 0,
        Right,
        Bottom,
        Top,
        Near,
        Far,
        Count,
        Combinations = Count * (Count - 1) / 2
    };
    
    template<Planes i, Planes j>
    struct ij2k
    {
        enum { k = i * (9 - i) / 2 + j - 1 };
    };
    
    template<Planes a, Planes b, Planes c>
    glm::vec3 intersection(const glm::vec3* crosses) const;
    
    glm::vec4 m_planes[Count] { };
    glm::vec3 m_points[8] { };

    glm::vec3 m_center { };
};

#endif //SUNGEARENGINE_FRUSTUM_H
