//
// Created by stuka on 07.05.2023.
//

#include <spdlog/spdlog.h>

#include "AssetManager.h"
#include "Assets/TextFileAsset.h"
#include "AssetsPackage.h"
#include "SGCore/Serde/Serde.h"
#include "SGCore/Serde/StandardSerdeSpecs/SerdeSpecs.h"

#include <assimp/version.h>

void SGCore::AssetManager::init() noexcept
{
    m_instance = getAssetManager("MainAssetManager");
    m_instance->addStandardAssets();
}

void SGCore::AssetManager::addStandardAssets() noexcept
{
    /*if(!isAssetExists<IMeshData>("quad_mesh"))
    {
        auto quad = getOrAddAssetByAlias<IMeshData>("quad_mesh");

        quad->m_indices.push_back(0);
        quad->m_indices.push_back(2);
        quad->m_indices.push_back(1);

        quad->m_indices.push_back(0);
        quad->m_indices.push_back(3);
        quad->m_indices.push_back(2);

        quad->prepare();
    }*/

    {

        // Serde::Serializer::fromFormat(FileUtils::readFile(InterpolatedPath("${enginePath}/Resources/materials/no_material.sgmat").resolved()), );
        auto standardMaterial = loadAsset<IMaterial>("${enginePath}/Resources/materials/no_material.sgmat");
        std::cout << "standard material: " << standardMaterial.get() << std::endl;
        // standardMaterial->addTexture2D(SGTextureSlot::SGTT_DIFFUSE, loadAsset<ITexture2D>("${enginePath}/Resources/textures/no_material.png"));
    }
}

void SGCore::AssetManager::clear() noexcept
{
    std::lock_guard guard(m_mutex);

    m_assets.clear();
}

bool SGCore::AssetManager::isAssetExists(const std::string& alias, const InterpolatedPath& path,
                                         AssetStorageType loadedBy, const size_t& assetTypeID) noexcept
{
    switch(loadedBy)
    {
        case AssetStorageType::BY_PATH:
        {
            return isAssetExists<AssetStorageType::BY_PATH>(path, assetTypeID);
            break;
        }
        case AssetStorageType::BY_ALIAS:
        {
            return isAssetExists<AssetStorageType::BY_ALIAS>(alias, assetTypeID);
            break;
        }
    }

    return false;
}

void SGCore::AssetManager::createPackage(const InterpolatedPath& toDirectory,
                                         const std::string& packageName) noexcept
{
    const std::filesystem::path binaryFilePath = (toDirectory / (packageName + ".bin")).resolved();
    const std::filesystem::path markupFilePath = (toDirectory / (packageName + ".json")).resolved();

    m_package.m_buffer.clear();

    m_package.m_path = binaryFilePath;
    m_package.m_parentAssetManager = this;

    const std::string writtenJSON = Serde::Serializer::toFormat(Serde::FormatType::JSON, m_assets, m_package);

    // writing markup (json) file
    FileUtils::writeToFile(markupFilePath, writtenJSON, false, true);
    // writing binary file
    FileUtils::writeBytes(binaryFilePath, 0, m_package.m_buffer, false);
}

void SGCore::AssetManager::loadPackage(const InterpolatedPath& fromDirectory,
                                       const std::string& packageName) noexcept
{
    const std::filesystem::path binaryFilePath = (fromDirectory / (packageName + ".bin")).resolved();
    const std::filesystem::path markupFilePath = (fromDirectory / (packageName + ".json")).resolved();

    m_package.m_path = binaryFilePath;
    m_package.m_parentAssetManager = this;

    std::string outputLog;
    decltype(m_assets) loadedAssets;
    Serde::Serializer::fromFormat(FileUtils::readFile(markupFilePath), loadedAssets, Serde::FormatType::JSON, outputLog, m_package);

    assets_refs_container_t deserializedConflictingAssets;

    // finding conflicting assets and adding assets that are not exist in current asset manager;
    for(auto& variantsIt : loadedAssets)
    {
        auto pathOrAliasHash = variantsIt.first;
        auto& assetsByPathOrAlias = variantsIt.second;

        if(!m_assets.contains(pathOrAliasHash))
        {
            m_assets[pathOrAliasHash] = assetsByPathOrAlias;
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

            resolveMemberAssetsReferences();
        }
    }
    else
    {
        resolveMemberAssetsReferences();
    }

    if(!outputLog.empty())
    {
        LOG_E(SGCORE_TAG, "Error while loading package with assets (directory of package: '{}', name of package '{}'): {}",
              Utils::toUTF8(fromDirectory.resolved().u16string()), packageName, outputLog);
    }
}

const SGCore::AssetsPackage& SGCore::AssetManager::getPackage() const noexcept
{
    return m_package;
}

void SGCore::AssetManager::resolveMemberAssetsReferences() noexcept
{
    getOnMemberAssetsReferencesResolveEvent()(this);
}

const std::string& SGCore::AssetManager::getName() const noexcept
{
    return m_name;
}

SGCore::Ref<SGCore::AssetManager> SGCore::AssetManager::getInstance() noexcept
{
    return m_instance;
}

void SGCore::AssetManager::reloadAssetFromDisk(SGCore::IAsset* asset, SGCore::AssetsLoadPolicy loadPolicy,
                                               SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{
    switch(loadPolicy)
    {
        case AssetsLoadPolicy::SINGLE_THREADED:
        {
            asset->doReloadFromDisk(loadPolicy, lazyLoadInThread);
            asset->lazyLoad();
            break;
        }
        case AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD:
        {
            auto loadInThread = m_threadsPool.getThread();
            // loadInThread->m_autoJoinIfNotBusy = true;
            auto loadAssetTask = MakeRef<Threading::Task>();

            loadAssetTask->setOnExecuteCallback([this, asset, loadPolicy, lazyLoadInThread]() {
                asset->doReloadFromDisk(loadPolicy, lazyLoadInThread);
            });

            if(lazyLoadInThread)
            {
                loadAssetTask->setOnExecutedCallback([asset]() {
                    asset->lazyLoad();
                }, lazyLoadInThread);
            }

            loadInThread->addTask(loadAssetTask);
            loadInThread->start();

            break;
        }
        case AssetsLoadPolicy::PARALLEL_NO_LAZYLOAD:
        {
            auto loadInThread = m_threadsPool.getThread();
            // loadInThread->m_autoJoinIfNotBusy = true;
            auto loadAssetTask = MakeRef<Threading::Task>();

            loadAssetTask->setOnExecuteCallback([this, asset, loadPolicy, lazyLoadInThread]() {
                asset->doReloadFromDisk(loadPolicy, lazyLoadInThread);
            });

            loadInThread->addTask(loadAssetTask);
            loadInThread->start();

            break;
        }
    }
}

bool SGCore::AssetManager::checkForAssetExistenceInFilesystem(const int64_t& assetTypeID,
                                                              const SGCore::InterpolatedPath& assetPath) noexcept
{
    if(!std::filesystem::exists(assetPath.resolved()))
    {
        LOG_E(SGCORE_TAG,
              "Can not load asset (typeID: '{}'): this path does not exist.\nAsset path: '{}'",
              assetTypeID,
              Utils::toUTF8(assetPath.resolved().u16string()));

        return false;
    }

    return true;
}
