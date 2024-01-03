//
// Created by Ilya on 20.12.2023.
//

#include "GPUObjectsStorage.h"

#include "API/ISubPassShader.h"
#include "API/ITexture2D.h"
#include "API/IFrameBuffer.h"

void SGCore::GPUObjectsStorage::addShader(const Ref<ISubPassShader>& shader) noexcept
{
    shader->attachToUniqueNamesManager(m_uniqueNamesManager);
    m_shaders[shader->getName()] = shader;
}

void SGCore::GPUObjectsStorage::addTexture(const Ref<ITexture2D>& texture2D) noexcept
{
    texture2D->attachToUniqueNamesManager(m_uniqueNamesManager);
    m_textures2D[texture2D->getName()] = texture2D;

    for(auto& shaderPair : m_shaders)
    {
        auto& shader = shaderPair.second;

        if(auto lockedShader = shader.lock())
        {
            lockedShader->addTexture(texture2D);
        }
    }
}

void SGCore::GPUObjectsStorage::addFramebuffer(const Ref<IFrameBuffer>& frameBuffer) noexcept
{
    frameBuffer->attachToUniqueNamesManager(m_uniqueNamesManager);
    m_frameBuffers[frameBuffer->getName()] = frameBuffer;
}
