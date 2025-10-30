//
// Created by stuka on 25.01.2024.
//

#include "RenderPipelinesManager.h"

void SGCore::RenderPipelinesManager::subscribeToRenderPipelineSetEvent(Slot<void()>& slot)
{
    onRenderPipelineSet += slot;
}

SGCore::Ref<SGCore::IRenderPipeline> SGCore::RenderPipelinesManager::getCurrentRenderPipeline() const noexcept
{
    return m_currentRenderPipeline;
}

SGCore::RenderPipelinesManager& SGCore::RenderPipelinesManager::instance() noexcept
{
    static RenderPipelinesManager instance;
    return instance;
}
