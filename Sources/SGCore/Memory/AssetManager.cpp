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
    m_instance = getAssetManager("MainAssetManager");
}

void SGCore::AssetManager::addStandardAssets() noexcept
{
    if(!isAssetExists<IMeshData>("quad_mesh"))
    {
        auto quad = createAssetWithAlias<IMeshData>("quad_mesh");

        quad->m_indices.push_back(0);
        quad->m_indices.push_back(2);
        quad->m_indices.push_back(1);

        quad->m_indices.push_back(0);
        quad->m_indices.push_back(3);
        quad->m_indices.push_back(2);

        quad->prepare();
    }

    getOrAddAssetByAlias<IMaterial>("default_material");
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

bool SGCore::AssetManager::isAssetExists(const std::string& alias, const std::filesystem::path& path,
                                         SGCore::AssetStorageType loadedBy, const size_t& assetTypeID) noexcept
{
    switch(loadedBy)
    {
        case AssetStorageType::BY_PATH:
        {
            return isAssetExists(Utils::toUTF8(path.u16string()), assetTypeID);
            break;
        }
        case AssetStorageType::BY_ALIAS:
        {
            return isAssetExists(alias, assetTypeID);
            break;
        }
    }

    return false;
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
    decltype(m_assets) loadedAssets;
    Serde::Serializer::fromFormat(FileUtils::readFile(markupFilePath), loadedAssets, Serde::FormatType::JSON, outputLog, m_package);

    assets_refs_container_t deserializedConflictingAssets;

    // finding conflicting assets and adding assets that are not exist in current asset manager;
    for(auto& variantsIt : loadedAssets)
    {
        const auto& pathOrAliasHash = variantsIt.first;
        auto& assetsByPathOrAlias = variantsIt.second;

        if(!m_assets.contains(pathOrAliasHash))
        {
            m_assets[pathOrAliasHash] = variantsIt.second;
            continue;
        }

        auto& alreadyLoadedAssetsByPathOrAlias = m_assets[pathOrAliasHash];

        for(auto& assetIt : assetsByPathOrAlias)
        {
            const auto& assetTypeID = assetIt.first;

            auto& asset = assetIt.second;

            // if asset 'asset' is already exists in current asset manager
            if(alreadyLoadedAssetsByPathOrAlias.contains(assetTypeID))
            {
                // add conflicting asset
                deserializedConflictingAssets[pathOrAliasHash][assetTypeID] = AssetRef<IAsset>(asset);
            }
            else
            {
                m_assets[pathOrAliasHash][assetTypeID] = asset;
            }
        }
    }

    if(!deserializedConflictingAssets.empty())
    {
        if(deserializedAssetsConflictsResolver)
        {
            // calling resolver of conflicts
            deserializedAssetsConflictsResolver(deserializedConflictingAssets, this);
        }
        else
        {
            LOG_W(SGCORE_TAG, "Resolver of conflicts for asset manager with name '{}' was not set! Conflicts were not resolved.", m_name);

            onAssetsReferencesResolve();
            onAssetsReferencesResolve.clear();
        }
    }
    else
    {
        onAssetsReferencesResolve();
        onAssetsReferencesResolve.clear();
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

void SGCore::AssetManager::resolveMemberAssetsReferences() noexcept
{
    for(auto& variantsIt : m_assets)
    {
        for(auto& assetIt : variantsIt.second)
        {
            assetIt.second->resolveMemberAssetsReferences(this);
        }
    }
}

const std::string& SGCore::AssetManager::getName() const noexcept
{
    return m_name;
}

SGCore::AssetRef<SGCore::IAsset>
SGCore::AssetManager::getAsset(const std::string& pathOrAlias, const size_t& assetTypeID) noexcept
{
    const size_t pathOrAliasHash = hashString(pathOrAlias);

    auto foundVariantsIt = m_assets.find(pathOrAliasHash);

    if(foundVariantsIt == m_assets.end())
    {
        return nullptr;
    }

    auto foundAssetIt = foundVariantsIt->second.find(assetTypeID);

    if(foundAssetIt == foundVariantsIt->second.end())
    {
        return nullptr;
    }

    return AssetRef<IAsset>(foundAssetIt->second);
}
