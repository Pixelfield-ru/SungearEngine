//
// Created by stuka on 04.12.2025.
//

#pragma once

#include <vector>
#include <glm/fwd.hpp>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Math/AABB.h"

namespace SGCore
{
    struct Vertex;
    class ITexture2D;
}

namespace SGCore::Navigation
{
    struct NavGrid3D : ECS::Component<NavGrid3D, const NavGrid3D>
    {
        struct Node
        {
            glm::vec3 m_position{};
            float m_size{};
        };

        float m_cellSize = 1.0f;
        std::vector<Node> m_nodes;

        void build(const std::vector<Vertex>& meshVertices,
                   const std::vector<std::uint32_t>& meshIndices,
                   std::uint8_t verticesPerPrimitiveCount,
                   const AABB<>& meshAABB,
                   const ECS::registry_t& sceneRegistry) noexcept;

        void build(const ITexture2D* displacementMap,
                   float displacementHeightScale,
                   const AABB<>& meshAABB,
                   const ECS::registry_t& sceneRegistry) noexcept;

        void applyModelMatrix(const glm::mat4& modelMatrix) noexcept;

    private:
        std::vector<Node> m_transformedNodes;

        static std::optional<float> getHeightAt(const std::vector<Vertex>& meshVertices,
                                                const std::vector<std::uint32_t>& meshIndices,
                                                const glm::vec2& position,
                                                std::uint8_t verticesPerPrimitiveCount) noexcept;
    };
}
