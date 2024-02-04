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

    for(auto& texture : m_textures2D)
    {
        if(auto lockedTexture = texture.lock())
        {
            shader->addTexture(lockedTexture);
        }
    }

    m_shaders.push_back(shader);
}

void SGCore::GPUObjectsStorage::addTexture(const Ref<ITexture2D>& texture2D) noexcept
{
    texture2D->attachToUniqueNamesManager(m_uniqueNamesManager);
    m_textures2D.push_back(texture2D);

    for(auto& shader : m_shaders)
    {
        if(auto lockedShader = shader.lock())
        {
            lockedShader->addTexture(texture2D);
        }
    }
}

void SGCore::GPUObjectsStorage::addFramebuffer(const Ref<IFrameBuffer>& frameBuffer) noexcept
{
    frameBuffer->attachToUniqueNamesManager(m_uniqueNamesManager);
    // todo: notify shaders
    m_frameBuffers.push_back(frameBuffer);
}

// todo: smells like shit but i dont care
void SGCore::GPUObjectsStorage::onSomeObjectNameChanged(const std::string& newName) noexcept
{
    for(auto& texture : m_textures2D)
    {
        if(auto lockedTexture = texture.lock())
        {
            if(lockedTexture->getName() == newName)
            {
                for(auto& shader : m_shaders)
                {
                    if(auto lockedShader =  shader.lock())
                    {
                        // re-add texture
                        lockedShader->removeTexture(lockedTexture);
                        lockedShader->addTexture(lockedTexture);
                    }
                }
            }
        }
    }
}
