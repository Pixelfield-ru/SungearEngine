//
// Created by stuka on 07.05.2023.
//

#include <assimp/version.h>

#include "AssetManager.h"

#include "../Logging/Log.h"
#include "Assets/FileAsset.h"

void Core::Memory::AssetManager::init()
{
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "-----------------------------------");
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "Assimp info:");
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "Assimp version is %i.%i.%i.%i", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionPatch(), aiGetVersionRevision());
    Core::Logging::consolePrintf(Core::Logging::SG_INFO, "-----------------------------------");
}