//
// Created by stuka on 07.05.2023.
//

#ifndef SUNGEARENGINE_ASSETMANAGER_H
#define SUNGEARENGINE_ASSETMANAGER_H

#include <SGCore/pch.h>

#include <sgcore_export.h>

#include "Assets/IAsset.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Graphics/GPUObject.h"
#include "SGCore/Threading/ThreadsPool.h"
#include "SGCore/Threading/ThreadsManager.h"
#include "SGCore/Logger/Logger.h"

namespace SGCore
{
    enum AssetsLoadPolicy
    {
        SINGLE_THREADED,
        PARALLEL_THEN_LAZYLOAD,
        PARALLEL_NO_LAZYLOAD
    };
    
    class SGCORE_EXPORT AssetManager
    {
    public:
        struct Package
        {
            std::filesystem::path m_path;
        };

        AssetsLoadPolicy m_defaultAssetsLoadPolicy = AssetsLoadPolicy::SINGLE_THREADED;
        
        static void init() noexcept;
        
        // =================================================================================================
        // =================================================================================================
        // =================================================================================================
        
        template<typename AssetT, typename... AssetCtorArgs>
        requires(std::is_base_of_v<IAsset, AssetT>)
        std::shared_ptr<AssetT> loadAsset(AssetsLoadPolicy assetsLoadPolicy,
                                          const Ref<Threading::Thread>& lazyLoadInThread,
                                          const std::filesystem::path& path,
                                          AssetCtorArgs&&... assetCtorArgs)
        {
            std::lock_guard guard(m_mutex);
            
            entity_t entity;

            const size_t hashedAssetPath = std::hash<std::filesystem::path>()(path);
            
            auto foundEntity = m_entities.find(hashedAssetPath);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry->create();
                m_entities[hashedAssetPath] = entity;
            }
            
            Ref<AssetT>* asset = m_registry->try_get<Ref<AssetT>>(entity);
            // asset found
            if(asset)
            {
                return *asset;
            }
            
            Ref<AssetT> newAsset = m_registry->emplace<Ref<AssetT>>(entity,
                                                                    AssetT::template createRefInstance<AssetT>(std::forward<AssetCtorArgs>(assetCtorArgs)...));
            
            std::filesystem::path p(path);
            
            distributeAsset(newAsset, path, assetsLoadPolicy, lazyLoadInThread);
            
            newAsset->setRawName(p.stem().string());

            LOG_I(SGCORE_TAG, "Loaded new asset associated by path: {}. Asset type: {}", Utils::toUTF8(path.u16string()), typeid(AssetT).name());
            
            return newAsset;
        }
        
        template<typename AssetT, typename... AssetCtorArgs>
        requires(std::is_base_of_v<IAsset, AssetT>)
        std::shared_ptr<AssetT> loadAsset(AssetsLoadPolicy assetsLoadPolicy,
                                          const std::filesystem::path& path,
                                          AssetCtorArgs&&... assetCtorArgs)
        {
            return loadAsset<AssetT, AssetCtorArgs...>(assetsLoadPolicy, Threading::ThreadsManager::getMainThread(), path, std::forward<AssetCtorArgs>(assetCtorArgs)...);
        }
        
        template<typename AssetT, typename... AssetCtorArgs>
        requires(std::is_base_of_v<IAsset, AssetT>)
        std::shared_ptr<AssetT> loadAsset(const std::filesystem::path& path,
                                          AssetCtorArgs&&... assetCtorArgs)
        {
            return loadAsset<AssetT, AssetCtorArgs...>(m_defaultAssetsLoadPolicy, Threading::ThreadsManager::getMainThread(), path, std::forward<AssetCtorArgs>(assetCtorArgs)...);
        }
        
