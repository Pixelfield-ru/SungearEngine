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
#include "entt/entity/registry.hpp"

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
            entt::entity entity;
            
            auto foundEntity = m_entities.find(path);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry.create();
                m_entities[path] = entity;
            }
            
            Ref<AssetT>* asset = m_registry.try_get<Ref<AssetT>>(entity);
            // asset found
            if(asset)
            {
                return *asset;
            }
            
            Ref<AssetT> newAsset = m_registry.emplace<Ref<AssetT>>(entity,
                                                                   AssetT::template createRefInstance<AssetT>());
            
            std::filesystem::path p(path);
            
            newAsset->load(path);
            newAsset->setRawName(p.stem().string());
            
            spdlog::info("Loaded new asset associated by path: {0}. Asset type: {1}", path, typeid(AssetT).name());
            
            return newAsset;
        }

        template<typename AssetT, typename... Args>
        requires(std::is_base_of_v<IAsset, AssetT>)
        static std::shared_ptr<AssetT> loadAssetWithAlias(const std::string& alias, const std::string& path)
        {
            entt::entity entity;
            
            auto foundEntity = m_entities.find(alias);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry.create();
                m_entities[alias] = entity;
            }
            
            Ref<AssetT>* asset = m_registry.try_get<Ref<AssetT>>(entity);
            // asset found
            if(asset)
            {
                return *asset;
            }
            
            Ref<AssetT> newAsset = m_registry.emplace<Ref<AssetT>>(entity,
                                                                   AssetT::template createRefInstance<AssetT>());
            
            std::filesystem::path p(path);
            
            newAsset->load(path);
            newAsset->setRawName(alias);
            
            spdlog::info("Loaded new asset associated by path: {0}. Asset type: {1}", path, typeid(AssetT).name());
            
            return newAsset;
        }
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        static void addAsset(const std::string& alias, const Ref<AssetT>& asset)
        {
            entt::entity entity;
            
            auto foundEntity = m_entities.find(alias);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry.create();
                m_entities[alias] = entity;
            }
            
            Ref<AssetT>* foundAsset = m_registry.try_get<Ref<AssetT>>(entity);
            // asset already exists
            if(foundAsset)
            {
                return;
            }
            
            m_registry.emplace<Ref<AssetT>>(entity, asset);
            asset->setRawName(alias);
            
            if(SG_INSTANCEOF(asset.get(), GPUObject))
            {
                std::dynamic_pointer_cast<GPUObject>(asset)->addToGlobalStorage();
            }
        }
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        static void addAsset(const Ref<AssetT>& asset)
        {
            const std::string& assetPath = asset->getPath().string();
            
            entt::entity entity;
            
            auto foundEntity = m_entities.find(assetPath);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry.create();
                m_entities[assetPath] = entity;
            }
            
            Ref<AssetT>* foundAsset = m_registry.try_get<Ref<AssetT>>(entity);
            // asset already exists
            if(foundAsset)
            {
                return;
            }
            
            m_registry.emplace<Ref<AssetT>>(entity, asset);
            
            if(SG_INSTANCEOF(asset.get(), GPUObject))
            {
                std::dynamic_pointer_cast<GPUObject>(asset)->addToGlobalStorage();
            }
        }
        
        static entt::registry& getRegistry() noexcept;

    private:
        static inline entt::registry m_registry;
        static inline std::unordered_map<std::string, entt::entity> m_entities;
    };
}

#endif // SUNGEARENGINE_ASSETMANAGER_H
