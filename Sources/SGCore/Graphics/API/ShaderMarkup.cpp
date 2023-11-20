//
// Created by Ilya on 03.11.2023.
//

#include "ShaderMarkup.h"

void SGCore::ShaderMarkup::addTexturesBlockDeclaration(const SGTextureType& blockType,
                                                               const std::string& blockTypeName,
                                                               const std::uint8_t& maxTextures) noexcept
{
    addTexturesBlockDeclaration(blockType, blockTypeName, maxTextures, true);
}

void SGCore::ShaderMarkup::addTexturesBlockDeclaration(const SGTextureType& blockType,
                                                               const std::string& blockTypeName,
                                                               const std::uint8_t& maxTextures,
                                                               const bool& autoSamplersCount) noexcept
{
    m_texturesBlocks[blockType].m_type = blockType;
    m_texturesBlocks[blockType].m_name = blockTypeName;
    m_texturesBlocks[blockType].m_maximumTextures = maxTextures;
    m_texturesBlocks[blockType].m_autoSamplersCount = autoSamplersCount;
}

void SGCore::ShaderMarkup::calculateBlocksOffsets() noexcept
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

void SGCore::ShaderMarkup::addFrameBufferBlockDeclaration(const std::string& blockName,
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

void SGCore::ShaderMarkup::addFrameBufferBlockDeclaration(const std::string& blockName) noexcept
{
    addFrameBufferBlockDeclaration(blockName, 1, 1, 1, 1);
}
