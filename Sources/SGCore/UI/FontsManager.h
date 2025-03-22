//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_FONTSMANAGER_H
#define SUNGEARENGINE_FONTSMANAGER_H

#include <SGCore/pch.h>

#include <sgcore_export.h>

#include <msdf-atlas-gen/msdf-atlas-gen.h>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class AssetManager;
}

namespace SGCore::UI
{
    struct SGCORE_EXPORT FontsManager
    {
        FontsManager(const FontsManager&) = delete;
        FontsManager(FontsManager&&) = delete;

        void init() const noexcept;

        static FontsManager& getInstance() noexcept;

        msdfgen::FreetypeHandle* getFTLibrary() noexcept;

        Ref<AssetManager> getAssetManager() noexcept;

        FontsManager& operator=(const FontsManager&) = delete;
        FontsManager& operator=(FontsManager&&) = delete;

    private:
        FontsManager() noexcept;
        ~FontsManager() noexcept;

        Ref<AssetManager> m_fontsAssetsManager;

        msdfgen::FreetypeHandle* m_FTLib = nullptr;
    };
}

#endif //SUNGEARENGINE_FONTSMANAGER_H
