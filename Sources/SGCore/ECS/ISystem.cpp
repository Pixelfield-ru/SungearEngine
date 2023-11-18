//
// Created by stuka on 30.10.2023.
//

#include "ISystem.h"
#include "SGCore/ECS/Rendering/Pipelines/PBRForwardRenderPipeline.h"

double Core::ECS::ISystem::getUpdateFunctionExecutionTime() const noexcept
{
    return m_update_executionTime;
}

double Core::ECS::ISystem::getFixedUpdateFunctionExecutionTime() const noexcept
{
    return m_fixedUpdate_executionTime;
}
