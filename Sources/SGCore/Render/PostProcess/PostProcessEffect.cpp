//
// Created by ilya on 19.04.24.
//

#include "PostProcessEffect.h"

#include "SGCore/Render/LayeredFrameReceiver.h"

void SGCore::PostProcessEffect::onAttachToLayer(const SGCore::Ref<SGCore::PostProcessLayer>& toLayer)
{
    passValuesToSubPassShader(toLayer->getFXSubPassShader());
}

void SGCore::PostProcessEffect::onLayerShaderChanged(const SGCore::Ref<SGCore::PostProcessLayer>& layer)
{
    passValuesToSubPassShader(layer->getFXSubPassShader());
}

bool SGCore::PostProcessEffect::isEnabled() const noexcept
{
    return m_isEnabled;
}

void SGCore::PostProcessEffect::setEnabled(bool isEnabled) noexcept
{
    m_isEnabled = isEnabled;
    
    for(const auto& layer : m_parentPostProcessLayers)
    {
        if(auto lockedLayer = layer.lock())
        {
            lockedLayer->getFXSubPassShader()->bind();
            
            lockedLayer->getFXSubPassShader()->useInteger(m_name + "_ENABLED", m_isEnabled);
        }
    }
}