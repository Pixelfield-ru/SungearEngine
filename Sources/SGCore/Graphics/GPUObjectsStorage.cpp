//
// Created by Ilya on 20.12.2023.
//

#include "GPUObjectsStorage.h"

#include "API/IShader.h"
#include "API/ITexture2D.h"
#include "API/IFrameBuffer.h"

void SGCore::GPUObjectsStorage::addShader(const SGCore::Ref<SGCore::IShader>& shader) noexcept
{
    m_shaders[shader.getName()] = shader;
}

void SGCore::GPUObjectsStorage::addTexture2D(const SGCore::Ref<SGCore::ITexture2D>& texture2D) noexcept
{

}

void SGCore::GPUObjectsStorage::addFramebuffer(const SGCore::Ref<SGCore::IFrameBuffer>& frameBuffer) noexcept
{

}
