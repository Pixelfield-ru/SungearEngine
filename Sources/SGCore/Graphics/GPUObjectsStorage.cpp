//
// Created by Ilya on 20.12.2023.
//

#include "GPUObjectsStorage.h"

#include "API/IShader.h"
#include "API/ITexture2D.h"
#include "API/IFrameBuffer.h"

void SGCore::GPUObjectsStorage::addShader(const Ref<IShader>& shader) noexcept
{
    shader->attachToUniqueNamesManager(m_uniqueNamesManager.weak_from_this());
    m_shaders[shader->getName()] = shader;
}

void SGCore::GPUObjectsStorage::addTexture2D(const Ref<ITexture2D>& texture2D) noexcept
{
    texture2D->attachToUniqueNamesManager(m_uniqueNamesManager.weak_from_this());
    m_textures2D[texture2D->getName()] = texture2D;
}

void SGCore::GPUObjectsStorage::addFramebuffer(const Ref<IFrameBuffer>& frameBuffer) noexcept
{
    frameBuffer->attachToUniqueNamesManager(m_uniqueNamesManager.weak_from_this());
    m_frameBuffers[frameBuffer->getName()] = frameBuffer;
}
