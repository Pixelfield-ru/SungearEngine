//
// Created by stuka on 26.11.2024.
//

#include "SSAO.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Graphics/API/ISubPassShader.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/API/ITexture2D.h"

#include <random>

SGCore::SSAO::SSAO()
{
    m_name = "SG_SSAO";

    m_noise = Ref<ITexture2D>(CoreMain::getRenderer()->createTexture2D());
    generateKernel();
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

    std::vector<float> noiseBuf;
    for(std::uint32_t i = 0; i < 16; ++i)
    {
        noiseBuf.push_back(randomFloats(generator) * 2.0 - 1.0);
        noiseBuf.push_back(randomFloats(generator) * 2.0 - 1.0);
        noiseBuf.push_back(0);
    }

    m_noise->create(noiseBuf.data(), 4, 4, 3, SGGColorInternalFormat::SGG_RGB16_FLOAT, SGGColorFormat::SGG_RGB);

    for(const auto& parentLayer : m_parentPostProcessLayers)
    {
        auto lockedParentLayer = parentLayer.lock();
        passValuesToSubPassShader(lockedParentLayer->getFXSubPassShader());
    }
}

void SGCore::SSAO::passValuesToSubPassShader(const Ref<ISubPassShader>& subPassShader) noexcept
{
    if(subPassShader)
    {
        subPassShader->bind();

        subPassShader->useInteger(m_name + "_samplesCount", m_samplesCount);

        subPassShader->removeTextureBinding(m_name + "_noise");
        subPassShader->addTextureBinding(m_name + "_noise", m_noise);

        std::uint16_t currentValIdx = 0;
        for(const auto& val : m_kernel)
        {
            subPassShader->useVectorf(m_name + "_samples[" + std::to_string(currentValIdx) + "]", val);
            ++currentValIdx;
        }
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