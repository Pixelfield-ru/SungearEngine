//
// Created by stuka on 07.05.2023.
//

#ifndef SUNGEARENGINE_ASSETMANAGER_H
#define SUNGEARENGINE_ASSETMANAGER_H

#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>
#include <sgcore_export.h>

#include "Assets/IAsset.h"
#include "SGUtils/Utils.h"
#include "SGCore/Graphics/GPUObject.h"
#include "entt/entity/registry.hpp"

namespace SGCore
{
    class SGCORE_EXPORT AssetManager
    {
    public:
        static void init() noexcept;

        /**
        * Adds asset with loading by path.
        * If asset already exists then return already loaded asset.
        * @tparam AssetT - Type of asset
        * @param path - Asset path
        * @return Added or already loaded asset
        */
        template<typename AssetT, typename... Args>
        requires(std::is_base_of_v<IAsset, AssetT>)
        std::shared_ptr<AssetT> loadAsset(const std::string& path)
        {
            entity_t entity;
            
            auto foundEntity = m_entities.find(path);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry->create();
                m_entities[path] = entity;
            }
            
            Ref<AssetT>* asset = m_registry->try_get<Ref<AssetT>>(entity);
            // asset found
            if(asset)
            {
                return *asset;
            }
            
            Ref<AssetT> newAsset = m_registry->emplace<Ref<AssetT>>(entity,
                                                                    AssetT::template createRefInstance<AssetT>());
            
            std::filesystem::path p(path);
            
            newAsset->load(path);
            newAsset->setRawName(p.stem().string());
            
            spdlog::info("Loaded new asset associated by path: {0}. Asset type: {1}", path, typeid(AssetT).name());
            
            return newAsset;
        }

        template<typename AssetT, typename... Args>
        requires(std::is_base_of_v<IAsset, AssetT>)
        std::shared_ptr<AssetT> loadAsset(const std::string& alias, const std::string& path)
        {
            entity_t entity;
            
            auto foundEntity = m_entities.find(alias);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry->create();
                m_entities[alias] = entity;
            }
            
            Ref<AssetT>* asset = m_registry->try_get<Ref<AssetT>>(entity);
            // asset found
            if(asset)
            {
                return *asset;
            }
            
            Ref<AssetT> newAsset = m_registry->emplace<Ref<AssetT>>(entity,
                                                                    AssetT::template createRefInstance<AssetT>());
            
            std::filesystem::path p(path);
            
            newAsset->load(path);
            newAsset->setRawName(alias);
            
            spdlog::info("Loaded new asset associated by path: {0}. Asset type: {1}", path, typeid(AssetT).name());
            
            return newAsset;
        }
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        void addAsset(const std::string& alias, const Ref<AssetT>& asset)
        {
            entity_t entity;
            
            auto foundEntity = m_entities.find(alias);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry->create();
                m_entities[alias] = entity;
            }
            
            Ref<AssetT>* foundAsset = m_registry->try_get<Ref<AssetT>>(entity);
            // asset already exists
            if(foundAsset)
            {
                return;
            }
            
            m_registry->emplace<Ref<AssetT>>(entity, asset);
            asset->setRawName(alias);
            
            if(SG_INSTANCEOF(asset.get(), GPUObject))
            {
                std::dynamic_pointer_cast<GPUObject>(asset)->addToGlobalStorage();
            }
        }
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        void addAsset(const Ref<AssetT>& asset)
        {
            const std::string& assetPath = asset->getPath().string();
            
            entity_t entity;
            
            auto foundEntity = m_entities.find(assetPath);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry->create();
                m_entities[assetPath] = entity;
            }
            
            Ref<AssetT>* foundAsset = m_registry->try_get<Ref<AssetT>>(entity);
            // asset already exists
            if(foundAsset)
            {
                return;
            }
            
            m_registry->emplace<Ref<AssetT>>(entity, asset);
            
            if(SG_INSTANCEOF(asset.get(), GPUObject))
            {
                std::dynamic_pointer_cast<GPUObject>(asset)->addToGlobalStorage();
            }
        }
        
        bool isAssetsEntityExists(const std::string& pathOrAlias) noexcept
        {
            auto foundEntity = m_entities.find(pathOrAlias);
            
            return foundEntity != m_entities.end();
        }
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        bool isAssetExists(const std::string& pathOrAlias) noexcept
        {
            auto foundEntity = m_entities.find(pathOrAlias);
            if(foundEntity == m_entities.end())
            {
                return false;
            }
            else
            {
                return m_registry->try_get<Ref<AssetT>>(foundEntity->second) != nullptr;
            }
        }
        
        Ref<registry_t> getRegistry() noexcept;
        
        SG_NOINLINE static Scope<AssetManager>& getInstance() noexcept;

    private:
        Ref<registry_t> m_registry = MakeRef<registry_t>();
        std::unordered_map<std::string, entity_t> m_entities;
        
        static inline Scope<AssetManager> m_instance;
    };
}

#endif // SUNGEARENGINE_ASSETMANAGER_H
