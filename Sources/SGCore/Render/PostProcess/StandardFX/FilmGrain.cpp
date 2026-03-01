//
// Created by stuka on 28.02.2026.
//

#include "FilmGrain.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"

SGCore::FilmGrain::FilmGrain()
{
    m_name = "SG_FILM_GRAIN";

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
    auto defaultShader = assetManager->loadAsset<IShader>("${enginePath}/Resources/sg_shaders/features/postprocessing/layered/film_grain.sgshader");

    setShader(defaultShader);
}

void SGCore::FilmGrain::passValuesToSubPassShader() noexcept
{
    setIntensity(getIntensity());
}

void SGCore::FilmGrain::onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept
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

float SGCore::FilmGrain::getIntensity() const noexcept
{
    return m_intensity;
}

void SGCore::FilmGrain::setIntensity(float intensity) noexcept
{
    m_intensity = intensity;

    auto shader = getShader();
    if(!shader) return;

    shader->bind();
    shader->useFloat(m_name + "_intensity", m_intensity);
}
