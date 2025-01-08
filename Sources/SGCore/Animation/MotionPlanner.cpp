//
// Created by stuka on 07.01.2025.
//

#include "MotionPlanner.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IRenderer.h"

SGCore::MotionPlanner::MotionPlanner() noexcept
{
    m_bonesTransformationsUniformBuffer = Ref<IUniformBuffer>(CoreMain::getRenderer()->createUniformBuffer());
    m_bonesTransformationsUniformBuffer->m_blockName = "BonesData";

    std::vector<IShaderUniform> uniforms;

    for(std::int32_t i = 0; i < m_maxBonesPerMesh; ++i)
    {
        uniforms.push_back(IShaderUniform("u_bonesTransformations[" + std::to_string(i) + "]", SGGDataType::SGG_MAT4));
    }

    uniforms.push_back(IShaderUniform("u_bonesCount", SGGDataType::SGG_INT));

    m_bonesTransformationsUniformBuffer->putUniforms(std::move(uniforms));

    m_bonesTransformationsUniformBuffer->setLayoutLocation(5);
    m_bonesTransformationsUniformBuffer->prepare();
}

SGCore::MotionPlanner SGCore::MotionPlanner::copyStructure() const noexcept
{
    MotionPlanner newMotionPlanner;
    newMotionPlanner.m_skeleton = m_skeleton;
    newMotionPlanner.m_maxBonesPerMesh = m_maxBonesPerMesh;
    if(m_rootNode)
    {
        newMotionPlanner.m_rootNode = MakeRef<MotionPlannerNode>(m_rootNode->copyStructure());
    }

    return newMotionPlanner;
}

