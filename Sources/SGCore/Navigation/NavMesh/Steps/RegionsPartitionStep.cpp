//
// Created by stuka on 08.12.2025.
//

#include "RegionsPartitionStep.h"

#include <iostream>
#include <stack>

#include "VoxelizationStep.h"
#include "SGCore/Navigation/NavMesh/NavMesh.h"

void SGCore::Navigation::RegionsPartitionStep::process(NavMesh& navMesh, const NavMeshConfig& config) noexcept
{
    m_regions.clear();

    const auto voxelizationStep = navMesh.getStep<VoxelizationStep>();

    // unordered set that contains indices of visited voxels
    std::unordered_set<size_t> visitedVoxels;

    // flood fill
    for(size_t i = 0; i < voxelizationStep->m_voxels.size(); ++i)
    {
        const auto& currentVoxel = voxelizationStep->m_voxels[i];

        if(visitedVoxels.contains(i) || !currentVoxel.m_isWalkable) continue;

        Region region;

        floodFill(region, voxelizationStep, currentVoxel, i, visitedVoxels, config);

        m_regions.push_back(region);
    }

    std::cout << std::format("SGCore::Navigation::RegionsPartitionStep: detected {} regions", m_regions.size()) << std::endl;
}

void SGCore::Navigation::RegionsPartitionStep::floodFill(Region& region,
                                                         const Ref<VoxelizationStep>& voxelizationStep,
                                                         const NavVoxel& voxel,
                                                         size_t voxelIndex,
                                                         std::unordered_set<size_t>& visitedVoxels,
                                                         const NavMeshConfig& config) noexcept
{
    std::stack<glm::i32vec3> neighborVoxels;
    neighborVoxels.push(voxel.m_position);

    const auto& voxelsMap = voxelizationStep->m_voxelsMap;
    const auto& voxels = voxelizationStep->m_voxels;

    auto isVoxelNeighbor = [&neighborVoxels, &visitedVoxels, &voxelsMap, &voxels](
        const decltype(voxelizationStep->m_voxelsMap)::const_iterator& neighborIndexIt) noexcept -> bool {

        return neighborIndexIt != voxelsMap.end() && voxels[neighborIndexIt->second].m_isWalkable;
    };

    auto checkNeighborVoxelAndAdd = [&neighborVoxels, &visitedVoxels, &isVoxelNeighbor](
        const decltype(voxelizationStep->m_voxelsMap)::const_iterator& neighborIndexIt,
        const glm::i32vec3& neighborVoxelPos,
        bool &hasVoxel) noexcept {

        if(isVoxelNeighbor(neighborIndexIt))
        {
            hasVoxel = true;

            if(!visitedVoxels.contains(neighborIndexIt->second))
            {
                neighborVoxels.push(neighborVoxelPos);
            }
        }
    };

    struct Plane
    {
        std::array<glm::i32vec3, 4> m_voxelsPos;
    };

    static const std::vector<Plane> planes {
        {
            .m_voxelsPos = { glm::i32vec3 { -1, 0, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 0, -1 } }
        },
        {
            .m_voxelsPos = { glm::i32vec3 { -1, 0, 0 }, { 0, 1, 0 }, { 1, 0, 0 }, { 0, -1, 0 } }
        },
        {
            .m_voxelsPos = { glm::i32vec3 { 0, 0, 1 }, { 0, 1, 0 }, { 0, 0, -1 }, { 0, -1, 0 } }
        },
        {
            .m_voxelsPos = { glm::i32vec3 { 0, -1, -1 }, { -1, 0, 0 }, { 1, 0, 0 }, { 0, 0, 1 } } // slope variant 1
        },
        {
            .m_voxelsPos = { glm::i32vec3 { 1, -1, 0 }, { 0, 0, -1 }, { -1, 0, 0 }, { 0, 0, 1 } } // slope variant 2
        },
        {
            .m_voxelsPos = { glm::i32vec3 { 0, -1, 1 }, { -1, 0, 0 }, { 0, 0, -1 }, { 1, 0, 0 } } // slope variant 3
        },
        {
            .m_voxelsPos = { glm::i32vec3 { -1, -1, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 0, -1 } } // slope variant 4
        }
        // ============ diagonals
        /*{
            .m_voxelsPos = { glm::i32vec3 { -1, -1, -1 }, { -1, 0, 1 }, { 1, 1, 1 }, { 1, 0, -1 } }
        },
        {
            .m_voxelsPos = { glm::i32vec3 { 1, -1, -1 }, { -1, 0, -1 }, { -1, 1, 1 }, { 1, 0, 1 } }
        },
        {
            .m_voxelsPos = { glm::i32vec3 { 1, -1, 1 }, { -1, 0, 1 }, { -1, 1, -1 }, { 1, 0, -1 } }
        },
        {
            .m_voxelsPos = { glm::i32vec3 { -1, -1, 1 }, { -1, 0, -1 }, { 1, 1, -1 }, { 1, 0, 1 } }
        },
        // =========== diagonals
        {
            .m_voxelsPos = { glm::i32vec3 { -1, -1, 0 }, { 0, 0, 1 }, { 1, 1, 0 }, { 0, 0, -1 } }
        },
        {
            .m_voxelsPos = { glm::i32vec3 { 0, -1, -1 }, { -1, 0, 0 }, { 0, 1, 1 }, { 1, 0, 0 } }
        },
        {
            .m_voxelsPos = { glm::i32vec3 { 1, -1, 0 }, { 0, 0, -1 }, { -1, 1, 0 }, { 0, 0, 1 } }
        },
        {
            .m_voxelsPos = { glm::i32vec3 { 0, -1, 1 }, { -1, 0, 0 }, { 0, 1, -1 }, { 1, 0, 0 } }
        }*/
    };

    while(!neighborVoxels.empty())
    {
        const auto& currentVoxelPos = neighborVoxels.top();
        neighborVoxels.pop();

        // guaranteed, that this voxel exists in map
        const auto currentVoxelIndexIt = voxelsMap.find(currentVoxelPos);
        const auto currentVoxelIndex = currentVoxelIndexIt->second;

        bool hasAnyPlaneWithAllVoxels = false;

        for(const auto& plane : planes)
        {
            bool hasAllVoxelsInPlane = true;

            for(const auto& voxelPos : plane.m_voxelsPos)
            {
                const auto pos = currentVoxelPos + voxelPos;

                bool hasVoxel = false;

                const auto indexIt = voxelsMap.find(pos);

                checkNeighborVoxelAndAdd(indexIt, pos, hasVoxel);

                hasAllVoxelsInPlane = hasAllVoxelsInPlane && hasVoxel;
            }

            hasAnyPlaneWithAllVoxels = hasAnyPlaneWithAllVoxels || hasAllVoxelsInPlane;
        }

        if(!hasAnyPlaneWithAllVoxels)
        {
            region.m_contourVoxelsIndices.push_back(currentVoxelIndex);
        }

        /*const auto left = currentVoxelPos + glm::i32vec3 { -1, 0, 0 };
        const auto right = currentVoxelPos + glm::i32vec3 { 1, 0, 0 };
        const auto down = currentVoxelPos + glm::i32vec3 { 0, -1, 0 };
        const auto top = currentVoxelPos + glm::i32vec3 { 0, 1, 0 };
        const auto back = currentVoxelPos + glm::i32vec3 { 0, 0, -1 };
        const auto front = currentVoxelPos + glm::i32vec3 { 0, 0, 1 };

        bool hasLeftVoxel = false;
        bool hasRightVoxel = false;
        bool hasDownVoxel = false;
        bool hasTopVoxel = false;
        bool hasBackVoxel = false;
        bool hasFrontVoxel = false;

        const auto leftIndex = voxelsMap.find(left);
        const auto rightIndex = voxelsMap.find(right);
        const auto downIndex = voxelsMap.find(down);
        const auto topIndex = voxelsMap.find(top);
        const auto backIndex = voxelsMap.find(back);
        const auto frontIndex = voxelsMap.find(front);

        // checking neighbors

        checkNeighborVoxelAndAdd(leftIndex, left, hasLeftVoxel);
        checkNeighborVoxelAndAdd(rightIndex, right, hasRightVoxel);
        checkNeighborVoxelAndAdd(downIndex, down, hasDownVoxel);
        checkNeighborVoxelAndAdd(topIndex, top, hasTopVoxel);
        checkNeighborVoxelAndAdd(backIndex, back, hasBackVoxel);
        checkNeighborVoxelAndAdd(frontIndex, front, hasFrontVoxel);

        // checking is voxel is contour
        // if voxel does not have all neighbors in any plane (X, Y, Z) then this voxel is contour
        if((!hasLeftVoxel || !hasFrontVoxel || !hasRightVoxel || !hasBackVoxel) &&      // Y plane
            (!hasTopVoxel || !hasBackVoxel || !hasDownVoxel || !hasFrontVoxel) &&       // Z plane
            (!hasLeftVoxel || !hasTopVoxel || !hasRightVoxel || !hasDownVoxel))         // X plane
        {
            region.m_contourVoxelsIndices.push_back(currentVoxelIndex);
        }*/

        region.m_voxelsIndices.push_back(currentVoxelIndex);

        visitedVoxels.insert(currentVoxelIndex);
    }
}
