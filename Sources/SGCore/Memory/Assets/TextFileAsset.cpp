#include <fstream>

#include "TextFileAsset.h"
#include "SGCore/Memory/AssetManager.h"

void SGCore::TextFileAsset::doLoad(const InterpolatedPath& path)
{
    m_data = SGCore::FileUtils::readFile(path.resolved());
}

std::string SGCore::TextFileAsset::getData() const noexcept
{
    return m_data;
}

void SGCore::TextFileAsset::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{
    m_data = parentAssetManager->getPackage().readData<std::string>(m_dataOffsetInPackage, m_dataSizeInPackage);
}
