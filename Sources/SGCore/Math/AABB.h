//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_AABB_H
#define SUNGEARENGINE_AABB_H

#include "SGCore/pch.h"

#include "MathUtils.h"
#include "SGCore/Serde/Serde.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/ImportedScenesArch/Vertex.h"

namespace SGCore
{
    template<typename ScalarT = float>
    requires(std::is_signed_v<ScalarT>)
    struct AABB : ECS::Component<AABB<ScalarT>, const AABB<ScalarT>>
    {
        static_assert(std::is_scalar_v<ScalarT> && "ScalarT must be scalar.");
        
        using scalar_t = ScalarT;
        
        using vec2_t = glm::vec<3, ScalarT, glm::defaultp>;
        using vec3_t = glm::vec<3, ScalarT, glm::defaultp>;
        using vec4_t = glm::vec<4, ScalarT, glm::defaultp>;
        
        using mat4_t = glm::mat<4, 4, ScalarT, glm::defaultp>;
        using quat_t = glm::qua<ScalarT, glm::defaultp>;
        
        AABB(const vec3_t& min, const vec3_t& max) noexcept : m_min(min), m_max(max) { }
        
        AABB() = default;
        AABB(const AABB&) = default;
        AABB(AABB&) = default;

        // assigning 'min' to numeric_limits::max to find vertex with real min pos in vertices positions list. because real minimum can be positive
        vec3_t m_min = { std::numeric_limits<ScalarT>::max(), std::numeric_limits<ScalarT>::max(), std::numeric_limits<ScalarT>::max() };
        // assigning 'max' to numeric_limits::min to find vertex with real max pos in vertices positions list. because real maximum can be negative
        vec3_t m_max = { std::numeric_limits<ScalarT>::min(), std::numeric_limits<ScalarT>::min(), std::numeric_limits<ScalarT>::min() };
        
        [[nodiscard]] bool isCollidesWith(const AABB& other) const noexcept
        {
            vec3_t globalCenter = getGlobalCenter();
            vec3_t localCenter = getLocalCenter();
            
            vec3_t otherGlobalCenter = other.getGlobalCenter();
            vec3_t otherLocalCenter = other.getLocalCenter();
            
            if(std::abs(globalCenter.x - otherGlobalCenter.x) > (localCenter.x + otherLocalCenter.x)) return false;
            if(std::abs(globalCenter.y - otherGlobalCenter.y) > (localCenter.y + otherLocalCenter.y)) return false;
            if(std::abs(globalCenter.z - otherGlobalCenter.z) > (localCenter.z + otherLocalCenter.z)) return false;
            
            return true;
        }
        
        [[nodiscard]] bool isCollidesWith2D(const AABB& other) const noexcept
        {
            vec3_t globalCenter = getGlobalCenter();
            vec3_t localCenter = getLocalCenter();
            
            // 10, 10 - min
            // 15, 40 - max
            // 2.5, 15 - local center
            // 12.5, 25 - global center
            
            // 30, 30 - min
            // 10, 10 - max
            // -10, -10 - local center
            // 20, 20 - global center
            
            vec3_t otherGlobalCenter = other.getGlobalCenter();
            vec3_t otherLocalCenter = other.getLocalCenter();
            
            if(std::abs(globalCenter.x - otherGlobalCenter.x) > (localCenter.x + otherLocalCenter.x)) return false;
            if(std::abs(globalCenter.y - otherGlobalCenter.y) > (localCenter.y + otherLocalCenter.y)) return false;
            
            return true;
        }
        
        [[nodiscard]] bool isOverlappedBy(const AABB& other) const noexcept
        {
            if(m_max.x > other.m_max.x) return false;
            if(m_max.y > other.m_max.y) return false;
            if(m_max.z > other.m_max.z) return false;
            
            if(m_min.x < other.m_min.x) return false;
            if(m_min.y < other.m_min.y) return false;
            if(m_min.z < other.m_min.z) return false;
            
            return true;
        }
        
        [[nodiscard]] bool isOverlappedBy2D(const AABB& other) const noexcept
        {
            if(m_max.x > other.m_max.x) return false;
            if(m_max.y > other.m_max.y) return false;
            
            if(m_min.x < other.m_min.x) return false;
            if(m_min.y < other.m_min.y) return false;
            
            return true;
        }

        void setToInitial() noexcept
        {
            // assigning 'min' to numeric_limits::max to find vertex with real min pos in vertices positions list. because real minimum can be positive
            m_min = { std::numeric_limits<ScalarT>::max(),
                      std::numeric_limits<ScalarT>::max(),
                      std::numeric_limits<ScalarT>::max() };
            // assigning 'max' to numeric_limits::min to find vertex with real max pos in vertices positions list. because real maximum can be negative
            m_max = { std::numeric_limits<ScalarT>::min(),
                      std::numeric_limits<ScalarT>::min(),
                      std::numeric_limits<ScalarT>::min() };
        }

