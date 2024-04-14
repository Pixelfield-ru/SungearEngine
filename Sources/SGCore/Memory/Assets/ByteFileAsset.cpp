//
// Created by ilya on 13.04.24.
//

#include "ByteFileAsset.h"
#include "SGUtils/FileUtils.h"

SGCore::ByteFileAsset::~ByteFileAsset()
{
    delete m_buffer;
}

void SGCore::ByteFileAsset::load(const std::string& path)
{
    m_buffer = SGUtils::FileUtils::readBytes(path, m_bufferSize);
    m_path = path;
}

char* SGCore::ByteFileAsset::getBuffer() const noexcept
{
    return m_buffer;
}

size_t SGCore::ByteFileAsset::getBufferSize() const noexcept
{
    return m_bufferSize;
}
