//
// Created by stuka on 01.03.2026.
//

#include "SSR.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"

SGCore::SSR::SSR()
{
    m_name = "SG_SSR";

    auto assetManager = AssetManager::getInstance();

    // ======================== creating default passes
    PostProcessFXSubPass subPass;
    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2;
    m_subPasses.push_back(subPass);

    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3;
    m_subPasses.push_back(subPass);

    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7;
    m_subPasses.push_back(subPass);

    m_usedAttachments = {
        { m_name + "_raw", SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2 },
        { m_name + "_final", SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3 }
    };

    // ======================== set default shader
    auto defaultShader = assetManager->loadAsset<IShader>("${enginePath}/Resources/sg_shaders/features/postprocessing/layered/ssr.sgshader");

    setShader(defaultShader);

    SSR::passValuesToSubPassShader();
}

void SGCore::SSR::passValuesToSubPassShader() noexcept
{
    setBlurRadius(m_blurRadius);
    setMaxSteps(m_maxSteps);
    setInitialStepSize(m_initialStepSize);
    setMinStepSize(m_minStepSize);
    setIntensity(m_intensity);
}

void SGCore::SSR::onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept
{
    targetFrameBuffer->bind();

    if(!targetFrameBuffer->hasAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2))
    {
        targetFrameBuffer->addAttachment(
                SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2, // VIGNETTE ATTACHMENT
                SGGColorFormat::SGG_RGBA,
                SGGColorInternalFormat::SGG_RGBA16_FLOAT,
                SGGDataType::SGG_FLOAT,
                0,
                0
        );
    }

    if(!targetFrameBuffer->hasAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3))
    {
        targetFrameBuffer->addAttachment(
                SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3, // VIGNETTE ATTACHMENT
                SGGColorFormat::SGG_RGBA,
                SGGColorInternalFormat::SGG_RGBA16_FLOAT,
                SGGDataType::SGG_FLOAT,
                0,
                0
        );
    }

    targetFrameBuffer->unbind();
}

void SGCore::SSR::setBlurRadius(float blurRadius) noexcept
{
    m_blurRadius = blurRadius;

    auto shader = getShader();
    if(!shader) return;

    shader->bind();
    shader->useFloat(m_name + "_blur_radius", m_blurRadius);
}

float SGCore::SSR::getBlurRadius() const noexcept
{
    return m_blurRadius;
}

void SGCore::SSR::setMaxSteps(std::int32_t maxSteps) noexcept
{
    m_maxSteps = maxSteps;

    auto shader = getShader();
    if(!shader) return;

    shader->bind();
    shader->useInteger(m_name + "_max_steps", m_maxSteps);
}

std::int32_t SGCore::SSR::getMaxSteps() const noexcept
{
    return m_maxSteps;
}

void SGCore::SSR::setInitialStepSize(float initialStepSize) noexcept
{
    m_initialStepSize = initialStepSize;

    auto shader = getShader();
    if(!shader) return;

    shader->bind();
    shader->useFloat(m_name + "_initial_step_size", m_initialStepSize);
}

float SGCore::SSR::getInitialStepSize() const noexcept
{
    return m_initialStepSize;
}

void SGCore::SSR::setMinStepSize(float minStepSize) noexcept
{
    m_minStepSize = minStepSize;

    auto shader = getShader();
    if(!shader) return;

    shader->bind();
    shader->useFloat(m_name + "_min_step_size", m_minStepSize);
}

float SGCore::SSR::getMinStepSize() const noexcept
{
    return m_minStepSize;
}

void SGCore::SSR::setIntensity(float intensity) noexcept
{
    m_intensity = intensity;

    auto shader = getShader();
    if(!shader) return;

    shader->bind();
    shader->useFloat(m_name + "_intensity", m_intensity);
}

float SGCore::SSR::getIntensity() const noexcept
{
    return m_intensity;
}
