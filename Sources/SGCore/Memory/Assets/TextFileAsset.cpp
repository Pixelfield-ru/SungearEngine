#include <fstream>

#include "TextFileAsset.h"

void SGCore::TextFileAsset::doLoad(const std::string& path)
{
    m_data = SGUtils::FileUtils::readFile(path);
    m_path = path;
}

std::string SGCore::TextFileAsset::getData() const noexcept
{
    return m_data;
}