//
// Created by Ilya on 03.11.2023.
//

#include "MarkedShader.h"

void Core::Graphics::MarkedShader::addTexturesBlockDeclaration(const SGTextureType& blockType,
                                                               const std::string& blockTypeName,
                                                               const std::uint8_t& maxTextures) noexcept
{
    addTexturesBlockDeclaration(blockType, blockTypeName, maxTextures, true);
}

void Core::Graphics::MarkedShader::addTexturesBlockDeclaration(const SGTextureType& blockType,
                                                               const std::string& blockTypeName,
                                                               const std::uint8_t& maxTextures,
                                                               const bool& autoSamplersCount) noexcept
{
    m_texturesBlocks[blockType].m_type = blockType;
    m_texturesBlocks[blockType].m_name = blockTypeName;
    m_texturesBlocks[blockType].m_maximumTextures = maxTextures;
    m_texturesBlocks[blockType].m_autoSamplersCount = autoSamplersCount;
}

void Core::Graphics::MarkedShader::calculateBlocksOffsets() noexcept
{
    std::uint8_t offset = 0;
    for(auto& block : m_texturesBlocks)
    {
        block.second.m_offset = offset;
        offset += block.second.m_maximumTextures;
    }

    for(auto& block : m_frameBuffersAttachmentsBlocks)
    {
        block.second.m_offset = offset;
        offset += block.second.m_maxDepthAttachments +
                  block.second.m_maxDepthStencilAttachments +
                  block.second.m_maxColorAttachments +
                  block.second.m_maxRenderAttachments;
    }
}

void Core::Graphics::MarkedShader::bind() const noexcept
{
    m_shader->bind();

    for(const auto& block : m_texturesBlocks)
    {
        std::uint8_t offset = block.second.m_offset;
        std::uint8_t maxTex = block.second.m_maximumTextures;

        for(std::uint8_t blockIdx = offset; blockIdx < offset + maxTex; ++blockIdx)
        {
            m_shader->useInteger(block.second.m_name + "[" + std::to_string(blockIdx - offset) + "]",
                                 blockIdx);
        }
    }

    for(const auto& block : m_frameBuffersAttachmentsBlocks)
    {
        std::uint8_t offset = block.second.m_offset;

        for(std::uint8_t blockIdx = offset; blockIdx < offset + block.second.m_maxDepthAttachments; ++blockIdx)
        {
            m_shader->useInteger(block.second.m_name + ".depthAttachments[" + std::to_string(blockIdx - offset) + "]",
                                 blockIdx);
        }

        offset += block.second.m_maxDepthAttachments;

        // -------------

        for(std::uint8_t blockIdx = offset; blockIdx < offset + block.second.m_maxDepthStencilAttachments; ++blockIdx)
        {
            m_shader->useInteger(block.second.m_name + ".depthStencilAttachments[" + std::to_string(blockIdx - offset) + "]",
                                 blockIdx);
        }

        offset += block.second.m_maxDepthStencilAttachments;

        // -------------

        for(std::uint8_t blockIdx = offset; blockIdx < offset + block.second.m_maxColorAttachments; ++blockIdx)
        {
            m_shader->useInteger(block.second.m_name + ".colorAttachments[" + std::to_string(blockIdx - offset) + "]",
                                 blockIdx);
        }

        offset += block.second.m_maxColorAttachments;

        // -------------

        for(std::uint8_t blockIdx = offset; blockIdx < offset + block.second.m_maxRenderAttachments; ++blockIdx)
        {
            m_shader->useInteger(block.second.m_name + ".renderAttachments[" + std::to_string(blockIdx - offset) + "]",
                                 blockIdx);
        }
    }
}

void Core::Graphics::MarkedShader::addFrameBufferBlockDeclaration(const std::string& blockName,
                                                                  const uint8_t& maxDepthAttachments,
                                                                  const uint8_t& maxDepthStencilAttachments,
                                                                  const uint8_t& maxColorAttachments,
                                                                  const uint8_t& maxRenderAttachments) noexcept
{
    m_frameBuffersAttachmentsBlocks[blockName].m_name = blockName;

    m_frameBuffersAttachmentsBlocks[blockName].m_maxDepthAttachments = maxDepthAttachments;
    m_frameBuffersAttachmentsBlocks[blockName].m_maxDepthStencilAttachments = maxDepthStencilAttachments;
    m_frameBuffersAttachmentsBlocks[blockName].m_maxColorAttachments = maxColorAttachments;
    m_frameBuffersAttachmentsBlocks[blockName].m_maxRenderAttachments = maxRenderAttachments;
}

void Core::Graphics::MarkedShader::addFrameBufferBlockDeclaration(const std::string& blockName) noexcept
{
    addFrameBufferBlockDeclaration(blockName, 1, 1, 1, 1);
}
