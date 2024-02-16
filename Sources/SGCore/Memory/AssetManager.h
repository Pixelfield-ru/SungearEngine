//
// Created by stuka on 07.05.2023.
//

#ifndef SUNGEARENGINE_ASSETMANAGER_H
#define SUNGEARENGINE_ASSETMANAGER_H

#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>

#include "Assets/IAsset.h"
#include "SGUtils/Utils.h"
#include "SGCore/Graphics/GPUObject.h"

namespace SGCore
{
    class AssetManager
    {
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
        template<typename AssetT, typename... Args>
        requires(std::is_base_of_v<IAsset, AssetT>)
        static std::shared_ptr<AssetT> loadAsset(const std::string& path)
        {
            auto foundAssetsPair = m_assets.find(path);

            if(foundAssetsPair != m_assets.end())
            {
                for(auto& a : foundAssetsPair->second)
                {
                    if(typeid(*a) == typeid(AssetT))
                    {
                        return std::static_pointer_cast<AssetT>(a);
                    }
                }
            }
            
            std::filesystem::path p(path);

            // sfinae just for GPU objects as assets
            Ref<AssetT> newAsset = AssetT::template createRefInstance<AssetT>();

            newAsset->load(path);
            newAsset->setRawName(p.stem().string());
            
            m_assets[path].push_back(newAsset);
            
            spdlog::info("Loaded new asset associated by path: {0}. Asset type: {1}", path, typeid(AssetT).name());

            return newAsset;
        }

        template<typename AssetT, typename... Args>
        requires(std::is_base_of_v<IAsset, AssetT>)
        static std::shared_ptr<AssetT> loadAssetWithAlias(const std::string& alias, const std::string& path)
        {
            auto foundAssetsPair = m_assets.find(path);
            
            if(foundAssetsPair != m_assets.end())
            {
                for(auto& a : foundAssetsPair->second)
                {
                    if(typeid(*a) == typeid(AssetT))
                    {
                        return std::static_pointer_cast<AssetT>(a);
                    }
                }
            }
            
            // sfinae just for GPU objects as assets
            Ref<AssetT> newAsset = AssetT::template createRefInstance<AssetT>();

            newAsset->load(path);
            newAsset->setRawName(alias);

            m_assets[alias].push_back(newAsset);
            
            spdlog::info("Loaded new asset: {0}", path);

            return newAsset;
        }

        static void addAsset(const std::string& alias, const Ref<IAsset>& asset)
        {
            auto foundAssetsPair = m_assets.find(alias);
            
            if(foundAssetsPair != m_assets.end())
            {
                for(auto& a : foundAssetsPair->second)
                {
                    if(typeid(*a) == typeid(*asset))
                    {
                        return;
                    }
                }
            }
            
            m_assets[alias].push_back(asset);
            asset->setRawName(alias);
            
            if(SG_INSTANCEOF(asset.get(), GPUObject))
            {
                std::dynamic_pointer_cast<GPUObject>(asset)->addToGlobalStorage();
            }
        }

        static void addAsset(const Ref<IAsset>& asset)
        {
            const std::string& assetPath = asset->getPath().string();
            
            auto foundAssetsPair = m_assets.find(asset->getPath().string());
            
            if(foundAssetsPair != m_assets.end())
            {
                for(auto& a : foundAssetsPair->second)
                {
                    if(typeid(*a) == typeid(*asset))
                    {
                        return;
                    }
                }
            }
            
            m_assets[assetPath].push_back(asset);
            
            if(SG_INSTANCEOF(asset.get(), GPUObject))
            {
                std::dynamic_pointer_cast<GPUObject>(asset)->addToGlobalStorage();
            }
        }

        /**
         * Creates asset without loading by path.
         * If asset already exists then return found asset.
         * @tparam AssetT - Type of asset
         * @param path - Asset pseudonym
         * @return Created or found asset
         */
        template<typename AssetT, typename... Args>
        requires(std::is_base_of_v<IAsset, AssetT>)
        static std::shared_ptr<AssetT> createAsset(const std::string& path, const Args&... args)
        {
            auto foundAssetPair = m_assets.find(path);

            if(foundAssetPair != m_assets.end())
            {
                return std::static_pointer_cast<AssetT>(foundAssetPair->second);
            }

            std::shared_ptr<AssetT> newAsset = std::make_shared<AssetT>(args...);

            m_assets.emplace(path, newAsset);

            return newAsset;
        }

    private:
        // todo: replace unordered map by vector because of first (std::string)
        static inline std::unordered_map<std::string, std::vector<std::shared_ptr<IAsset>>> m_assets;
    };
}

#endif // SUNGEARENGINE_ASSETMANAGER_H
