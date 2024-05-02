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

SGCore::Scope<SGCore::AssetManager>& SGCore::AssetManager::getInstance() noexcept
{
    return m_instance;
}
