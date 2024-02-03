//
// Created by Ilya on 03.11.2023.
//
#include "IRenderPipeline.h"
#include "SGCore/Main/CoreMain.h"

void SGCore::IRenderPipeline::render(SGCore::Ref<SGCore::Scene> scene) noexcept
{
    for(auto& renderPass : m_renderPasses)
    {
        if(!renderPass->m_active) continue;

        renderPass->render(scene, shared_from_this());
    }
}
