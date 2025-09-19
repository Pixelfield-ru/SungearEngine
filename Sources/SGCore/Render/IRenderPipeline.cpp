//
// Created by Ilya on 03.11.2023.
//
#include "IRenderPipeline.h"
#include "SGCore/Main/CoreMain.h"

void SGCore::IRenderPipeline::render(Scene* scene) noexcept
{
    auto t0 = glfwGetTime();
    
    for(auto& renderPass : m_renderPasses)
    {
        if(!renderPass->m_active) continue;

        auto t1 = glfwGetTime();

        renderPass->render(scene, shared_from_this());

        auto t2 = glfwGetTime();

        renderPass->m_executionTime = (t2 - t1) * 1000.0;
    }
    
    auto t1 = glfwGetTime();
    
    m_renderPassesExecutionTime = (t1 - t0) * 1000.0;
}

double SGCore::IRenderPipeline::getRenderPassesExecutionTime() const noexcept
{
    return m_renderPassesExecutionTime;
}
