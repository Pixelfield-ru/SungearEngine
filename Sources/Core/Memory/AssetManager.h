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
#include "../Exceptions/SGNotImplementedException.h"

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

            if(std::is_same_v<AssetT, Memory::Assets::FileAsset>)
            {
                if(foundAssetPair != m_assets.end())
                {
                    return std::static_pointer_cast<AssetT>(foundAssetPair->second);
                }

                std::shared_ptr<Assets::FileAsset> newFileAsset = std::make_shared<Assets::FileAsset>();

                newFileAsset->load(path);

                m_assets.emplace(path, newFileAsset);

                return std::static_pointer_cast<AssetT>(newFileAsset);
            }
            else
            {
                throw Core::Exceptions::SGNotImplementedException("LOAD_ASSET: loading of this type of asset is not implemented.");
            }
        }
    };
}

#endif //NATIVECORE_ASSETMANAGER_H
