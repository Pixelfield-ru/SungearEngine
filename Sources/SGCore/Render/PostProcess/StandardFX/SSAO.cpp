//
// Created by stuka on 26.11.2024.
//

#include "SSAO.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/API/ITexture2D.h"

#include <random>

#include "SGCore/Graphics/API/IFrameBuffer.h"

SGCore::SSAO::SSAO()
{
    m_name = "SG_SSAO";

    auto assetManager = AssetManager::getInstance();

    m_noise = AssetManager::getInstance()->createAndAddAsset<ITexture2D>();

    generateKernel();

    // ======================== creating default passes
    PostProcessFXSubPass subPass;

    // calculate SSAO occlusion
    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2;
    m_subPasses.push_back(subPass);

    // calculate final SSAO result
    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3;
    m_subPasses.push_back(subPass);

    // write final SSAO result in final FX attachment
    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7;
    m_subPasses.push_back(subPass);

    m_usedAttachments = {
        { m_name + "_occlusion", SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2 },
        { m_name + "_final", SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3 },
    };

    // ======================== set default shader
    auto defaultShader = assetManager->loadAsset<IShader>("${enginePath}/Resources/sg_shaders/features/postprocessing/layered/ssao.sgshader");

    setShader(defaultShader);
}

void SGCore::SSAO::onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept
{
    targetFrameBuffer->bind();

    if(!targetFrameBuffer->hasAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2))
    {
        targetFrameBuffer->addAttachment(
                SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2, // SSAO OCCLUSION ATTACHMENT
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
                SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3, // SSAO FINAL COLOR ATTACHMENT
                SGGColorFormat::SGG_RGBA,
                SGGColorInternalFormat::SGG_RGBA16_FLOAT,
                SGGDataType::SGG_FLOAT,
                0,
                0
        );
    }

    targetFrameBuffer->unbind();
}

void SGCore::SSAO::generateKernel() noexcept
{
    m_kernel.clear();

    std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
    std::default_random_engine generator;

    for(std::uint16_t i = 0; i < m_samplesCount; ++i)
    {
        // creating vec3 in range x: (-1, 1), y: (-1, 1), z: (0, 1)
        glm::vec3 sample(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator)
        );

        // normalizing sample for hemisphere (otherwise it will be a half cube)
        sample = glm::normalize(sample);
        // idk
        sample *= randomFloats(generator);

        // sample *= 0.1 + 0.9 * (i / (float) m_samplesCount) * (i / (float) m_samplesCount);
        float scale = (float) i / (float) m_samplesCount;
        scale = std::lerp(0.1f, 1.0f, scale * scale);

        // scaling vector along the axis
        sample *= scale;

        m_kernel.push_back(sample);
    }

    const glm::i32vec2 noiseTexSize { 4, 4 };

    std::vector<float> noiseBuf;
    for(std::uint32_t i = 0; i < noiseTexSize.x * noiseTexSize.y; ++i)
    {
        noiseBuf.push_back(randomFloats(generator) * 2.0 - 1.0);
        noiseBuf.push_back(randomFloats(generator) * 2.0 - 1.0);
        noiseBuf.push_back(0);
    }

    m_noise->create(noiseBuf.data(), noiseTexSize.x, noiseTexSize.y, 3, SGGColorInternalFormat::SGG_RGB16_FLOAT, SGGColorFormat::SGG_RGB);

    passValuesToSubPassShader();
}

void SGCore::SSAO::passValuesToSubPassShader() noexcept
{
    auto shader = getShader();

    if(!shader) return;

    shader->bind();

    shader->useInteger(m_name + "_samplesCount", m_samplesCount);

    shader->removeTextureBinding(m_name + "_noise");
    shader->addTextureBinding(m_name + "_noise", m_noise);

    std::uint16_t currentValIdx = 0;
    for(const auto& val : m_kernel)
    {
        shader->useVectorf(m_name + "_samples[" + std::to_string(currentValIdx) + "]", val);
        ++currentValIdx;
    }
}

std::uint16_t SGCore::SSAO::getSamplesCount() const noexcept
{
    return m_samplesCount;
}

void SGCore::SSAO::setSamplesCount(std::uint16_t samplesCount) noexcept
{
    m_samplesCount = samplesCount;

    generateKernel();
}