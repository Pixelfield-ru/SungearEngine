//
// Created by stuka on 07.05.2023.
//

#include <spdlog/spdlog.h>

#include "AssetManager.h"
#include "Assets/TextFileAsset.h"
#include "AssetsPackage.h"
#include "SGCore/Serde/Serde.h"
#include "SGCore/Serde/StandardSerdeSpecs.h"

#include <assimp/version.h>

void SGCore::AssetManager::init() noexcept
{
    m_instance = MakeScope<AssetManager>();
}

SGCore::Ref<SGCore::AssetManager>& SGCore::AssetManager::getInstance() noexcept
{
    return m_instance;
}

void SGCore::AssetManager::clear() noexcept
{
    std::lock_guard guard(m_mutex);

    m_assets.clear();
}

void SGCore::AssetManager::fullRemoveAsset(const std::filesystem::path& aliasOrPath) noexcept
{
    m_assets.erase(std::hash<std::filesystem::path>()(aliasOrPath));
}

void SGCore::AssetManager::createPackage(const std::filesystem::path& toDirectory, const std::string& packageName, bool saveAssetsData) noexcept
{
    const std::filesystem::path binaryFilePath = toDirectory / (packageName + ".bin");
    const std::filesystem::path markupFilePath = toDirectory / (packageName + ".json");

    AssetsPackage outPackage;
    outPackage.m_path = binaryFilePath;
    outPackage.m_useSerdeData = saveAssetsData;

    const std::string writtenJSON = Serde::Serializer::toFormat(Serde::FormatType::JSON, m_assets, outPackage);

    // writing markup (json) file
    FileUtils::writeToFile(markupFilePath, writtenJSON, false, true);
    // writing binary file
    FileUtils::writeBytes(binaryFilePath, 0, outPackage.m_buffer, false);
}
