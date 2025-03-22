//
// Created by ilya on 23.02.24.
//
#include "FontsManager.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Memory/Assets/Font.h"

#include <spdlog/spdlog.h>


SGCore::UI::FontsManager::FontsManager() noexcept
{
    m_FTLib = msdfgen::initializeFreetype();
    if(!m_FTLib)
    {
        LOG_E(SGCORE_TAG, "Could not init FreeType! Error code is hidden in msdf-atlas-gen :)!");
        return;
    }

    m_fontsAssetsManager = AssetManager::getAssetManager("fonts-manager");
}

SGCore::UI::FontsManager::~FontsManager() noexcept
{
    msdfgen::deinitializeFreetype(m_FTLib);
}

void SGCore::UI::FontsManager::init() const noexcept
{
    m_fontsAssetsManager->loadAssetWithAlias<Font>("times-new-roman", "${enginePath}/Resources/fonts/timesnewromanpsmt.ttf");
}

SGCore::UI::FontsManager& SGCore::UI::FontsManager::getInstance() noexcept
{
    static FontsManager instance;

    return instance;
}

msdfgen::FreetypeHandle* SGCore::UI::FontsManager::getFTLibrary() noexcept
{
    return m_FTLib;
}

SGCore::Ref<SGCore::AssetManager> SGCore::UI::FontsManager::getAssetManager() noexcept
{
    return m_fontsAssetsManager;
}
