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

const std::string& SGCore::SkeletalBoneAnimation::getBoneName() const noexcept
{
    return m_boneName;
}

void SGCore::SkeletalBoneAnimation::sortPositionKeys() noexcept
{
    std::sort(m_positionKeys.begin(), m_positionKeys.end(), [](const auto& k0, const auto& k1) {
        return k0.m_timeStamp < k1.m_timeStamp;
    });
}

void SGCore::SkeletalBoneAnimation::sortRotationKeys() noexcept
{
    std::sort(m_rotationKeys.begin(), m_rotationKeys.end(), [](const auto& k0, const auto& k1) {
        return k0.m_timeStamp < k1.m_timeStamp;
    });
}

void SGCore::SkeletalBoneAnimation::sortScaleKeys() noexcept
{
    std::sort(m_scaleKeys.begin(), m_scaleKeys.end(), [](const auto& k0, const auto& k1) {
        return k0.m_timeStamp < k1.m_timeStamp;
    });
}

void SGCore::SkeletalBoneAnimation::sortAllKeys() noexcept
{
    sortPositionKeys();
    sortRotationKeys();
    sortScaleKeys();
}

std::int64_t SGCore::SkeletalBoneAnimation::findPositionKeyByTime(const float& time) const noexcept
{
    const std::int64_t keysCount = std::ssize(m_positionKeys);

    if(keysCount == 1)
    {
        return 0;
    }

    for(std::int64_t i = 0; i < keysCount; ++i)
    {
        const auto& key = m_positionKeys[i];
        if(time < key.m_timeStamp)
        {
            return i;
        }
    }

    return keysCount - 1;
}

std::int64_t SGCore::SkeletalBoneAnimation::findRotationKeyByTime(const float& time) const noexcept
{
    const std::int64_t keysCount = std::ssize(m_rotationKeys);

    if(keysCount == 1)
    {
        return 0;
    }

    for(std::int64_t i = 0; i < keysCount; ++i)
    {
        const auto& key = m_rotationKeys[i];
        if(time < key.m_timeStamp)
        {
            return i;
        }
    }

    return keysCount - 1;
}

std::int64_t SGCore::SkeletalBoneAnimation::findScaleKeyByTime(const float& time) const noexcept
{
    const std::int64_t keysCount = std::ssize(m_scaleKeys);

    if(keysCount == 1)
    {
        return 0;
    }

    for(std::int64_t i = 0; i < keysCount; ++i)
    {
        const auto& key = m_scaleKeys[i];
        if(time < key.m_timeStamp)
        {
            return i;
        }
    }

    return keysCount - 1;
}
