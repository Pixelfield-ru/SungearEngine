//
// Created by ilya on 28.04.24.
//

#ifndef SUNGEARENGINEEDITOR_RESOURCES_H
#define SUNGEARENGINEEDITOR_RESOURCES_H

#include <vector>
#include <imgui.h>
#include "SGCore/Utils/Utils.h"
#include <SGCore/Memory/AssetManager.h>

namespace SGE
{
    struct Resources
    {
        static void loadStandardResources(SGCore::AssetManager& toManager = *SGCore::AssetManager::getInstance()) noexcept;
        static void unloadStandardResources(SGCore::AssetManager& fromManager = *SGCore::AssetManager::getInstance()) noexcept;

        SG_NOINLINE static SGCore::Ref<SGCore::AssetManager> getEditorOnlyAssetManager() noexcept;
        
    private:
        static inline SGCore::Ref<SGCore::AssetManager> m_editorOnlyAssetManager = SGCore::AssetManager::getAssetManager("SGEDITOR_INSTANCE");
    };
}

#endif //SUNGEARENGINEEDITOR_RESOURCES_H
