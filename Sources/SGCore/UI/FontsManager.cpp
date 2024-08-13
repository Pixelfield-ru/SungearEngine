//
// Created by ilya on 23.02.24.
//
#include "FontsManager.h"

#include <spdlog/spdlog.h>

void SGCore::FontsManager::init() noexcept
{
    int errCode = FT_Init_FreeType(&m_FTLib);
    if(errCode)
    {
        spdlog::error("Could not init FreeType! FreeType error code is: {0}", errCode);
        return;
    }
}

FT_Library SGCore::FontsManager::getFTLibrary() noexcept
{
    return m_FTLib;
}

