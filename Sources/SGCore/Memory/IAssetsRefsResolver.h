//
// Created by stuka on 17.11.2024.
//

#ifndef SUNGEARENGINE_IASSETSREFSRESOLVER_H
#define SUNGEARENGINE_IASSETSREFSRESOLVER_H

#include "SGCore/Utils/EventListener.h"

namespace SGCore
{
    class AssetManager;

    struct IAssetsRefsResolver
    {
        IAssetsRefsResolver() noexcept;
        IAssetsRefsResolver(const IAssetsRefsResolver&) noexcept = default;
        IAssetsRefsResolver(IAssetsRefsResolver&$) noexcept = default;

        IAssetsRefsResolver& operator=(const IAssetsRefsResolver& other) noexcept = default;
        IAssetsRefsResolver& operator=(IAssetsRefsResolver&& other) noexcept = default;

    protected:
        /**
         * Implement this function to resolve reference of member asset that contains in current class.\n
         * Then you can use \p updatedAssetManager->resolveAssetReference(myAssetReferences) or \p updatedAssetManager->resolveWeakAssetReference(myWeakAssetReferences) .
         * @param updatedAssetManager AssetManager for which new assets were deserialized (AssetManager was updated).
         */
        virtual void onMemberAssetsReferencesResolve(AssetManager* updatedAssetManager) noexcept = 0;

    private:
        EventListener<void(AssetManager* updatedAssetManager)> onMemberAssetsReferencesResolveListener = [this](AssetManager* updatedAssetManager) {
            onMemberAssetsReferencesResolve(updatedAssetManager);
        };
    };
}

#endif //SUNGEARENGINE_IASSETSREFSRESOLVER_H
