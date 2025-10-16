//
// Created by Ilya on 03.11.2023.
//
#include "IRenderPipeline.h"
#include "SGCore/Main/CoreMain.h"

void SGCore::IRenderPipeline::render(Scene* scene) noexcept
{
#ifdef SG_PLATFORM_PC
    auto t0 = glfwGetTime();
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
    
    for(auto& renderPass : m_renderPasses)
    {
        if(!renderPass->m_active) continue;

#ifdef SG_PLATFORM_PC
        auto t1 = glfwGetTime();
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif

        renderPass->render(scene, shared_from_this());

#ifdef SG_PLATFORM_PC
        auto t2 = glfwGetTime();
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif

#ifdef SG_PLATFORM_PC
        renderPass->m_executionTime = (t2 - t1) * 1000.0;
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
    }

#ifdef SG_PLATFORM_PC
    auto t1 = glfwGetTime();
    
    m_renderPassesExecutionTime = (t1 - t0) * 1000.0;
#elif defined(SG_PLATFORM_OS_ANDROID)
#endif
}

double SGCore::IRenderPipeline::getRenderPassesExecutionTime() const noexcept
{
    return m_renderPassesExecutionTime;
}
