//
// Created by stuka on 28.05.2023.
//

#ifndef SUNGEARENGINE_GL46TEXTURE2D_H
#define SUNGEARENGINE_GL46TEXTURE2D_H

#include "SGCore/Graphics/API/GL/GL4/GL4Texture2D.h"

namespace SGCore
{
    class GL46Texture2D : public GL4Texture2D
    {
    public:
        ~GL46Texture2D() noexcept;

        void create(std::weak_ptr<Texture2DAsset>) noexcept final;
        void destroy() noexcept final;

        void onAssetModified() noexcept final;
        void onAssetPathChanged() noexcept final;
        void onAssetDeleted() noexcept final;
        void onAssetRestored() noexcept final;

        void bind(const std::uint8_t& textureUnit) noexcept final;

        GL46Texture2D& operator=(const std::shared_ptr<ITexture2D>& other) final;
    };
}

#endif //SUNGEARENGINE_GL46TEXTURE2D_H
