//
// Created by stuka on 07.05.2023.
//

#include <assimp/version.h>

#include "AssetManager.h"
#include "Assets/FileAsset.h"

void Core::Memory::AssetManager::init()
{
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "-----------------------------------");

    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "Assimp info:");
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "Assimp version is " +
    std::to_string(aiGetVersionMajor()) + "." +
    std::to_string(aiGetVersionMinor()) + "." +
    std::to_string(aiGetVersionPatch()) + "." +
    std::to_string(aiGetVersionRevision()));

    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "-----------------------------------");
}