//
// Created by stuka on 07.12.2025.
//

#pragma once

#include <vector>
#include <limits>
#include <unordered_map>
#include <glm/vec3.hpp>

#include "INavMeshBuildStep.h"
#include "SGCore/Math/MathPrimitivesUtils.h"

namespace SGCore::Navigation
{
    struct NavVoxel
    {
        bool m_isWalkable {};
        float m_distanceToEdge = std::numeric_limits<float>::max();
        std::int32_t m_regionID {};

        glm::ivec3 m_position {};

        // float m_heightOffsetToGeometry {};
    };

    struct VoxelizationStep : INavMeshBuildStep
    {
        std::vector<NavVoxel> m_voxels;
        std::unordered_map<glm::ivec3, size_t, MathUtils::GLMVectorHash<glm::ivec3>> m_voxelsMap;
        std::int32_t m_voxelGridWidth {};
        std::int32_t m_voxelGridHeight {};
        std::int32_t m_voxelGridDepth {};
        glm::vec3 m_voxelGridOrigin {};

        void process(NavMesh& navMesh, const NavMeshConfig& config) noexcept override;

        std::int32_t worldToVoxelX(float wx, float cellSize) const noexcept;
        std::int32_t worldToVoxelY(float wy, float cellHeight) const noexcept;
        std::int32_t worldToVoxelZ(float wz, float cellSize) const noexcept;

        glm::vec3 voxelToWorld(const glm::i32vec3& voxelPos, float cellSize, float cellHeight) const noexcept;

        float voxelToWorldX(std::int32_t vx, float cellSize) const noexcept;
        float voxelToWorldY(std::int32_t vy, float cellHeight) const noexcept;
        float voxelToWorldZ(std::int32_t vz, float cellSize) const noexcept;

        NavVoxel& getVoxel(std::int32_t x, std::int32_t y, std::int32_t z) noexcept;

        bool isVoxelInTriangle(std::int32_t vx, std::int32_t vy, std::int32_t vz, const MathPrimitivesUtils::Triangle<>& tri, const NavMeshConfig& config) const;
    };
}
