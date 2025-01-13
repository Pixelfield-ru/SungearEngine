//
// Created by stuka on 05.01.2025.
//

#include "Skeleton.h"
#include "SGCore/Memory/AssetManager.h"

void SGCore::Skeleton::doLoad(const SGCore::InterpolatedPath& path)
{

}

void SGCore::Skeleton::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{

}

void SGCore::Skeleton::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                        SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{

}

void SGCore::Skeleton::onMemberAssetsReferencesResolveImpl(SGCore::AssetManager* updatedAssetManager) noexcept
{
    for(auto& bone : m_allBones)
    {
        AssetManager::resolveAssetReference(updatedAssetManager, bone);
    }

    AssetManager::resolveAssetReference(updatedAssetManager, m_rootBone);
}

SGCore::AssetRef<SGCore::Bone> SGCore::Skeleton::findBone(const std::string& boneName) const noexcept
{
    for(const auto& bone : m_allBones)
    {
        if(bone->m_boneName == boneName)
        {
            return bone;
        }
    }

    return nullptr;
}
