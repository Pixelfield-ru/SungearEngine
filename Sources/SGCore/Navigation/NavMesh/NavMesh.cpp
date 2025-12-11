//
// Created by stuka on 06.12.2025.
//

#include "NavMesh.h"

#include "Steps/FilterErosionStep.h"
#include "Steps/HeightfieldBuildStep.h"
#include "Steps/InputFilteringStep.h"
#include "Steps/VoxelizationStep.h"

void SGCore::Navigation::NavMesh::useStandardSteps() noexcept
{
    // required step
    addStep(0, MakeRef<InputFilteringStep>());
    // todo: i think it is required step
    addStep(1, MakeRef<VoxelizationStep>());
    // todo: optional step if mesh is simple (not 3d)
    // addStep(2, MakeRef<HeightfieldBuildStep>());
    // todo: optional: filter erosion (step 3)
    addStep(2, MakeRef<FilterErosionStep>());
    // todo: optional: region partitioner (step 4)
    // todo: required: contour builder (step 5)
    // todo: optional: contour simplifier (step 6)
    // todo: required: triangulator (step 7)
    // todo: required: adjacency graph builder (for A*) (step 8)
}

void SGCore::Navigation::NavMesh::build(std::vector<MathPrimitivesUtils::Triangle<>> sceneTriangles) noexcept
{
    m_inputSceneTriangles = std::move(sceneTriangles);

    for(const auto& step : m_steps)
    {
        step->process(*this, m_config);
    }
}
