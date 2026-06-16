//
// Created by stuka on 17.06.2026.
//

#pragma once

#include <array>
#include <glm/glm.hpp>

namespace SGCore::Primitives
{
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
}