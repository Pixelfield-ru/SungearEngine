//
// Created by stuka on 07.05.2023.
//

#include <spdlog/spdlog.h>

#include "AssetManager.h"
#include "Assets/TextFileAsset.h"
#include "AssetsPackage.h"

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

void SGCore::AssetManager::createPackage(const std::filesystem::path& toPath, bool saveAssetsData) noexcept
{
    // structure of package:
    // sections for all assets:
    //
    // size:<size of full asset section>
    // path:<path to asset>
    // alias:<alias of asset>
    // typeID:<ID of type that must be provided in every derived type of IAsset (see IAsset struct)>
    //
    // next you can put custom data

    AssetsPackage outPackage;
    outPackage.m_path = toPath;

    for(auto& p0 : m_assets)
    {
        for(auto& p1 : p0.second)
        {
            AssetsPackage::AssetSection assetSection(&outPackage);
            p1.second->serializeToPackage(assetSection, saveAssetsData);
        }
    }

    FileUtils::writeBytes(toPath, 0, outPackage.m_buffer, false);
}
