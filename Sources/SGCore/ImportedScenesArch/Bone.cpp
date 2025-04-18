//
// Created by stuka on 06.01.2025.
//
#include "Bone.h"
#include "SGCore/Memory/AssetManager.h"

const std::string& SGCore::Bone::getName() const noexcept
{
    return m_boneName;
}

void SGCore::Bone::doLoad(const InterpolatedPath& path)
{
    LOG_NOT_SUPPORTED_FUNC(SGCORE_TAG);
}

void SGCore::Bone::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{
    auto& package = parentAssetManager->getPackage();

    for(auto& meshBoneData : m_affectedMeshesBoneData)
    {
        meshBoneData.m_weights = package.readData<std::vector<BoneVertexWeight>>(
                meshBoneData.m_weightsDataMarkupInPackage.m_offset,
                meshBoneData.m_weightsDataMarkupInPackage.m_sizeInBytes
        );
    }
}

void SGCore::Bone::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                    SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{
    LOG_NOT_SUPPORTED_FUNC(SGCORE_TAG);
}

void SGCore::Bone::onMemberAssetsReferencesResolveImpl(SGCore::AssetManager* updatedAssetManager) noexcept
{
    for(auto& bone : m_children)
    {
        AssetManager::resolveAssetReference(updatedAssetManager, bone);
    }
}
