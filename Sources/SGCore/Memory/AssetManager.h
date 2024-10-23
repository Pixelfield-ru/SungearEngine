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
            std::lock_guard guard(m_mutex);;

            const size_t hashedAssetPath = std::hash<std::string>()(Utils::toUTF8(path.u16string()));

            // getting variants of assets that were loaded by path 'path'
            auto& foundVariants = m_assets[hashedAssetPath];

            // trying to find existing asset of type 'AssetT' loaded bt path 'path'
            auto foundAssetOfTIt = foundVariants.find(AssetT::asset_type_id);
            // if asset already exists then we are just leaving
            if(foundAssetOfTIt != foundVariants.end())
            {
                // WE ARE USING STATIC CAST BECAUSE WE KNOW THAT ONLY AN ASSET WITH THE ASSET TYPE HAS SUCH AN asset_type_id.
                // IF THIS IS NOT THE CASE, AND SUDDENLY IT TURNS OUT THAT THERE ARE TWO OR MORE ASSET CLASSES WITH THE SAME asset_type_id, THEN ALAS (i hope engine will crash).
                return std::static_pointer_cast<AssetT>(foundAssetOfTIt->second);
            }

            Ref<AssetT> newAsset = AssetT::template createRefInstance<AssetT>(std::forward<AssetCtorArgs>(assetCtorArgs)...);
            foundVariants[AssetT::asset_type_id] = newAsset;

            
            std::filesystem::path p(path);
            
            distributeAsset(newAsset, path, assetsLoadPolicy, lazyLoadInThread);
            
            newAsset->setRawName(p.stem().string());

            LOG_I(SGCORE_TAG, "Loaded new asset associated by path: {}. Asset type: {}. Asset type ID: {}", Utils::toUTF8(path.u16string()), typeid(AssetT).name(), AssetT::asset_type_id);
            
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

            const size_t hashedAssetPath = std::hash<std::string>()(Utils::toUTF8(path.u16string()));

            // getting variants of assets that were loaded by path 'path'
            auto& foundVariants = m_assets[hashedAssetPath];

            // trying to find existing asset of type 'AssetT' loaded bt path 'path'
            auto foundAssetOfTIt = foundVariants.find(AssetT::asset_type_id);
            // if asset already exists then we are just leaving
            if(foundAssetOfTIt != foundVariants.end())
            {
                return;
            }

            foundVariants[AssetT::asset_type_id] = assetToLoad;
            
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

            const size_t hashedAssetAlias = std::hash<std::string>()(alias);

            // getting variants of assets that were loaded with alias 'alias'
            auto& foundVariants = m_assets[hashedAssetAlias];

            // trying to find existing asset of type 'AssetT' loaded with alias 'alias'
            auto foundAssetOfTIt = foundVariants.find(AssetT::asset_type_id);
            // if asset already exists then we are just leaving
            if(foundAssetOfTIt != foundVariants.end())
            {
                return;
            }

            // else we are assigning asset of type 'AssetT'
            foundVariants[AssetT::asset_type_id] = assetToLoad;
            
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

            const size_t hashedAssetAlias = std::hash<std::string>()(alias);

            // getting variants of assets that were loaded with alias 'alias'
            auto& foundVariants = m_assets[hashedAssetAlias];

            // trying to find existing asset of type 'AssetT' loaded with alias 'alias'
            auto foundAssetOfTIt = foundVariants.find(AssetT::asset_type_id);
            // if asset already exists then we are just leaving
            if(foundAssetOfTIt != foundVariants.end())
            {
                // WE ARE USING STATIC CAST BECAUSE WE KNOW THAT ONLY AN ASSET WITH THE ASSET TYPE HAS SUCH AN asset_type_id.
                // IF THIS IS NOT THE CASE, AND SUDDENLY IT TURNS OUT THAT THERE ARE TWO OR MORE ASSET CLASSES WITH THE SAME asset_type_id, THEN ALAS (i hope engine will crash).
                return std::static_pointer_cast<AssetT>(foundAssetOfTIt->second);
            }

            // else we are creating new asset with type 'AssetT'
            Ref<AssetT> newAsset = AssetT::template createRefInstance<AssetT>(std::forward<AssetCtorArgs>(assetCtorArgs)...);
            foundVariants[AssetT::asset_type_id] = newAsset;
            
            distributeAsset(newAsset, path, assetsLoadPolicy, lazyLoadInThread);
            
            newAsset->setRawName(alias);

            LOG_I(SGCORE_TAG, "Loaded new asset associated by path: {}; and alias: {}. Asset type: {}. Asset type ID: {}", Utils::toUTF8(path.u16string()), alias, typeid(AssetT).name(), AssetT::asset_type_id);
            
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

            const size_t hashedAssetAlias = std::hash<std::string>()(alias);

            // getting variants of assets that were loaded with alias 'alias'
            auto& foundVariants = m_assets[hashedAssetAlias];

            // trying to find existing asset of type 'AssetT' loaded with alias 'alias'
            auto foundAssetOfTIt = foundVariants.find(AssetT::asset_type_id);
            // if asset already exists then we are just leaving
            if(foundAssetOfTIt != foundVariants.end())
            {
                return;
            }

            // assigning new asset by type 'AssetT'
            foundVariants[AssetT::asset_type_id] = asset;
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

            // getting variants of assets that were loaded by path 'assetPath'
            auto& foundVariants = m_assets[hashedAssetPath];

            // trying to find existing asset of type 'AssetT' loaded by path 'assetPath'
            auto foundAssetOfTIt = foundVariants.find(AssetT::asset_type_id);
            // if asset already exists then we are just leaving
            if(foundAssetOfTIt != foundVariants.end())
            {
                return;
            }

            // assigning new asset by type 'AssetT'
            foundVariants[AssetT::asset_type_id] = asset;
            
            if(SG_INSTANCEOF(asset.get(), GPUObject))
            {
                std::dynamic_pointer_cast<GPUObject>(asset)->addToGlobalStorage();
            }

            LOG_I(SGCORE_TAG, "Added new asset with alias '{}', path '{}' and type '{}'", asset->m_name, Utils::toUTF8(asset->getPath().u16string()), typeid(AssetT).name())
        }
        
        bool areAssetsVariantsExist(const std::string& pathOrAlias) noexcept
        {
            auto foundVariantsIt = m_assets.find(std::hash<std::string>()(pathOrAlias));
            
            return foundVariantsIt != m_assets.end();
        }
        
        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        bool isAssetExists(const std::string& pathOrAlias) noexcept
        {
            std::lock_guard guard(m_mutex);
            
            auto foundVariantsIt = m_assets.find(std::hash<std::string>()(pathOrAlias));
            if(foundVariantsIt == m_assets.end())
            {
                return false;
            }
            else
            {
                auto foundAssetIt = foundVariantsIt->second.find(AssetT::asset_type_id);
                return foundAssetIt != foundVariantsIt->second.end();
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
        requires(std::is_base_of_v<IAsset, AssetT>)
        void removeAssetLoadedByType(const std::filesystem::path& aliasOrPath) noexcept
        {
            auto foundIt = m_assets.find(std::hash<std::string>()(Utils::toUTF8(aliasOrPath.u16string())));
            if(foundIt == m_assets.end()) return;

            auto& variants = foundIt->second;
            variants.erase(AssetT::asset_type_id);
        }

        template<typename AssetT>
        requires(std::is_base_of_v<IAsset, AssetT>)
        [[nodiscard]] std::vector<Ref<AssetT>> getAssetsWithType() const noexcept
        {
            std::vector<Ref<AssetT>> assets;
            for(const auto& p0 : m_assets)
            {
                auto foundAssetWithTIt = p0.second.find(AssetT::asset_type_id);
                if(foundAssetWithTIt == p0.second.end())
                {
                    continue;
                }

                // WE ARE USING STATIC CAST BECAUSE WE KNOW THAT ONLY AN ASSET WITH THE ASSET TYPE HAS SUCH AN asset_type_id.
                // IF THIS IS NOT THE CASE, AND SUDDENLY IT TURNS OUT THAT THERE ARE TWO OR MORE ASSET CLASSES WITH THE SAME asset_type_id, THEN ALAS (i hope engine will crash).
                assets.push_back(std::static_pointer_cast<AssetT>(foundAssetWithTIt->second));
            }
            return assets;
        }

        void clear() noexcept;
        
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

        // first - hash of path or alias
        // second - asset by path but loaded in different formats
        //      first - type name id
        //      second - asset
        std::unordered_map<size_t, std::unordered_map<size_t, Ref<IAsset>>> m_assets;
        
        Threading::BaseThreadsPool<Threading::LeastTasksCount> m_threadsPool { 2, false };
        
        static inline Ref<AssetManager> m_instance;
    };
}

#endif // SUNGEARENGINE_ASSETMANAGER_H
