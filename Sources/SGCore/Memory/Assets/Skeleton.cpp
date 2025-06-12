//
// Created by stuka on 05.01.2025.
//

#include "Skeleton.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/ImportedScenesArch/Bone.h"

void SGCore::Skeleton::doLoad(const SGCore::InterpolatedPath& path)
{

}

void SGCore::Skeleton::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{
    // filling bones map
    for(std::int64_t i = 0; i < std::ssize(m_allBones); ++i)
    {
        m_allBonesMap[m_allBones[i]->getName()] = i;
    }

    std::cout << "Skeleton::doLoadFromBinaryFile: m_allBonesMap size: " << m_allBonesMap.size() << std::endl;
}

void SGCore::Skeleton::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                        SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{

}

void SGCore::Skeleton::onMemberAssetsReferencesResolveImpl(SGCore::AssetManager* updatedAssetManager) noexcept
{
    for(std::int64_t i = 0; i < std::ssize(m_allBones); ++i)
    {
        auto& bone = m_allBones[i];
        AssetManager::resolveAssetReference(updatedAssetManager, bone);
    }

    AssetManager::resolveAssetReference(updatedAssetManager, m_rootBone);

    std::cout << "Skeleton::onMemberAssetsReferencesResolveImpl: m_allBones size: " << m_allBones.size() << std::endl;
}

void SGCore::Skeleton::addBone(AssetRef<Bone> bone, std::string boneName) noexcept
{
    if(m_allBonesMap.contains(boneName) || !bone) return;

    if(auto lastParentSkeleton = bone->m_parentSkeleton.lock())
    {
        lastParentSkeleton->removeBone(bone->getName());
    }

    bone->m_parentSkeleton = assetRef();

    m_allBonesMap[boneName] = m_allBones.size();
    bone->m_boneName = std::move(boneName);
    m_allBones.push_back(std::move(bone));
}

void SGCore::Skeleton::removeBone(const std::string& name) noexcept
{

}

void SGCore::Skeleton::setRootBone(AssetRef<Bone> rootBone, std::string boneName) noexcept
{
    if(!rootBone) return;

    auto foundIt = m_allBonesMap.find(boneName);
    // if this bone is already exists in skeleton and it is not root
    if(foundIt != m_allBonesMap.end() || m_allBones[foundIt->second] != 0)
    {
        removeBone(boneName);
    }
    else
    {
        if(auto lastParentSkeleton = rootBone->m_parentSkeleton.lock())
        {
            lastParentSkeleton->removeBone(rootBone->getName());
        }

        rootBone->m_parentSkeleton = assetRef();
    }

    m_rootBone = rootBone;

    if(m_allBones.empty())
    {
        m_allBones.push_back(std::move(rootBone));
        m_allBonesMap[boneName] = 0;
        return;
    }

    m_allBonesMap[boneName] = 0;
    rootBone->m_boneName = std::move(boneName);
    m_allBones[0] = std::move(rootBone);
}

const SGCore::Bone* SGCore::Skeleton::getRootBone() const noexcept
{
    return m_rootBone.get();
}

void SGCore::Skeleton::renameBone(const std::string& lastBoneName, const std::string& newBoneName) noexcept
{
    if(!m_allBonesMap.contains(lastBoneName) || m_allBonesMap.contains(newBoneName)) return;

    const std::int64_t boneIndex = m_allBonesMap[lastBoneName];

    m_allBones[boneIndex]->m_boneName = newBoneName;
    m_allBonesMap[newBoneName] = boneIndex;
    m_allBonesMap.erase(lastBoneName);
}

SGCore::Bone* SGCore::Skeleton::findBone(const std::string& boneName) const noexcept
{
    /*for(const auto& bone : m_allBones)
    {
        if(bone->getName() == boneName)
        {
            return bone;
        }
    }

    return nullptr;*/

    const auto foundIt = m_allBonesMap.find(boneName);
    return foundIt != m_allBonesMap.end() ? m_allBones[foundIt->second].get() : nullptr;
}

std::int64_t SGCore::Skeleton::bonesCount() const noexcept
{
    return std::ssize(m_allBones);
}

/*const std::vector<SGCore::AssetRef<SGCore::Bone>>& SGCore::Skeleton::getAllBones() const noexcept
{
    return m_allBones;
}*/
