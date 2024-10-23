//
// Created by stuka on 07.05.2023.
//

#ifndef NATIVECORE_IASSET_H
#define NATIVECORE_IASSET_H

#include <SGCore/pch.h>
#include "SGCore/Utils/StaticTypeID.h"

#include "IAssetObserver.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Utils/Event.h"
#include "SGCore/Serde/SerializationType.h"
#include "SGCore/Memory/AssetsPackage.h"

namespace SGCore
{
    class IAssetObserver;

    class IAsset : public UniqueNameWrapper
    {
        friend class AssetManager;

    public:
        /// You must implement this field in your type of asset. This field must have explicit value and be the same on different platforms.
        static inline size_t asset_type_id = StaticTypeID<IAsset>::setID(0);

        std::string m_name;
        
        /// You can make a downcast to the type of asset you subscribe to using static_cast<your_type>(asset).
        Event<void(IAsset* asset)> onLoadDone;
        /// You can make a downcast to the type of asset you subscribe to using static_cast<your_type>(asset).
        Event<void(IAsset* asset)> onLazyLoadDone;

        void load(const std::filesystem::path& path)
        {
            m_path = path;
            
            doLoad(path);
            
            onLoadDone(this);
        }
        
        void lazyLoad()
        {
            doLazyLoad();
            
            onLazyLoadDone(this);
        }

        // LEGACY CODE ================================
        
        void addObserver(const std::shared_ptr<IAssetObserver>&) noexcept;
        void removeObserver(const std::shared_ptr<IAssetObserver>&) noexcept;

        void onModified();
        void onPathChanged();
        void onDeleted();
        void onRestored();
        
        // =============================================

        long getLastModified() noexcept;
        [[nodiscard]] std::filesystem::path getPath() const noexcept;

    protected:
        virtual void doLoad(const std::filesystem::path& path) = 0;
        virtual void doLazyLoad() { };

        virtual void serializeToPackage(AssetsPackage::AssetSection& currentAssetSection, bool isDataSerializing) = 0;
        
        long m_lastModified = -1;
        std::filesystem::path m_path;
        std::list<Weak<IAssetObserver>> m_observers;

    private:
        template<typename InstanceT, typename... AssetCtorArgs>
        requires(std::is_base_of_v<IAsset, InstanceT>)
        static Ref<InstanceT> createRefInstance(AssetCtorArgs&&... assetCtorArgs) noexcept
        {
            return MakeRef<InstanceT>(std::forward<AssetCtorArgs>(assetCtorArgs)...);
        }
    };

    template <class T>
    concept DerivedFromIAsset = std::is_base_of_v<IAsset, T>;
}

#endif //NATIVECORE_IASSET_H
