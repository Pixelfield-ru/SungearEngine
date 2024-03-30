//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_AABB_H
#define SUNGEARENGINE_AABB_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

#include "MathUtils.h"

namespace SGCore
{
    struct AABB
    {
        AABB(const glm::vec3& min, const glm::vec3& max) noexcept;
        
        AABB() = default;
        AABB(const AABB&) = default;
        AABB(AABB&) = default;
        
        glm::vec3 m_min = { 0, 0, 0 };
        glm::vec3 m_max = { 0, 0, 0 };
        
        [[nodiscard]] bool isCollidesWith(const AABB& other) const noexcept;
        [[nodiscard]] bool isOverlappedBy(const AABB& other) const noexcept;
        
        void calculateAABBFromModelMatrix(const glm::mat4& modelMatrix, const AABB& sourceAABB) noexcept;
        void calculateAABBFromTRS(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale, const AABB& sourceAABB) noexcept;
        
        [[nodiscard]] glm::vec3 getLocalCenter() const noexcept;
        [[nodiscard]] glm::vec3 getGlobalCenter() const noexcept;
        
        AABB& operator=(const AABB&) noexcept = default;
        AABB& operator=(AABB&&) noexcept = default;
        
        [[nodiscard]] bool operator==(const AABB& abbb) const noexcept;
        [[nodiscard]] bool operator!=(const AABB& abbb) const noexcept;
    };
}

template<>
struct std::hash<SGCore::AABB>
{
    std::size_t operator()(const SGCore::AABB& k) const
    {
        return SGCore::MathUtils::hashVector(k.m_min) ^ SGCore::MathUtils::hashVector(k.m_max);
    }
};


#endif //SUNGEARENGINE_AABB_H
