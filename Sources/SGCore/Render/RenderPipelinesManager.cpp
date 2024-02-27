//
// Created by stuka on 25.01.2024.
//

#include "RenderPipelinesManager.h"

SGCore::Ref<SGCore::IRenderPipeline> SGCore::RenderPipelinesManager::getCurrentRenderPipeline() noexcept
{
    return m_currentRenderPipeline;
}
