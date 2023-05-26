//
// Created by stuka on 07.05.2023.
//

#pragma once

#ifndef NATIVECORE_ASSETMANAGER_H
#define NATIVECORE_ASSETMANAGER_H

#include <iostream>
#include <map>
#include <memory>

#include "Assets/IAsset.h"
#include "Assets/FileAsset.h"
#include "Assets/Texture2DAsset.h"
#include "SGCore/Logging/Log.h"

namespace Core::Memory
{
    class AssetManager
    {
    private:
        static inline std::map<std::string, std::shared_ptr<Core::Memory::Assets::IAsset>> m_assets;

    public:
        AssetManager() = delete;

        static void init();

        template<typename AssetT>
        requires(std::is_base_of_v<Assets::IAsset, AssetT>)
        static std::shared_ptr<AssetT> loadAsset(const std::string& path)
        {
            auto foundAssetPair = m_assets.find(path);

            if(foundAssetPair != m_assets.end())
            {
                return std::static_pointer_cast<AssetT>(foundAssetPair->second);
            }

            std::shared_ptr<Assets::IAsset> newAsset = std::make_shared<AssetT>();

            newAsset->load(path);

            m_assets.emplace(path, newAsset);

            return std::static_pointer_cast<AssetT>(newAsset);
        }
    };
}

#endif //NATIVECORE_ASSETMANAGER_H
