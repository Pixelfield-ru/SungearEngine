//
// Created by stuka on 07.11.2025.
//

#include "GLObjectsStorage.h"

#include "GL46/GL46Shader.h"
#include "SGCore/Graphics/API/GL/GL4/GL4FrameBuffer.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Texture2D.h"
#include "SGCore/Graphics/API/GL/GL4/GL4UniformBuffer.h"
#include "SGCore/Graphics/API/GL/GLVertexArray.h"
#include "SGCore/Graphics/API/GL/GLVertexBuffer.h"
#include "SGCore/Graphics/API/GL/GLIndexBuffer.h"

void SGCore::GLObjectsStorage::recreateAll() noexcept
{
    // ======================================== recreating framebuffers
    for(auto& frameBuffer : m_frameBuffers)
    {
        for(const auto& attachment : frameBuffer->getAttachments())
        {
            attachment.second->destroyOnGPU();
        }

        frameBuffer->destroy();
        frameBuffer->create();
        frameBuffer->bind();

        for(const auto& attachment : frameBuffer->getAttachments())
        {
            attachment.second->createAsFrameBufferAttachment(frameBuffer, attachment.first);
        }
    }

    // ========================================== recreating textures
    for(const auto& texture : m_textures)
    {
        // skipping attachments
        if(texture->getFrameBufferAttachmentType() != SGFrameBufferAttachmentType::SGG_NOT_ATTACHMENT) continue;

        const auto handle = (GLuint64) texture->getTextureNativeHandler();

        texture->destroyOnGPU();

        if(!texture->getData() && !texture->m_isTextureBuffer)
        {
            LOG_W(SGCORE_TAG, "Unable to recreate OpenGL texture with handle '{}': RAM data has been deleted.", handle)
            continue;
        }
        texture->create();
    }

    // =========================================== recreating shaders
    for(const auto& shader : m_shaders)
    {
        shader->recompile();
    }

    // =========================================== recreating VAOs
    for(const auto& vao : m_vertexArrays)
    {
        vao->destroy();

        vao->create();
        vao->bind();

        for(auto* vbo : vao->getVertexBuffers())
        {
            vbo->destroy();

            vbo->create();
            vbo->bind();
            vbo->putData(vbo->getData());
            vbo->useAttributes();
        }

        auto* ibo = vao->getIndexBuffer();

        if(ibo)
        {
            ibo->destroy();

            ibo->create();
            ibo->bind();
            ibo->putData(ibo->getData());
        }
    }

    // ========================================== recreating uniform buffers
    for(const auto& uniformBuffer : m_uniformBuffers)
    {
        uniformBuffer->destroy();
        uniformBuffer->prepare();
    }
}

void SGCore::GLObjectsStorage::clear() noexcept
{
    // ======================================== destroying framebuffers
    for(auto& frameBuffer : m_frameBuffers)
    {
        for(const auto& attachment : frameBuffer->getAttachments())
        {
            attachment.second->destroyOnGPU();
        }

        frameBuffer->destroy();
    }

    // ========================================== destroying textures
    for(const auto& texture : m_textures)
    {
        // skipping attachments
        if(texture->getFrameBufferAttachmentType() != SGFrameBufferAttachmentType::SGG_NOT_ATTACHMENT) continue;

        texture->destroyOnGPU();
    }

    // =========================================== destroying shaders
    for(const auto& shader : m_shaders)
    {
        shader->destroy();
    }

    // =========================================== destroying VAOs
    for(const auto& vao : m_vertexArrays)
    {
        vao->destroy();

        for(const auto& vbo : vao->getVertexBuffers())
        {
            vao->destroy();
        }

        if(vao->getIndexBuffer())
        {
            vao->getIndexBuffer()->destroy();
        }
    }

    // ========================================== destroying uniform buffers
    for(const auto& uniformBuffer : m_uniformBuffers)
    {
        uniformBuffer->destroy();
    }
}
