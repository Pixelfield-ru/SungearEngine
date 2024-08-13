//
// Created by stuka on 29.08.2023.
//

#ifndef SUNGEARENGINE_ICUBEMAPTEXTURE_H
#define SUNGEARENGINE_ICUBEMAPTEXTURE_H

#include "ITexture2D.h"

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct ICubemapTexture : public ITexture2D
    {
        friend class AssetManager;

        std::vector<Ref<ITexture2D>> m_parts;

    private:
        template<typename InstanceT, typename... AssetCtorArgs>
        requires(std::is_same_v<ICubemapTexture, InstanceT>)
        static Ref<InstanceT> createRefInstance(AssetCtorArgs&&... assetCtorArgs) noexcept
        {
            auto tex = Ref<InstanceT>(CoreMain::getRenderer()->createCubemapTexture());

            tex->addToGlobalStorage(std::forward<AssetCtorArgs>(assetCtorArgs)...);

            return tex;
        }
    };
}

#endif //SUNGEARENGINE_ICUBEMAPTEXTURE_H
