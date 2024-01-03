//
// Created by stuka on 24.12.2023.
//

#include "ShaderTexturesFromMaterialBlock.h"

#include "API/ISubPassShader.h"
#include "API/ITexture2D.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"

void SGCore::ShaderTexturesFromMaterialBlock::addTexture(const SGCore::Ref<SGCore::ITexture2D>& texture, SGTextureType textureType) noexcept
{
    // m_textures.push_back(texture2D);

    if(textureType != m_typeToCollect) return;

    // if texture does not exist
    if(std::find_if(m_textures.begin(), m_textures.end(), [&texture](auto otherTexture)
                    {
                        return !(otherTexture.owner_before(texture) || texture.owner_before(otherTexture));
                    }
    ) == m_textures.end())
    {
        m_textures.push_back(texture);
    }

    if(auto lockedShader = m_parentShader.lock())
    {
        lockedShader->onTexturesCountChanged();
    }
}

void SGCore::ShaderTexturesFromMaterialBlock::removeTexture(const SGCore::Ref<SGCore::ITexture2D>& texture) noexcept
{
    size_t removedCnt = m_textures.remove_if([&texture](auto otherTexture) {
        return !(otherTexture.owner_before(texture) || texture.owner_before(otherTexture));
    });

    if(removedCnt > 0)
    {
        if(auto lockedShader = m_parentShader.lock())
        {
            lockedShader->onTexturesCountChanged();
        }
    }
}

void SGCore::ShaderTexturesFromMaterialBlock::clearTextures() noexcept
{
    m_textures.clear();

    if(auto lockedShader = m_parentShader.lock())
    {
        lockedShader->onTexturesCountChanged();
    }
}

void SGCore::ShaderTexturesFromMaterialBlock::collectTexturesFromMaterial
(const SGCore::Ref<SGCore::IMaterial>& material) noexcept
{
    for(const auto& typesPair : material->m_textures)
    {
        if(typesPair.first != m_typeToCollect) continue;

        for(const auto& texture : typesPair.second)
        {
            // if texture does not exist
            if(std::find_if(m_textures.begin(), m_textures.end(), [&texture](auto otherTexture) {
                return !(otherTexture.owner_before(texture) || texture.owner_before(otherTexture));
            }) == m_textures.end())
            {
                m_textures.push_back(texture);
            }
        }
    }

    if(auto lockedShader = m_parentShader.lock())
    {
        lockedShader->onTexturesCountChanged();
    }
}
