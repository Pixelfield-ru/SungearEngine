//
// Created by ilya on 15.02.24.
//

#include "MathUtils.h"

glm::vec2 SGCore::MathUtils::raySphereIntersection(const glm::vec3& ro, const glm::vec3& rd, const float& sr) noexcept
{
    float b = glm::dot(ro, rd);
    float c = glm::dot(ro, ro) - sr * sr;
    float h = b * b - c;
    if(h < 0.0) return {-1.0, -1.0};
    h = std::sqrt(h);
    return {-b - h, -b + h};
}
