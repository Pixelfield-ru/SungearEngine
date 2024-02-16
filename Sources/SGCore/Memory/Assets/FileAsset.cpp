#include <fstream>

#include "FileAsset.h"

void SGCore::FileAsset::load(const std::string& path)
{
    m_data = SGUtils::FileUtils::readFile(path);
    m_path = path;
}

std::string SGCore::FileAsset::getData() const noexcept
{
    return m_data;
}