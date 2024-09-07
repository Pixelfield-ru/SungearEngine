//
// Created by ilya on 13.04.24.
//

#include "ByteFileAsset.h"
#include "SGCore/Utils/FileUtils.h"

SGCore::ByteFileAsset::~ByteFileAsset()
{
    delete m_dataBuffer;
}

void SGCore::ByteFileAsset::doLoad(const std::filesystem::path& path)
{
    m_dataBuffer = SGCore::FileUtils::readBytes(path, m_dataBufferSize);
}

char* SGCore::ByteFileAsset::getDataBuffer() const noexcept
{
    return m_dataBuffer;
}

size_t SGCore::ByteFileAsset::getDataBufferSize() const noexcept
{
    return m_dataBufferSize;
}
