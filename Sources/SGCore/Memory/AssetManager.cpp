//
// Created by stuka on 07.05.2023.
//

#include <spdlog/spdlog.h>

#include "AssetManager.h"
#include "Assets/TextFileAsset.h"

#include <assimp/version.h>

void SGCore::AssetManager::init() noexcept
{
    m_instance = MakeScope<AssetManager>();
}

SGCore::Ref<SGCore::registry_t> SGCore::AssetManager::getRegistry() noexcept
{
    return m_registry;
}

SGCore::Ref<SGCore::AssetManager>& SGCore::AssetManager::getInstance() noexcept
{
    return m_instance;
}

void SGCore::AssetManager::clear() noexcept
{
    std::lock_guard guard(m_mutex);
    
    m_registry->clear();
    m_entities.clear();
}

void SGCore::AssetManager::fullRemoveAsset(const std::filesystem::path& aliasOrPath) noexcept
{
    auto foundIt = m_entities.find(std::hash<std::filesystem::path>()(aliasOrPath));
    if(foundIt == m_entities.end()) return;

    const auto& e = foundIt->second;
    m_registry->destroy(e);

    m_entities.erase(foundIt);
}

void SGCore::AssetManager::createPackage(const std::filesystem::path& toPath, bool saveAssetsData) noexcept
{

}
