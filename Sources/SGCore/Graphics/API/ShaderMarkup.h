//
// Created by Ilya on 03.11.2023.
//

#ifndef SUNGEARENGINE_MARKEDSHADER_H
#define SUNGEARENGINE_MARKEDSHADER_H

#include <string>
#include <cstdint>
#include <unordered_map>
#include "GraphicsDataTypes.h"

namespace Core::Graphics
{
    struct MarkedTexturesBlock
    {
        SGTextureType m_type = SGTextureType::SGTP_DIFFUSE;
        std::string m_name;
        // The maximum number of textures of this type
        std::uint8_t m_maximumTextures = 1;
        // Offset for texture block (beginning)
        std::uint8_t m_offset = 0;
        // will the number of samplers be automatically counted and passed to the shader
        bool m_autoSamplersCount = true;
    };

    struct MarkedFrameBufferAttachmentsBlock
    {
        std::uint8_t m_offset = 0;

        std::uint8_t m_maxDepthAttachments = 1;
        std::uint8_t m_maxDepthStencilAttachments = 1;
        std::uint8_t m_maxColorAttachments = 1;
        std::uint8_t m_maxRenderAttachments = 1;

        std::string m_name = "sgfb_unknown";
    };

    struct ShaderMarkup
    {
        void addTexturesBlockDeclaration(const SGTextureType& blockType,
                                         const std::string& blockTypeName,
                                         const std::uint8_t& maxTextures) noexcept;

        void addTexturesBlockDeclaration(const SGTextureType& blockType,
                                         const std::string& blockTypeName,
                                         const std::uint8_t& maxTextures,
                                         const bool& autoSamplersCount) noexcept;

        void addFrameBufferBlockDeclaration(const std::string& blockName,
                                            const std::uint8_t& maxDepthAttachments,
                                            const std::uint8_t& maxDepthStencilAttachments,
                                            const std::uint8_t& maxColorAttachments,
                                            const std::uint8_t& maxRenderAttachments) noexcept;

        void addFrameBufferBlockDeclaration(const std::string& blockName) noexcept;

        void calculateBlocksOffsets() noexcept;

        // Blocks of textures that correspond to a specific type of texture
        std::unordered_map<SGTextureType, MarkedTexturesBlock> m_texturesBlocks;

        // FRAME BUFFER ATTACHMENTS UNITS ARE ALWAYS AFTER SIMPLE TEXTURES. SO YOU NEED TO KEEP IN MIND THAT TEXTURES + ATTACHMENTS < MAX GAPI SAMPLERS IN ONE SHADER
        // first - block name
        std::unordered_map<std::string, MarkedFrameBufferAttachmentsBlock> m_frameBuffersAttachmentsBlocks;
    };
}

#endif // SUNGEARENGINE_MARKEDSHADER_H
