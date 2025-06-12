//
// Created by ilya on 13.04.24.
//

#include "ByteFileAsset.h"

#include "SGCore/Memory/AssetManager.h"
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

void SGCore::ByteFileAsset::doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept
{
    m_dataBufferSize = m_dataMarkupInPackage.m_sizeInBytes;

    m_dataBuffer = parentAssetManager->getPackage().readBytes(m_dataMarkupInPackage.m_offset, m_dataMarkupInPackage.m_sizeInBytes);
}

void SGCore::ByteFileAsset::doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept
{
    doLoad(getPath());
}
