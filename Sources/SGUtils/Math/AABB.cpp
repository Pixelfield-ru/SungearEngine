//
// Created by ilya on 10.03.24.
//
#include "AABB.h"

#include <glm/gtx/matrix_decompose.hpp>

SGCore::AABB::AABB(const glm::vec3& min, const glm::vec3& max) noexcept : m_min(min), m_max(max)
{
}

void SGCore::AABB::calculateAABBFromModelMatrix(const glm::mat4& modelMatrix, const AABB& sourceAABB) noexcept
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    
    glm::decompose(modelMatrix, scale, rotation, translation, skew,
                   perspective);
    
    calculateAABBFromTRS(translation, rotation, scale, sourceAABB);
}

void SGCore::AABB::calculateAABBFromTRS(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale,
                                        const SGCore::AABB& sourceAABB) noexcept
{
    m_min = sourceAABB.m_min;
    m_max = sourceAABB.m_max;
    
    glm::vec3& min = m_min;
    glm::vec3& max = m_max;
    
    glm::vec3 points[8] = {
            { min },
            { min.x, min.y, max.z },
            { min.x, max.y, max.z },
            { min.x, max.y, min.z },
            
            { max.x, min.y, min.z },
            { max.x, max.y, min.z },
            { max },
            { max.x, min.y, max.z }
    };
    
    glm::vec3 eulerRot = glm::eulerAngles(rotation);
    
    for(auto& point : points)
    {
        point *= scale;
        point = rotation * glm::vec4(point, 1.0);
        point += translation;
    }
    
    min = points[0];
    max = points[0];
    
    for(const auto& point : points)
    {
        if(point.x < min.x)
        {
            min.x = point.x;
        }
        if(point.y < min.y)
        {
            min.y = point.y;
        }
        if(point.z < min.z)
        {
            min.z = point.z;
        }
    }
    
    for(const auto& point : points)
    {
        if(point.x > max.x)
        {
            max.x = point.x;
        }
        if(point.y > max.y)
        {
            max.y = point.y;
        }
        if(point.z > max.z)
        {
            max.z = point.z;
        }
    }
}

glm::vec3 SGCore::AABB::getLocalCenter() const noexcept
{
    return (m_max - m_min) / 2.0f;
}

glm::vec3 SGCore::AABB::getGlobalCenter() const noexcept
{
    return m_min + getLocalCenter();
}

bool SGCore::AABB::isCollidesWith(const SGCore::AABB& other) const noexcept
{
    glm::vec3 globalCenter = getGlobalCenter();
    glm::vec3 localCenter = getLocalCenter();
    
    glm::vec3 otherGlobalCenter = other.getGlobalCenter();
    glm::vec3 otherLocalCenter = other.getLocalCenter();
    
    if(std::abs(globalCenter.x - otherGlobalCenter.x) > (localCenter.x + otherLocalCenter.x)) return false;
    if(std::abs(globalCenter.y - otherGlobalCenter.y) > (localCenter.y + otherLocalCenter.y)) return false;
    if(std::abs(globalCenter.z - otherGlobalCenter.z) > (localCenter.z + otherLocalCenter.z)) return false;
    
    return true;
    
    /*glm::vec3 globalCenter = getGlobalCenter();
    glm::vec3 localCenter = getLocalCenter();
    
    glm::vec3 otherGlobalCenter = other.getGlobalCenter();
    glm::vec3 otherLocalCenter = other.getLocalCenter();*/
    
    // float globalDistance = glm::length(globalCenter - otherGlobalCenter) - glm::length(localCenter + otherLocalCenter);
    
    // return glm::length(globalCenter - otherGlobalCenter) <= glm::length(localCenter + otherLocalCenter);
    
    // We have an overlap
    // return globalDistance <= 0;
}

