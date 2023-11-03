//
// Created by Ilya on 03.11.2023.
//

#include "MarkedShader.h"

void Core::Graphics::MarkedShader::addBlockDeclaration(const SGTextureType& blockType,
                                                       const uint8_t& maxTextures,
                                                       const uint8_t& blockOffset)
{
    m_blocks[blockType].m_maximumTextures = maxTextures;
    m_blocks[blockType].m_texturesUnitOffset = blockOffset;
}

void Core::Graphics::MarkedShader::bind() const noexcept
{
    m_shader->bind();

    for(const auto& block : m_blocks)
    {
        std::string blockName = sgTextureTypeToString(block.first);

        std::uint8_t offset = block.second.m_texturesUnitOffset;
        std::uint8_t maxTex = block.second.m_maximumTextures;

        for(std::uint8_t blockIdx = offset; blockIdx < offset + maxTex; blockIdx++)
        {
            m_shader->useInteger(blockName + "Samplers[" + std::to_string(blockIdx - offset) + "]",
                                 blockIdx);
        }
    }
}
