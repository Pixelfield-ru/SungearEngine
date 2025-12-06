//
// Created by stuka on 06.12.2025.
//

#include "NavMesh.h"

#include "Steps/InputFilteringStep.h"
#include "Steps/VoxelizationStep.h"

void SGCore::Navigation::NavMesh::useStandardSteps() noexcept
{
    addStep(0, MakeRef<InputFilteringStep>());
    addStep(1, MakeRef<VoxelizationStep>());
}

void SGCore::Navigation::NavMesh::build(std::vector<MathPrimitivesUtils::Triangle<>> sceneTriangles) noexcept
{
    m_inputSceneTriangles = std::move(sceneTriangles);

    for(const auto& step : m_steps)
    {
        step->process(*this, m_config);
    }
}
