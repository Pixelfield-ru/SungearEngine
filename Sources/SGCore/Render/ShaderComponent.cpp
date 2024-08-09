//
// Created by ilya on 03.03.24.
//

#include "ShaderComponent.h"
#include "RenderPipelinesManager.h"

SGCore::ShaderComponent::ShaderComponent()
{
    RenderPipelinesManager::subscribeToRenderPipelineSetEvent(m_onRenderPipelineSet);
}

