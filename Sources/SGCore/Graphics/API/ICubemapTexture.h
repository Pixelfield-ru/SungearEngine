//
// Created by stuka on 29.08.2023.
//

#ifndef SUNGEARENGINE_ICUBEMAPTEXTURE_H
#define SUNGEARENGINE_ICUBEMAPTEXTURE_H

#include "SGCore/Memory/IAssetsRefsResolver.h"
#include "ITexture2D.h"

#include "SGCore/Main/CoreGlobals.h"

#include "SGCore/Memory/AssetRef.h"

namespace SGCore
{
    struct ICubemapTexture : public ITexture2D, public IAssetsRefsResolver<ICubemapTexture>
    {
        friend class AssetManager;
        sg_assets_refs_resolver_as_friend

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

#endif //SUNGEARENGINE_ICUBEMAPTEXTURE_H
