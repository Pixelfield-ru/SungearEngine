//
// Created by stuka on 07.11.2025.
//

#include "GLObjectsStorage.h"

#include "DeviceGLInfo.h"
#include "GL46/GL46Shader.h"
#include "SGCore/Graphics/API/GL/GL4/GL4FrameBuffer.h"
#include "SGCore/Graphics/API/GL/GL4/GL4Texture2D.h"
#include "SGCore/Graphics/API/GL/GL4/GL4UniformBuffer.h"
#include "SGCore/Graphics/API/GL/GLVertexArray.h"
#include "SGCore/Graphics/API/GL/GLVertexBuffer.h"
#include "SGCore/Graphics/API/GL/GLIndexBuffer.h"

void SGCore::GLObjectsStorage::recreateAll() noexcept
{
    // FIRST MUST DESTROY ALL OBJECT THEN CREATE ALL OBJECTS!!! ITS VERY IMPORTANT

    // ========================================== destroying textures
    for(const auto& texture : m_textures)
    {
        // skipping attachments
        if(texture->getFrameBufferAttachmentType() != SGFrameBufferAttachmentType::SGG_NOT_ATTACHMENT) continue;
        // skipping cubemaps
        if(texture->m_type == SGTextureType::SG_TEXTURE_CUBEMAP) continue;

        const auto lastHandle = (GLuint64) texture->getTextureNativeHandler();

        texture->destroyOnGPU();

        if(!texture->getData() && texture->m_type != SGTextureType::SG_TEXTURE_BUFFER)
        {
            LOG_W(SGCORE_TAG, "Unable to recreate OpenGL texture with handle '{}': RAM data has been deleted.", lastHandle)
        }
    }

    // ========================================== destroying framebuffers
    for(auto& frameBuffer : m_frameBuffers)
    {
        frameBuffer->destroy();
    }

    // ========================================== destroying shaders
    for(const auto& shader : m_shaders)
    {
        shader->destroy();
    }

    // =========================================== destroying VAOs
    for(const auto& vao : m_vertexArrays)
    {
        vao->destroy();

        for(auto* vbo : vao->getVertexBuffers())
        {
            vbo->destroy();
        }

        auto* ibo = vao->getIndexBuffer();

        if(ibo)
        {
            ibo->destroy();
        }
    }

    // ========================================== recreating uniform buffers
    for(const auto& uniformBuffer : m_uniformBuffers)
    {
        uniformBuffer->destroy();
    }

    // ===========================================================================
    // ===========================================================================
    // ===========================================================================

    // ========================================== recreating textures
    for(const auto& texture : m_textures)
    {
        // skipping attachments
        if(texture->getFrameBufferAttachmentType() != SGFrameBufferAttachmentType::SGG_NOT_ATTACHMENT) continue;
        // skipping cubemaps
        if(texture->m_type == SGTextureType::SG_TEXTURE_CUBEMAP) continue;

        texture->create();
    }

    // ========================================== recreating cubemaps
    // loading cubemaps after other texture because cubemaps using other textures and other textures must be loaded at the moment of cubemap creation
    for(const auto& texture : m_textures)
    {
        // skipping not cubemaps
        if(texture->m_type != SGTextureType::SG_TEXTURE_CUBEMAP) continue;

        texture->create();
    }

    // ======================================== recreating framebuffers
    for(auto& frameBuffer : m_frameBuffers)
    {
        frameBuffer->create();
        frameBuffer->bind();

        for(const auto& attachment : frameBuffer->getAttachments())
        {
            attachment.second->createAsFrameBufferAttachment(frameBuffer, attachment.first);
        }
    }

    // =========================================== recreating shaders
    for(const auto& shader : m_shaders)
    {
        shader->recompile();
    }

    // =========================================== recreating VAOs
    for(const auto& vao : m_vertexArrays)
    {
        vao->create();
        vao->bind();

        for(auto* vbo : vao->getVertexBuffers())
        {
            vbo->create();
            vbo->bind();
            vbo->putData(vbo->getData());
            vbo->useAttributes();
        }

        auto* ibo = vao->getIndexBuffer();

        if(ibo)
        {
            ibo->create();
            ibo->bind();
            ibo->putData(ibo->getData());
        }
    }

    // ========================================== recreating uniform buffers
    for(const auto& uniformBuffer : m_uniformBuffers)
    {
        uniformBuffer->prepare();
    }

    CoreMain::getRenderer()->useState(CoreMain::getRenderer()->getCachedRenderState(), true);
    CoreMain::getRenderer()->useMeshRenderState(CoreMain::getRenderer()->getCachedMeshRenderState(), true);
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
