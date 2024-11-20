//
// Created by stuka on 29.08.2023.
//

#include "ICubemapTexture.h"
#include "SGCore/Memory/AssetManager.h"

void SGCore::ICubemapTexture::onMemberAssetsReferencesResolveImpl(SGCore::AssetManager* updatedAssetManager) noexcept
{
    for(auto& part : m_parts)
    {
        AssetManager::resolveAssetReference(updatedAssetManager, part);
    }
}
