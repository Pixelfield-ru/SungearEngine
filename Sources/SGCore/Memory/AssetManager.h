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

        /**
        * Adds asset with loading by path.
        * If asset already exists then return already loaded asset.
        * @tparam AssetT - Type of asset
        * @param path - Asset path
        * @return Added or already loaded asset
        */
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

            newAsset = newAsset->load(path);

            m_assets.emplace(path, newAsset);

            return std::static_pointer_cast<AssetT>(newAsset);
        }

        /**
         * Creates asset without loading by path.
         * If asset already exists then return found asset.
         * @tparam AssetT - Type of asset
         * @param path - Asset pseudonym
         * @return Created or found asset
         */
        template<typename AssetT>
        requires(std::is_base_of_v<Assets::IAsset, AssetT>)
        static std::shared_ptr<AssetT> createAsset(const std::string& path)
        {
            auto foundAssetPair = m_assets.find(path);

            if(foundAssetPair != m_assets.end())
            {
                return std::static_pointer_cast<AssetT>(foundAssetPair->second);
            }

            std::shared_ptr<Assets::IAsset> newAsset = std::make_shared<AssetT>();

            m_assets.emplace(path, newAsset);

            return std::static_pointer_cast<AssetT>(newAsset);
        }
    };
}

#endif //NATIVECORE_ASSETMANAGER_H
