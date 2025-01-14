//
// Created by stuka on 17.11.2024.
//

#ifndef SUNGEARENGINE_IASSETSREFSRESOLVER_H
#define SUNGEARENGINE_IASSETSREFSRESOLVER_H

#include "SGCore/Utils/EventListener.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Utils/Defines.h"

#define sg_assets_refs_resolver_as_friend template<typename> friend struct IAssetsRefsResolver;

namespace SGCore
{
    class AssetManager;

    // CRTP STRUCT
    /**
     * DECLARE THIS STRUCT AS FRIEND IF YOU INHERITED THIS STRUCT.
     * @tparam DerivedT
     */
    template<typename DerivedT>
    struct IAssetsRefsResolver
    {
        IAssetsRefsResolver() noexcept
        {
            AssetManager::getOnMemberAssetsReferencesResolveEvent() += onMemberAssetsReferencesResolveListener;
        }

        IAssetsRefsResolver(const IAssetsRefsResolver& other) noexcept
        {
            *this = other;
        }

        IAssetsRefsResolver(IAssetsRefsResolver&& other) noexcept
        {
            *this = std::move(other);
        }

        IAssetsRefsResolver& operator=(const IAssetsRefsResolver& other) noexcept
        {
            if(this == std::addressof(other)) return *this;

            onMemberAssetsReferencesResolveListener = other.onMemberAssetsReferencesResolveListener;
            onMemberAssetsReferencesResolveListener = [this](AssetManager* updatedAssetManager) {
                onMemberAssetsReferencesResolve(updatedAssetManager);
            };

            return *this;
        }

        IAssetsRefsResolver& operator=(IAssetsRefsResolver&& other) noexcept
        {
            if(this == std::addressof(other)) return *this;

            onMemberAssetsReferencesResolveListener = std::move(other.onMemberAssetsReferencesResolveListener);
            onMemberAssetsReferencesResolveListener = [this](AssetManager* updatedAssetManager) {
                onMemberAssetsReferencesResolve(updatedAssetManager);
            };

            return *this;
        }

    protected:
        /**
         * Implement this function to resolve reference of member asset that contains in current class.\n
         * Then you can use \p updatedAssetManager->resolveAssetReference(myAssetReferences) or \p updatedAssetManager->resolveWeakAssetReference(myWeakAssetReferences) .
         * @param updatedAssetManager AssetManager for which new assets were deserialized (AssetManager was updated).
         */
        /*SG_CRTP_VIRTUAL void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept
        {

        }*/

    private:
        void onMemberAssetsReferencesResolve(AssetManager* updatedAssetManager) noexcept
        {
            static_cast<DerivedT*>(this)->onMemberAssetsReferencesResolveImpl(updatedAssetManager);
        }

        EventListener<void(AssetManager* updatedAssetManager)> onMemberAssetsReferencesResolveListener = [this](AssetManager* updatedAssetManager) {
            onMemberAssetsReferencesResolve(updatedAssetManager);
        };
    };
}

#endif //SUNGEARENGINE_IASSETSREFSRESOLVER_H
