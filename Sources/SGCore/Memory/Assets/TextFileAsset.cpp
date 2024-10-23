#include <fstream>

#include "TextFileAsset.h"

void SGCore::TextFileAsset::doLoad(const std::filesystem::path& path)
{
    m_data = SGCore::FileUtils::readFile(path);
}

std::string SGCore::TextFileAsset::getData() const noexcept
{
    return m_data;
}