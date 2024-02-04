//
// Created by stuka on 24.12.2023.
//

#include "TexturesFromGlobalStorageBlock.h"

#include "API/ISubPassShader.h"
#include "API/ITexture2D.h"

void SGCore::TexturesFromGlobalStorageBlock::addTexture(const SGCore::Ref<SGCore::ITexture2D>& texture2D) noexcept
{
    // if texture with this name requires
    if (std::find(m_requiredTexturesNames.begin(), m_requiredTexturesNames.end(), texture2D->getName()) !=
        m_requiredTexturesNames.end())
    {
        m_requiredTexturesNames.remove(texture2D->getName());

        m_textures.push_back(texture2D);

        if(auto lockedShader = m_parentShader.lock())
        {
            lockedShader->onTexturesCountChanged();
        }
    }
}

void SGCore::TexturesFromGlobalStorageBlock::removeTexture
        (const SGCore::Ref<SGCore::ITexture2D>& texture2D) noexcept
{
    size_t removedCnt = m_textures.remove_if([&texture2D](auto otherTexture2D) {
        return !(otherTexture2D.owner_before(texture2D) || texture2D.owner_before(otherTexture2D));
    });

    if(removedCnt > 0)
    {
        m_requiredTexturesNames.push_back(texture2D->getName());

        if(auto lockedShader = m_parentShader.lock())
        {
            lockedShader->onTexturesCountChanged();
        }
    }
}

void SGCore::TexturesFromGlobalStorageBlock::clearTextures() noexcept
{
    auto currIter = m_textures.begin();
    while(currIter != m_textures.end())
    {
        m_requiredTexturesNames.push_back(currIter->lock()->getName());
        currIter = m_textures.erase(currIter);
    }

    if(auto lockedShader = m_parentShader.lock())
    {
        lockedShader->onTexturesCountChanged();
    }
}
