//
// Created by Ilya on 03.11.2023.
//
#include "IRenderPipeline.h"
#include "SGCore/Main/CoreMain.h"
#include "IRenderPass.h"

void SGCore::IRenderPipeline::update(const SGCore::Ref<SGCore::Scene>& scene) noexcept
{
    if(m_prepareFunc)
    {
        m_prepareFunc();
    }

    auto thisShared = shared_from_this();

    for(auto& renderPass : m_renderPasses)
    {
        if(!renderPass->m_active) continue;

        renderPass->render(scene, thisShared);
    }
}
