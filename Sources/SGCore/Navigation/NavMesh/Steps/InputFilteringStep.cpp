//
// Created by stuka on 07.12.2025.
//

#include "InputFilteringStep.h"

#include <iostream>

#include "SGCore/Navigation/NavMesh/NavMesh.h"

void SGCore::Navigation::InputFilteringStep::process(NavMesh& navMesh, const NavMeshConfig& config) noexcept
{
    m_walkableTriangles.clear();

    const float maxSlopeCos = cos(glm::radians(config.m_agentMaxSlope));
    const glm::vec3 up = MathUtils::up3;

    for(const auto& tri : navMesh.m_inputSceneTriangles)
    {
        const float slopeCos = glm::abs(glm::dot(tri.m_normal, up));

        if(slopeCos >= maxSlopeCos)
        {
            m_walkableTriangles.push_back(tri);
        }
    }

    if (m_walkableTriangles.empty()) return;

    m_sceneAABB.m_min = m_walkableTriangles[0].m_vertices[0];
    m_sceneAABB.m_max = m_walkableTriangles[0].m_vertices[0];

    for(const auto& tri : m_walkableTriangles)
    {
        for(std::uint8_t i = 0; i < 3; ++i)
        {
            m_sceneAABB.m_min = glm::min(m_sceneAABB.m_min, tri.m_vertices[i]);
            m_sceneAABB.m_max = glm::max(m_sceneAABB.m_max, tri.m_vertices[i]);
        }
    }

    const float padding = config.m_agentRadius * 2.0f;
    m_sceneAABB.m_min -= glm::vec3(padding);
    m_sceneAABB.m_max += glm::vec3(padding);

    std::cout << "SGCore::Navigation::InputFilteringStep: filtered " << m_walkableTriangles.size()
              << " walkable triangles from " << navMesh.m_inputSceneTriangles.size() << std::endl;
}
