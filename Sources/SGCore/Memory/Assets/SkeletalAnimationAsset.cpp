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

