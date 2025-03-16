//
// Created by ilya on 23.02.24.
//
#include "FontsManager.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Memory/Assets/Font.h"

#include <spdlog/spdlog.h>


void SGCore::UI::FontsManager::init() noexcept
{
    int errCode = FT_Init_FreeType(&m_FTLib);
    if(errCode)
    {
        LOG_E(SGCORE_TAG, "Could not init FreeType! FreeType error code is: {}", errCode);
        return;
    }

    m_fontsAssetsManager = AssetManager::getAssetManager("fonts-manager");

    m_fontsAssetsManager->loadAssetWithAlias<Font>("times-new-roman", "${enginePath}/Resources/fonts/timesnewromanpsmt.ttf");
}

FT_Library SGCore::UI::FontsManager::getFTLibrary() noexcept
{
    return m_FTLib;
}

SGCore::Ref<SGCore::AssetManager> SGCore::UI::FontsManager::getAssetManager() noexcept
{
    return m_fontsAssetsManager;
}

