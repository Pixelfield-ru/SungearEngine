//
// Created by stuka on 08.07.2025.
//

#include "CSMTarget.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/ITexture2D.h"

SGCore::CSMTarget::CSMTarget()
{
    CSMCascade cascade0;
    cascade0.m_level = 250.0f;
    cascade0.m_frameBufferSize = { 1024 * 4, 1024 * 4 };
    cascade0.m_bias = 0.0012f;
    cascade0.createFrameBuffer();
    m_levels.push_back(cascade0);

    CSMCascade cascade1;
    cascade1.m_level = 150.0f;
    cascade1.m_frameBufferSize = { 1024 * 4, 1024 * 4 };
    cascade1.m_bias = 0.002f;
    cascade1.createFrameBuffer();
    m_levels.push_back(cascade1);

    CSMCascade cascade2;
    cascade2.m_level = 50.0f;
    cascade2.m_frameBufferSize = { 1024 * 4, 1024 * 4 };
    cascade2.m_bias = 0.004f;
    cascade2.createFrameBuffer();
    m_levels.push_back(cascade2);

    CSMCascade cascade3;
    cascade3.m_level = 10.0f;
    cascade3.m_frameBufferSize = { 1024 * 4, 1024 * 4 };
    cascade3.m_bias = 0.014f;
    cascade3.createFrameBuffer();
    m_levels.push_back(cascade3);
}

const std::vector<SGCore::CSMCascade>& SGCore::CSMTarget::getCascades() const noexcept
{
    return m_levels;
}

SGCore::CSMCascade& SGCore::CSMTarget::getCascade(std::uint8_t idx) noexcept
{
    return m_levels[idx];
}

const SGCore::CSMCascade& SGCore::CSMTarget::getCascade(std::uint8_t idx) const noexcept
{
    return m_levels[idx];
}

int SGCore::CSMTarget::bindUniformsToShader(IShader* forShader, float cameraZFar, int texUnitOffset) const noexcept
{
    forShader->useInteger("CSMCascadesCount", m_levels.size());
    for(std::uint8_t i = 0; i < m_levels.size(); ++i)
    {
        const auto& cascade = m_levels[i];
        forShader->useMatrix("CSMLightSpaceMatrix[" + std::to_string(i) + "]", cascade.m_projectionSpaceMatrix);
        forShader->useFloat("CSMCascadesPlanesDistances[" + std::to_string(i) + "]", cameraZFar / cascade.m_level);
        forShader->useFloat("CSMCascadesBiases[" + std::to_string(i) + "]", cascade.m_bias);
        forShader->useTextureBlock("CSMShadowMaps[" + std::to_string(i) + "]", texUnitOffset + i);
        cascade.m_frameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0)->bind(texUnitOffset + i);
        // cascade.m_frameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT0)->bind(texUnitOffset + i);
    }

    return m_levels.size() + texUnitOffset + 1;
}
