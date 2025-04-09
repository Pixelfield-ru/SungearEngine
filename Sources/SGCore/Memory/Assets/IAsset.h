//
// Created by stuka on 07.05.2023.
//

#ifndef NATIVECORE_IASSET_H
#define NATIVECORE_IASSET_H

#include <SGCore/pch.h>
#include <SGCore/Utils/StringInterpolation/InterpolatedPath.h>
#include "SGCore/Utils/UUID.h"
#include "SGCore/Utils/StaticTypeID.h"

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Utils/Signal.h"
#include "SGCore/Serde/SerializationType.h"
#include "SGCore/Memory/AssetsLoadPolicy.h"
#include "SGCore/Threading/Thread.h"
#include "SGCore/Threading/ThreadsManager.h"

// =======================================================================================================
/**
 * @brief Implementation of SerdeSpec for your custom asset type.
 *
 * See the implementation of SerdeSpec for ITexture2D as an example.
 *
 */
// =======================================================================================================

sg_predeclare_serde()

namespace SGCore
{
    class IAssetObserver;

    /// How asset was stored in asset manager.
    enum class AssetStorageType : std::uint8_t
    {
        BY_PATH,
        BY_ALIAS
    };

    template<typename>
    struct AssetRef;
    
    class AssetManager;

    /// You must use \p sg_implement_type_id macro in your assets types to implement static type ID.
    class IAsset
    {
    public:
        sg_serde_as_friend()
        sg_implement_type_id_base(IAsset, 0)

        friend class AssetManager;

        /// You can make a downcast to the type of asset you subscribe to using static_cast<your_type>(asset).
        Signal<void(IAsset* asset)> onLoadDone;
        /// You can make a downcast to the type of asset you subscribe to using static_cast<your_type>(asset).
        Signal<void(IAsset* asset)> onLazyLoadDone;

        virtual ~IAsset() = default;

        void load(const InterpolatedPath& path)
        {
            m_isLoaded = true;
            m_path = path;
            
            doLoad(m_path);
            
            onLoadDone(this);
        }
        
        void lazyLoad()
        {
            m_isLoaded = true;

            doLazyLoad();
            
            onLazyLoadDone(this);
        }

        /**
         * Reloads asset from disk. Calls parent asset manager`s (if parent asset manager is not null) reloadAssetFromDisk function for this function.\n
         * Parent asset manager calls doReloadFromDisk function for this asset.\n
         * lazyLoad function is called directly after execution of this function.\n
         * This function can be executed parallel.
         */
        void reloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread = Threading::ThreadsManager::getMainThread()) noexcept;

        /**
         * Reloads asset from disk. Calls parent asset manager`s (if parent asset manager is not null) reloadAssetFromDisk function for this function.\n
         * Parent asset manager calls doReloadFromDisk function for this asset.\n
         * lazyLoad function is called directly after execution of this function.\n
         * This function can be executed parallel.
         */
        void reloadFromDisk() noexcept;

        void loadFromBinaryFile(AssetManager* parentAssetManager) noexcept
        {
            m_isLoaded = true;

            doLoadFromBinaryFile(parentAssetManager);
        }

        long getLastModified() noexcept;
        [[nodiscard]] const InterpolatedPath& getPath() const noexcept;
        [[nodiscard]] const std::string& getAlias() const noexcept;
        [[nodiscard]] AssetStorageType storedByWhat() const noexcept;

        [[nodiscard]] Ref<AssetManager> getParentAssetManager() const noexcept;

        [[nodiscard]] bool isLoaded() const noexcept;

        [[nodiscard]] bool isSavedBinaryFile() const noexcept;

    protected:

        /// In the implementation of the \p doLoad function, you must implement all the logic of loading an asset, which can be executed in parallel (for example: loading an asset from disk).
        /// The \p doLoad function can be called in parallel.
        virtual void doLoad(const InterpolatedPath& path) = 0;
        /// In the implementation of the \p doLazyLoad function, you must implement all the logic of asset loading, which cannot be executed in a separate thread.
        /// For example: creating a GPU object.
        virtual void doLazyLoad() { };

        virtual void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) = 0;

        /**
         * In the implementation of this function, you must read data from the binary file of the asset manager
         * and save it to heavy data-variables that you serialized in the SerdeSpec implementation for the asset of the current type.\n
         * For example: your asset class contains a heavy variable \p m_data. Along with it, you must store fields such as:
         * \p m_dataOffset to indicate the position of \p m_data in the \p parentAssetManager binary file and \p m_dataSizeInBytes to indicate the size of the serialized \p m_data in bytes.\n
         * To understand how to implement SerdeSpec for your custom asset type see the text 'Implementation of SerdeSpec for your custom asset type'.
         *
         * @see Implementation of SerdeSpec for your custom asset type.
         *
         * @param parentAssetManager Parent AssetManager, that contains current used AssetsPackage.\n
         * Use that AssetPackage from 'parentAssetManager' to read data from binary file.
         */
        virtual void doLoadFromBinaryFile(AssetManager* parentAssetManager) = 0;

        /// Indicates whether this asset was loaded along the path to any file (for example: .wav, .gltf) or loaded from the binary file of the some AssetManager.\n
        /// You can change value of this variable in your implementations of \p doLoad , \p doLazyLoad or \p doLoadFromBinaryFile functions to indicate whether this asset was successfully loaded or it is need to be reloaded.
        bool m_isLoaded = false;
        /// Is asset is in loading now.
        bool m_isLoadingNow = false;

        long m_lastModified = -1;

    private:
        Weak<AssetManager> m_parentAssetManager;

        // we are generating UUID for these fields to guarantee uniqueness for every asset even the one that wasn`t added to AssetManager
        InterpolatedPath m_path = UUID::generateNew();
        std::string m_alias = UUID::generateNew();

        AssetStorageType m_storedBy = AssetStorageType::BY_ALIAS;

        /// Specifies whether to load this asset from a binary file. If true, the \p loadFromBinaryFile function is called.
        /// READ ONLY.
        bool m_isSavedInBinaryFile = false;

        /// You must implement this function if your asset type is abstract and can be created only using factory (for example).
        /*template<typename... AssetCtorArgs>
        static Ref<IAsset> createRefInstance(AssetCtorArgs&&... assetCtorArgs) noexcept
        {
            return MakeRef<IAsset>(std::forward<AssetCtorArgs>(assetCtorArgs)...);
        }*/
    };

    template <class T>
    concept DerivedFromIAsset = std::is_base_of_v<IAsset, T>;
}

#endif //NATIVECORE_IASSET_H
