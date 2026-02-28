//
// Created by stuka on 28.02.2026.
//

#include "Vignette.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"

SGCore::Vignette::Vignette()
{
    m_name = "SG_VIGNETTE";

    auto assetManager = AssetManager::getInstance();

    // ======================== creating default passes
    PostProcessFXSubPass subPass;
    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2;
    m_subPasses.push_back(subPass);

    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7;
    m_subPasses.push_back(subPass);

    m_usedAttachments = {
        { m_name + "_final", SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2 }
    };

    // ======================== set default shader
    auto defaultShader = assetManager->loadAsset<IShader>("${enginePath}/Resources/sg_shaders/features/postprocessing/layered/vignette.sgshader");

    setShader(defaultShader);
}

void SGCore::Vignette::passValuesToSubPassShader() noexcept
{
    setSmoothness(getSmoothness());
    setRadius(getRadius());
}

void SGCore::Vignette::onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept
{
    if(targetFrameBuffer->hasAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2)) return;

    targetFrameBuffer->bind();

    targetFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2, // VIGNETTE ATTACHMENT
            SGGColorFormat::SGG_RGBA,
            SGGColorInternalFormat::SGG_RGBA16_FLOAT,
            SGGDataType::SGG_FLOAT,
            0,
            0
    );

    targetFrameBuffer->unbind();
}

void SGCore::Vignette::onRemoveAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept
{
    targetFrameBuffer->bind();

    targetFrameBuffer->removeAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2);

    targetFrameBuffer->unbind();
}

float SGCore::Vignette::getRadius() const noexcept
{
    return m_radius;
}

void SGCore::Vignette::setRadius(float radius) noexcept
{
    m_radius = radius;

    auto shader = getShader();
    if(!shader) return;

    shader->bind();
    shader->useFloat(m_name + "_radius", m_radius);
}

float SGCore::Vignette::getSmoothness() const noexcept
{
    return m_smoothness;
}

void SGCore::Vignette::setSmoothness(float smoothness) noexcept
{
    m_smoothness = smoothness;

    auto shader = getShader();
    if(!shader) return;

    shader->bind();
    shader->useFloat(m_name + "_smoothness", m_smoothness);
}
