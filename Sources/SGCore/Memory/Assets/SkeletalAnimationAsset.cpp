//
// Created by stuka on 05.01.2025.
//
#include "SkeletalAnimationAsset.h"

void SGCore::SkeletalAnimationAsset::doLoad(const InterpolatedPath& path)
{
    LOG_NOT_SUPPORTED_FUNC(SGCORE_TAG);
}

void SGCore::SkeletalAnimationAsset::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{
    auto& package = parentAssetManager->getPackage();

    for(auto& boneAnimPair : m_bonesAnimations)
    {
        auto& boneAnim = boneAnimPair.second;

        const auto& posKeysMarkup = boneAnim.m_positionKeysMarkupInPackage;
        const auto& rotKeysMarkup = boneAnim.m_rotationKeysMarkupInPackage;
        const auto& scaleKeysMarkup = boneAnim.m_scaleKeysMarkupInPackage;

        boneAnim.m_positionKeys = package.readData<std::vector<KeyPosition>>(posKeysMarkup.m_offset, posKeysMarkup.m_sizeInBytes);
        boneAnim.m_rotationKeys = package.readData<std::vector<KeyRotation>>(rotKeysMarkup.m_offset, rotKeysMarkup.m_sizeInBytes);
        boneAnim.m_scaleKeys = package.readData<std::vector<KeyScale>>(scaleKeysMarkup.m_offset, scaleKeysMarkup.m_sizeInBytes);
    }
}

void SGCore::SkeletalAnimationAsset::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                                      SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{
    LOG_NOT_SUPPORTED_FUNC(SGCORE_TAG);
}

void SGCore::SkeletalAnimationAsset::changeBoneName(const std::string& lastBoneName,
                                                    const std::string& newBoneName) noexcept
{
    auto boneAnimationIt = m_bonesAnimations.find(lastBoneName);
    if(boneAnimationIt != m_bonesAnimations.end())
    {
        auto&& boneAnim = m_bonesAnimations[lastBoneName];
        boneAnim.m_boneName = newBoneName;

        m_bonesAnimations[newBoneName] = std::move(boneAnim);
        m_bonesAnimations.erase(lastBoneName);
    }
}

SGCore::SkeletalBoneAnimation* SGCore::SkeletalAnimationAsset::getBoneAnimation(const std::string& boneName) noexcept
{
    auto boneAnimationIt = m_bonesAnimations.find(boneName);
    if(boneAnimationIt == m_bonesAnimations.end()) return nullptr;

    return &boneAnimationIt->second;
}

size_t SGCore::SkeletalAnimationAsset::getBonesAnimationsCount() const noexcept
{
    return m_bonesAnimations.size();
}

const std::string& SGCore::SkeletalBoneAnimation::getBoneName() const noexcept
{
    return m_boneName;
}

void SGCore::SkeletalBoneAnimation::sortAllKeys() noexcept
{
    sortKeys(&SkeletalBoneAnimation::m_positionKeys);
    sortKeys(&SkeletalBoneAnimation::m_rotationKeys);
    sortKeys(&SkeletalBoneAnimation::m_scaleKeys);
}

std::int64_t SGCore::SkeletalBoneAnimation::findPositionKeyByTime(float time) const noexcept
{
    return findKeyByTime(&SkeletalBoneAnimation::m_positionKeys, time);
}

std::int64_t SGCore::SkeletalBoneAnimation::findRotationKeyByTime(float time) const noexcept
{
    return findKeyByTime(&SkeletalBoneAnimation::m_rotationKeys, time);
}

std::int64_t SGCore::SkeletalBoneAnimation::findScaleKeyByTime(float time) const noexcept
{
    return findKeyByTime(&SkeletalBoneAnimation::m_scaleKeys, time);
}