        // =================================================================================================
        // =================================================================================================
        // =================================================================================================
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        void loadAsset(Ref<AssetT>& assetToLoad,
                       AssetsLoadPolicy assetsLoadPolicy,
                       const Ref<Threading::Thread>& lazyLoadInThread,
                       const std::filesystem::path& path)
        {
            std::lock_guard guard(m_mutex);
            
            entity_t entity;

            const size_t hashedAssetPath = std::hash<std::filesystem::path>()(path);
            
            auto foundEntity = m_entities.find(hashedAssetPath);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry->create();
                m_entities[hashedAssetPath] = entity;
            }
            
            Ref<AssetT>* foundAsset = m_registry->try_get<Ref<AssetT>>(entity);
            // asset found
            if(foundAsset)
            {
                assetToLoad = *foundAsset;
                return;
            }
            
            m_registry->emplace<Ref<AssetT>>(entity, assetToLoad);
            
            std::filesystem::path p(path);
            
            distributeAsset(assetToLoad, path, assetsLoadPolicy, lazyLoadInThread);
            
            assetToLoad->setRawName(p.stem().string());

            LOG_I(SGCORE_TAG, "Loaded new asset associated by path: {}. Asset type: {}", Utils::toUTF8(path.u16string()), typeid(AssetT).name());
        }
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        void loadAsset(Ref<AssetT>& assetToLoad,
                       AssetsLoadPolicy assetsLoadPolicy,
                       const std::filesystem::path& path)
        {
            loadAsset<AssetT>(assetToLoad, assetsLoadPolicy, Threading::ThreadsManager::getMainThread(), path);
        }
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        void loadAsset(Ref<AssetT>& assetToLoad,
                       const std::filesystem::path& path)
        {
            loadAsset<AssetT>(assetToLoad, m_defaultAssetsLoadPolicy, Threading::ThreadsManager::getMainThread(), path);
        }
        
        // =================================================================================================
        // =================================================================================================
        // =================================================================================================
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        void loadAssetWithAlias(Ref<AssetT>& assetToLoad,
                                AssetsLoadPolicy assetsLoadPolicy,
                                const Ref<Threading::Thread>& lazyLoadInThread,
                                const std::string& alias,
                                const std::filesystem::path& path)
        {
            std::lock_guard guard(m_mutex);
            
            entity_t entity;

            const size_t hashedAssetAlias = std::hash<std::string>()(alias);
            
            auto foundEntity = m_entities.find(hashedAssetAlias);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry->create();
                m_entities[hashedAssetAlias] = entity;
            }
            
            Ref<AssetT>* foundAsset = m_registry->try_get<Ref<AssetT>>(entity);
            // asset found
            if(foundAsset)
            {
                assetToLoad = *foundAsset;
                return;
            }
            
            m_registry->emplace<Ref<AssetT>>(entity, assetToLoad);
            
            distributeAsset(assetToLoad, path, assetsLoadPolicy, lazyLoadInThread);
            
            assetToLoad->setRawName(alias);

            LOG_I(SGCORE_TAG, "Loaded new asset associated by path: {}. Asset type: {}", Utils::toUTF8(path.u16string()), typeid(AssetT).name());
        }
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        void loadAssetWithAlias(Ref<AssetT>& assetToLoad,
                                AssetsLoadPolicy assetsLoadPolicy,
                                const std::string& alias,
                                const std::filesystem::path& path)
        {
            loadAssetWithAlias<AssetT>(assetToLoad, assetsLoadPolicy, Threading::ThreadsManager::getMainThread(), alias, path);
        }
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        void loadAssetWithAlias(Ref<AssetT>& assetToLoad,
                                const std::string& alias,
                                const std::filesystem::path& path)
        {
            loadAssetWithAlias<AssetT>(assetToLoad, m_defaultAssetsLoadPolicy, Threading::ThreadsManager::getMainThread(), alias, path);
        }
        
        // =================================================================================================
        // =================================================================================================
        // =================================================================================================
        
        template<typename AssetT, typename... AssetCtorArgs>
        requires(std::is_base_of_v<IAsset, AssetT>)
        std::shared_ptr<AssetT> loadAssetWithAlias(AssetsLoadPolicy assetsLoadPolicy,
                                                   const Ref<Threading::Thread>& lazyLoadInThread,
                                                   const std::string& alias,
                                                   const std::filesystem::path& path,
                                                   AssetCtorArgs&&... assetCtorArgs)
        {
            std::lock_guard guard(m_mutex);
            
            entity_t entity;

            const size_t hashedAssetAlias = std::hash<std::string>()(alias);
            
            auto foundEntity = m_entities.find(hashedAssetAlias);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry->create();
                m_entities[hashedAssetAlias] = entity;
            }
            
            Ref<AssetT>* asset = m_registry->try_get<Ref<AssetT>>(entity);
            // asset found
            if(asset)
            {
                return *asset;
            }
            
            Ref<AssetT> newAsset = m_registry->emplace<Ref<AssetT>>(entity,
                                                                    AssetT::template createRefInstance<AssetT>(std::forward<AssetCtorArgs>(assetCtorArgs)...));
            
            distributeAsset(newAsset, path, assetsLoadPolicy, lazyLoadInThread);
            
            newAsset->setRawName(alias);

            LOG_I(SGCORE_TAG, "Loaded new asset associated by path: {}. Asset type: {}", Utils::toUTF8(path.u16string()), typeid(AssetT).name());
            
            return newAsset;
        }
        
        template<typename AssetT, typename... AssetCtorArgs>
        requires(std::is_base_of_v<IAsset, AssetT>)
        std::shared_ptr<AssetT> loadAssetWithAlias(AssetsLoadPolicy assetsLoadPolicy,
                                                   const std::string& alias,
                                                   const std::filesystem::path& path,
                                                   AssetCtorArgs&&... assetCtorArgs)
        {
            return loadAssetWithAlias<AssetT, AssetCtorArgs...>(assetsLoadPolicy, Threading::ThreadsManager::getMainThread(), alias, path, std::forward<AssetCtorArgs>(assetCtorArgs)...);
        }
        
        template<typename AssetT, typename... AssetCtorArgs>
        requires(std::is_base_of_v<IAsset, AssetT>)
        std::shared_ptr<AssetT> loadAssetWithAlias(const std::string& alias,
                                                   const std::filesystem::path& path,
                                                   AssetCtorArgs&&... assetCtorArgs)
        {
            return loadAssetWithAlias<AssetT, AssetCtorArgs...>(m_defaultAssetsLoadPolicy, Threading::ThreadsManager::getMainThread(), alias, path, std::forward<AssetCtorArgs>(assetCtorArgs)...);
        }
        
        // =================================================================================================
        // =================================================================================================
        // =================================================================================================
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        void addAsset(const std::string& alias, const Ref<AssetT>& asset)
        {
            std::lock_guard guard(m_mutex);
            
            entity_t entity;

            const size_t hashedAssetAlias = std::hash<std::string>()(alias);

            auto foundEntity = m_entities.find(hashedAssetAlias);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry->create();
                m_entities[hashedAssetAlias] = entity;
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

            LOG_I(SGCORE_TAG, "Added new asset with alias '{}', path '{}' and type '{}'", alias, Utils::toUTF8(asset->getPath().u16string()), typeid(AssetT).name())
        }
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        void addAsset(const Ref<AssetT>& asset)
        {
            std::lock_guard guard(m_mutex);
            
            const std::string& assetPath = asset->getPath().string();

            const size_t hashedAssetPath = std::hash<std::string>()(assetPath);
            
            entity_t entity;
            
            auto foundEntity = m_entities.find(hashedAssetPath);
            if(foundEntity != m_entities.end())
            {
                entity = foundEntity->second;
            }
            else
            {
                entity = m_registry->create();
                m_entities[hashedAssetPath] = entity;
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

            LOG_I(SGCORE_TAG, "Added new asset with alias '{}', path '{}' and type '{}'", asset->m_name, Utils::toUTF8(asset->getPath().u16string()), typeid(AssetT).name())
        }
        
        bool isAssetsEntityExists(const std::string& pathOrAlias) noexcept
        {
            auto foundEntity = m_entities.find(std::hash<std::string>()(pathOrAlias));
            
            return foundEntity != m_entities.end();
        }
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        bool isAssetExists(const std::string& pathOrAlias) noexcept
        {
            std::lock_guard guard(m_mutex);
            
            auto foundEntity = m_entities.find(std::hash<std::string>()(pathOrAlias));
            if(foundEntity == m_entities.end())
            {
                return false;
            }
            else
            {
                return m_registry->try_get<Ref<AssetT>>(foundEntity->second) != nullptr;
            }
        }

        /**
         * Completely deletes the asset (deletes all copies of the asset that were loaded as different types).
         * @param aliasOrPath
         */
        void fullRemoveAsset(const std::filesystem::path& aliasOrPath) noexcept;

        /**
         * Deletes only one copy of an asset that was loaded as type AssetT.
         * @tparam AssetT
         * @param aliasOrPath
         */
        template<typename AssetT>
        void removeAssetLoadedByType(const std::filesystem::path& aliasOrPath) noexcept
        {
            auto foundIt = m_entities.find(std::hash<std::filesystem::path>()(aliasOrPath));
            if(foundIt == m_entities.end()) return;

            const auto& e = foundIt->second;
            m_registry->remove<SGCore::Ref<AssetT>>(e);

            m_entities.erase(foundIt);
        }

        void clear() noexcept;
        
        Ref<registry_t> getRegistry() noexcept;
        
        SG_NOINLINE static Ref<AssetManager>& getInstance() noexcept;

        void createPackage(const std::filesystem::path& toPath, bool saveAssetsData = true) noexcept;

    private:
        void distributeAsset(const Ref<IAsset>& asset,
                             const std::filesystem::path& path,
                             AssetsLoadPolicy loadPolicy,
                             const Ref<Threading::Thread>& lazyLoadInThread) noexcept
        {
            switch(loadPolicy)
            {
                case SINGLE_THREADED:
                {
                    asset->load(path);
                    asset->lazyLoad();
                    break;
                }
                case PARALLEL_THEN_LAZYLOAD:
                {
                    auto loadInThread = m_threadsPool.getThread();
                    // loadInThread->m_autoJoinIfNotBusy = true;
                    auto loadAssetTask = loadInThread->createTask();
        
                    loadAssetTask->setOnExecuteCallback([asset, path]() {
                        asset->load(path);
                    });
                    
                    if(lazyLoadInThread)
                    {
                        loadAssetTask->setOnExecutedCallback([asset]() {
                            asset->lazyLoad();
                        }, lazyLoadInThread);
                    }
                    
                    loadInThread->addTask(loadAssetTask);
                    loadInThread->start();
                    
                    break;
                }
                case PARALLEL_NO_LAZYLOAD:
                {
                    auto loadInThread = m_threadsPool.getThread();
                    // loadInThread->m_autoJoinIfNotBusy = true;
                    auto loadAssetTask = loadInThread->createTask();
                    
                    loadAssetTask->setOnExecuteCallback([asset, path]() {
                        asset->load(path);
                    });
                    
                    loadInThread->addTask(loadAssetTask);
                    loadInThread->start();
                    
                    break;
                }
            }
            
            if(lazyLoadInThread)
            {
                lazyLoadInThread->processFinishedTasks();
            }
        }
        
        std::mutex m_mutex;
        
        Ref<registry_t> m_registry = MakeRef<registry_t>();
        std::unordered_map<size_t, entity_t> m_entities;
        
        Threading::BaseThreadsPool<Threading::LeastTasksCount> m_threadsPool { 2, false };
        
        static inline Ref<AssetManager> m_instance;
    };
}

#endif // SUNGEARENGINE_ASSETMANAGER_H
