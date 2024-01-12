//
// Created by stuka on 02.01.2024.
//
#include "IComplexGizmo.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/ECS/Rendering/Pipelines/IRenderPipeline.h"

SGCore::IComplexGizmo::IComplexGizmo(const Ref<IRenderPipeline>& pipeline) noexcept
{
    loadShader(pipeline, "Gizmos/ComplexGizmosShader");
}