        template<typename VertexT>
        requires(requires(VertexT vertex) { vertex.m_position; })
        void calculate(const std::vector<VertexT>& vertices) noexcept
        {
            setToInitial();

            for(const auto& vertex : vertices)
            {
                if(vertex.m_position.x < m_min.x)
                {
                    m_min.x = vertex.m_position.x;
                }

                if(vertex.m_position.y < m_min.y)
                {
                    m_min.y = vertex.m_position.y;
                }

                if(vertex.m_position.z < m_min.z)
                {
                    m_min.z = vertex.m_position.z;
                }

                if(vertex.m_position.x > m_max.x)
                {
                    m_max.x = vertex.m_position.x;
                }

                if(vertex.m_position.y > m_max.y)
                {
                    m_max.y = vertex.m_position.y;
                }

                if(vertex.m_position.z > m_max.z)
                {
                    m_max.z = vertex.m_position.z;
                }
            }
        }
        
        void applyTransformations(const mat4_t& modelMatrix, const AABB& sourceAABB) noexcept
        {
            vec3_t scale;
            quat_t rotation;
            vec3_t translation;
            vec3_t skew;
            vec4_t perspective;
            
            glm::decompose(modelMatrix, scale, rotation, translation, skew, perspective);
            
            applyTransformations(translation, rotation, scale, sourceAABB);
        }
        
        void applyTransformations(const vec3_t& translation, const quat_t& rotation, const vec3_t& scale, const AABB& sourceAABB) noexcept
        {
            m_min = sourceAABB.m_min;
            m_max = sourceAABB.m_max;
            
            vec3_t& min = m_min;
            vec3_t& max = m_max;
            
            vec3_t points[8] = {
                    { min },
                    { min.x, min.y, max.z },
                    { min.x, max.y, max.z },
                    { min.x, max.y, min.z },
                    
                    { max.x, min.y, min.z },
                    { max.x, max.y, min.z },
                    { max },
                    { max.x, min.y, max.z }
            };
            
            vec3_t eulerRot = glm::eulerAngles(rotation);
            
            for(auto& point : points)
            {
                point *= scale;
                point = rotation * vec4_t(point, 1.0);
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
        
        void fixMinMax() noexcept
        {
            if(m_max.x < m_min.x)
            {
                std::swap(m_max.x, m_min.x);
            }
            
            if(m_max.y < m_min.y)
            {
                std::swap(m_max.y, m_min.y);
            }
            
            if(m_max.z < m_min.z)
            {
                std::swap(m_max.z, m_min.z);
            }
        }
        
        [[nodiscard]] vec3_t getLocalCenter() const noexcept
        {
            return (m_max - m_min) / 2.0f;
        }
        
        [[nodiscard]] vec3_t getGlobalCenter() const noexcept
        {
            return m_min + getLocalCenter();
        }
        
        AABB& operator=(const AABB&) noexcept = default;
        AABB& operator=(AABB&&) noexcept = default;
        
        [[nodiscard]] bool operator==(const AABB& aabb) const noexcept
        {
            return m_min == aabb.m_min && m_max == aabb.m_max;
        }
        
        [[nodiscard]] bool operator!=(const AABB& aabb) const noexcept
        {
            return !(*this == aabb);
        }
    };

    /*template<typename ScalarT>
    struct SerializerSpec<AABB<ScalarT>>
    {
        static void serialize(rapidjson::Document& toDocument, rapidjson::Value& parent,
                              const std::string& varName, const AABB<ScalarT>& value) noexcept
        {
            rapidjson::Value k(rapidjson::kStringType);
            rapidjson::Value v(rapidjson::kObjectType);

            k.SetString(varName.c_str(), varName.length(), toDocument.GetAllocator());

            Serializer::serialize(toDocument, v, "min", value.m_min);
            Serializer::serialize(toDocument, v, "max", value.m_max);

            parent.AddMember(k, v, toDocument.GetAllocator());
        }
    };*/
}

template<typename ScalarT>
struct std::hash<SGCore::AABB<ScalarT>>
{
    std::size_t operator()(const SGCore::AABB<ScalarT>& k) const
    {
        return SGCore::MathUtils::hashVector(k.m_min) ^ SGCore::MathUtils::hashVector(k.m_max);
    }
};

#endif //SUNGEARENGINE_AABB_H
