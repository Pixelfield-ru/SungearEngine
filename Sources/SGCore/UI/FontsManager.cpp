//
// Created by ilya on 23.02.24.
//
#include "FontsManager.h"

#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>

void SGCore::FontsManager::init() noexcept
{
    int errCode = FT_Init_FreeType(&m_FTLib);
    if(errCode)
    {
        LOG_E("Could not init FreeType! FreeType error code is: {}", errCode);
        return;
    }
}

FT_Library SGCore::FontsManager::getFTLibrary() noexcept
{
    return m_FTLib;
}

