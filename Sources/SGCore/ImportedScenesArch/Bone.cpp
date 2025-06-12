//
// Created by stuka on 06.01.2025.
//
#include "Bone.h"

#include "IMeshData.h"
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

        /*if(meshBoneData.m_affectedMesh)
        {
            for(const auto& weight : meshBoneData.m_weights)
            {
                meshBoneData.m_affectedMesh->m_vertices[weight.m_vertexIdx].addWeightData(weight.m_weight, m_id);
            }
        }*/
    }

    std::cout << "SGCore::Bone::doLoadFromBinaryFile: path: " << getPath().raw() << ", m_affectedMeshesBoneData size: " << m_affectedMeshesBoneData.size() << std::endl;
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

        bone->m_parent = this;
    }

    std::cout << "SGCore::Bone::onMemberAssetsReferencesResolveImpl: path: " << getPath().raw() << "" << std::endl;
}
