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

bool SGCore::AssetManager::isAssetExists(const std::string& pathOrAlias, const size_t& assetTypeID) noexcept
{
    std::lock_guard guard(m_mutex);

    auto foundVariantsIt = m_assets.find(hashString(pathOrAlias));
    if(foundVariantsIt == m_assets.end())
    {
        return false;
    }
    else
    {
        auto foundAssetIt = foundVariantsIt->second.find(assetTypeID);
        return foundAssetIt != foundVariantsIt->second.end();
    }
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

    // setting m_hasBeenSerialized to false. that means that no one asset was serialized
    for(auto& variantsIt : m_assets)
    {
        for(auto& asset : variantsIt.second)
        {
            asset.second->m_hasBeenSerialized = false;
        }
    }

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
    decltype(m_assets) loadedAssets;
    Serde::Serializer::fromFormat(FileUtils::readFile(markupFilePath), loadedAssets, Serde::FormatType::JSON, outputLog, m_package);

    // adding only assets that were not loaded in current asset manager
    for(auto& variantsIt : loadedAssets)
    {
        const auto& pathOrAliasHash = variantsIt.first;
        auto& assetsByPathOrAlias = variantsIt.second;

        // if no asset was loaded by path or alias with hash equals to 'variantsIt.first'
        // then we are adding all assets from 'variantsIt.second' m_assets
        if(!m_assets.contains(pathOrAliasHash))
        {
            LOG_D(SGCORE_TAG, "Loaded bunch of assets from .json and .bin files. Assets path or alias hash: {}", pathOrAliasHash);
            m_assets[pathOrAliasHash] = std::move(assetsByPathOrAlias);
            continue;
        }

        auto& alreadyLoadedAssetsByPathOrAlias = m_assets[pathOrAliasHash];

        for(auto& assetIt : assetsByPathOrAlias)
        {
            const auto& assetTypeID = assetIt.first;
            auto& asset = assetIt.second;

            // if asset 'asset' does not exist in current asset manager
            if(!alreadyLoadedAssetsByPathOrAlias.contains(assetTypeID))
            {
                LOG_D(SGCORE_TAG, "Loaded new asset from .json and .bin files. Asset path or alias hash: {}, asset type ID: {}", pathOrAliasHash, assetTypeID);
                alreadyLoadedAssetsByPathOrAlias[assetTypeID] = std::move(asset);
            }
        }
    }

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
