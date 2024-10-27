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

void SGCore::AssetManager::createPackage(const std::filesystem::path& toDirectory,
                                         const std::string& packageName,
                                         bool saveAssetsData) noexcept
{
    const std::filesystem::path binaryFilePath = toDirectory / (packageName + ".bin");
    const std::filesystem::path markupFilePath = toDirectory / (packageName + ".json");

    m_package.m_buffer.clear();

    m_package.m_path = binaryFilePath;
    m_package.m_useSerdeData = saveAssetsData;
    m_package.m_parentAssetManager = this;

    const std::string writtenJSON = Serde::Serializer::toFormat(Serde::FormatType::JSON, m_assets, m_package);

    // writing markup (json) file
    FileUtils::writeToFile(markupFilePath, writtenJSON, false, true);
    // writing binary file
    FileUtils::writeBytes(binaryFilePath, 0, m_package.m_buffer, false);
}

void SGCore::AssetManager::loadPackage(const std::filesystem::path& fromDirectory,
                                       const std::string& packageName) noexcept
{
    const std::filesystem::path binaryFilePath = fromDirectory / (packageName + ".bin");
    const std::filesystem::path markupFilePath = fromDirectory / (packageName + ".json");

    m_package.m_path = binaryFilePath;
    m_package.m_parentAssetManager = this;

    std::string outputLog;
    // Serde::Serializer::fromFormat(FileUtils::readFile(markupFilePath), m_assets, Serde::FormatType::JSON, outputLog, m_package);

    if(!outputLog.empty())
    {
        LOG_E(SGCORE_TAG, "Error while loading package with assets (directory of package: '{}', name of package '{}'): {}",
              Utils::toUTF8(fromDirectory.u16string()), packageName, outputLog);
    }
}

const SGCore::AssetsPackage& SGCore::AssetManager::getPackage() const noexcept
{
    return m_package;
}
