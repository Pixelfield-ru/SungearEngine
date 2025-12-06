//
// Created by stuka on 07.12.2025.
//

#include "VoxelizationStep.h"

#include "InputFilteringStep.h"
#include "SGCore/Navigation/NavMesh/NavMesh.h"

void SGCore::Navigation::VoxelizationStep::process(NavMesh& navMesh, const NavMeshConfig& config) noexcept
{
    const auto inputFilteringStep = navMesh.getStep<InputFilteringStep>();

    for(const auto& tri : inputFilteringStep->m_walkableTriangles)
    {
        const auto minXZ = tri.minXZ();
        const auto maxXZ = tri.maxXZ();
    }
}
