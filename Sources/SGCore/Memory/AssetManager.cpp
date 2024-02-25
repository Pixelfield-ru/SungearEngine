//
// Created by stuka on 07.05.2023.
//

#include <spdlog/spdlog.h>

#include "AssetManager.h"
#include "Assets/FileAsset.h"

#include <assimp/version.h>

void SGCore::AssetManager::init()
{
    spdlog::info("-----------------------------------");

    spdlog::info("Assimp info:");
    spdlog::info("Assimp version is {0}.{1}.{2}.{3}",
             aiGetVersionMajor(),
             aiGetVersionMinor(),
             aiGetVersionPatch(),
             aiGetVersionRevision());

    spdlog::info("-----------------------------------");
}

entt::registry& SGCore::AssetManager::getRegistry() noexcept
{
    return m_registry;
}
