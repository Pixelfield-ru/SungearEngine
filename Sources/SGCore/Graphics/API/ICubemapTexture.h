//
// Created by stuka on 29.08.2023.
//

#pragma once

#include "SGCore/Memory/IAssetsRefsResolver.h"
#include "ITexture2D.h"

#include "SGCore/Main/CoreGlobals.h"

#include "SGCore/Memory/AssetRef.h"

namespace SGCore
{
    struct SGCORE_EXPORT ICubemapTexture : public ITexture2D, public IAssetsRefsResolver<ICubemapTexture>
    {
        friend class AssetManager;
        sg_assets_refs_resolver_as_friend

        ICubemapTexture() noexcept;

        std::vector<AssetRef<ITexture2D>> m_parts;

    protected:
        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept;

    private:
        template<typename... AssetCtorArgs>
        static Ref<ICubemapTexture> createRefInstance(AssetCtorArgs&&... assetCtorArgs) noexcept
        {
            auto tex = Ref<ICubemapTexture>(CoreMain::getRenderer()->createCubemapTexture(std::forward<AssetCtorArgs>(assetCtorArgs)...));

            return tex;
        }
    };
}
