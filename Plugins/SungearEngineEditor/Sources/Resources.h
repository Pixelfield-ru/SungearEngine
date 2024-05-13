//
// Created by ilya on 28.04.24.
//

#ifndef SUNGEARENGINEEDITOR_RESOURCES_H
#define SUNGEARENGINEEDITOR_RESOURCES_H

#include <vector>
#include <imgui.h>
#include <SGUtils/Utils.h>
#include <SGCore/Memory/AssetManager.h>

namespace SGE
{
    struct Resources
    {
        SG_NOINLINE static SGCore::AssetManager& getMainAssetManager() noexcept;
        
    private:
        static inline SGCore::AssetManager m_mainAssetManager;
    };
}

#endif //SUNGEARENGINEEDITOR_RESOURCES_H
