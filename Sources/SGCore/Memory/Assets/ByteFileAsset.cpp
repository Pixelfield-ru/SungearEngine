//
// Created by ilya on 13.04.24.
//

#include "ByteFileAsset.h"
#include "SGCore/Utils/FileUtils.h"

SGCore::ByteFileAsset::~ByteFileAsset()
{
    delete m_buffer;
}

void SGCore::ByteFileAsset::doLoad(const std::string& path)
{
    m_buffer = SGCore::FileUtils::readBytes(path, m_bufferSize);
}

char* SGCore::ByteFileAsset::getBuffer() const noexcept
{
    return m_buffer;
}

size_t SGCore::ByteFileAsset::getBufferSize() const noexcept
{
    return m_bufferSize;
}
