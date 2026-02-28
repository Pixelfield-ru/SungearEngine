//
// Created by stuka on 26.11.2024.
//

#include "PostProcessEffect.h"

#include "SGCore/Render/LayeredFrameReceiver.h"

void SGCore::PostProcessEffect::onAttachToLayer(const SGCore::Ref<SGCore::PostProcessLayer>& toLayer)
{
    passValuesToSubPassShader();
    setEnabled(isEnabled());
}

SGCore::AssetRef<SGCore::IShader> SGCore::PostProcessEffect::getShader() const noexcept
{
    return m_effectShader;
}

void SGCore::PostProcessEffect::setShader(const AssetRef<IShader>& shader) noexcept
{
    m_effectShader = shader;

    onLayerShaderChangedSlot.disconnectFromAll();
    onLayerShaderChangedSlot = [this, effectShader = m_effectShader](IAsset* shader) {
        passValuesToSubPassShader();
        setEnabled(isEnabled());
    };

    m_effectShader->onLazyLoadDone += onLayerShaderChangedSlot;

    passValuesToSubPassShader();
    setEnabled(isEnabled());
}

bool SGCore::PostProcessEffect::isEnabled() const noexcept
{
    return m_isEnabled;
}

void SGCore::PostProcessEffect::setEnabled(bool isEnabled) noexcept
{
    m_isEnabled = isEnabled;

    if(!m_effectShader) return;

    m_effectShader->bind();

    m_effectShader->useInteger(m_name + "_ENABLED", m_isEnabled);
}