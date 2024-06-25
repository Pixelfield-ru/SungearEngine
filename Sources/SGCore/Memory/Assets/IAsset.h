//
// Created by stuka on 07.05.2023.
//

#ifndef NATIVECORE_IASSET_H
#define NATIVECORE_IASSET_H

#include <SGCore/pch.h>

#include "IAssetObserver.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Utils/Event.h"

namespace SGCore
{
    class IAssetObserver;

    class IAsset : public UniqueNameWrapper
    {
        friend class AssetManager;

    public:
        std::string m_name;
        
        /// You can make a downcast to the type of asset you subscribe to using static_cast<your_type>(asset).
        Event<void(IAsset* asset)> onLoadDone;
        /// You can make a downcast to the type of asset you subscribe to using static_cast<your_type>(asset).
        Event<void(IAsset* asset)> onLazyLoadDone;

        void load(const std::string& path)
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
        virtual void doLoad(const std::string& path) = 0;
        virtual void doLazyLoad() { };
        
        long m_lastModified = -1;
        std::filesystem::path m_path;
        std::list<Weak<IAssetObserver>> m_observers;

    private:
        size_t m_assetTypeID = 0;
        
        template<typename InstanceT, typename... AssetCtorArgs>
        requires(std::is_base_of_v<IAsset, InstanceT>)
        static Ref<InstanceT> createRefInstance(AssetCtorArgs&&... assetCtorArgs) noexcept
        {
            return MakeRef<InstanceT>(std::forward<AssetCtorArgs>(assetCtorArgs)...);
        }
    };
}

#endif //NATIVECORE_IASSET_H
