//
// Created by ilya on 13.04.24.
//

#include "ByteFileAsset.h"
#include "SGCore/Utils/FileUtils.h"

SGCore::ByteFileAsset::~ByteFileAsset()
{
    delete m_dataBuffer;
}

void SGCore::ByteFileAsset::doLoad(const InterpolatedPath& path)
{
    m_dataBuffer = SGCore::FileUtils::readBytes(path.resolved(), m_dataBufferSize);
}

char* SGCore::ByteFileAsset::getDataBuffer() const noexcept
{
    return m_dataBuffer;
}

size_t SGCore::ByteFileAsset::getDataBufferSize() const noexcept
{
    return m_dataBufferSize;
}

void SGCore::ByteFileAsset::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{

}

void SGCore::ByteFileAsset::doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept
{
    doLoad(getPath());
}
