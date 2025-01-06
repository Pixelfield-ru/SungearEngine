//
// Created by stuka on 06.01.2025.
//
#include "Bone.h"

void SGCore::Bone::doLoad(const InterpolatedPath& path)
{
    LOG_NOT_SUPPORTED_FUNC(SGCORE_TAG);
}

void SGCore::Bone::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{

}

void SGCore::Bone::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                    SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{
    LOG_NOT_SUPPORTED_FUNC(SGCORE_TAG);
}
