//
// Created by stuka on 07.05.2023.
//

#include "AssetManager.h"
#include "Assets/FileAsset.h"

#include <assimp/version.h>

void Core::Memory::AssetManager::init()
{
    SGC_INFO("-----------------------------------");

    SGC_INFO("Assimp info:");
    SGC_INFO("Assimp version is " +
             std::to_string(aiGetVersionMajor()) + "." +
             std::to_string(aiGetVersionMinor()) + "." +
             std::to_string(aiGetVersionPatch()) + "." +
             std::to_string(aiGetVersionRevision()));

    SGC_INFO("-----------------------------------");
}