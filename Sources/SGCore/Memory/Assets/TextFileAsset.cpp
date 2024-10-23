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

void SGCore::TextFileAsset::serializeToPackage(SGCore::AssetsPackage::AssetSection& currentAssetSection,
                                               bool isDataSerializing)
{
    currentAssetSection.addStandardInfo(this);

    if(isDataSerializing)
    {
        currentAssetSection.addSection("m_data", m_data);
    }
}
