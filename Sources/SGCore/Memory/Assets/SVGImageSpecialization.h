//
// Created by ilya on 21.05.24.
//

#ifndef SUNGEARENGINE_SVGIMAGESPECIALIZATION_H
#define SUNGEARENGINE_SVGIMAGESPECIALIZATION_H

#include <SGCore/pch.h>

#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Memory/AssetRef.h"
#include "SGCore/Memory/IAssetsRefsResolver.h"

namespace SGCore
{
    struct SVGImageSpecialization : public IAsset, public IAssetsRefsResolver<SVGImageSpecialization>
    {
        friend struct SVGImage;

        sg_implement_type_id(SVGImageSpecialization, 12)

        sg_assets_refs_resolver_as_friend
        
        void regenerate(std::uint8_t channelsCount = 4,
                        SGGColorInternalFormat internalColorFormat = SGGColorInternalFormat::SGG_RGBA8,
                        SGGColorFormat colorFormat = SGGColorFormat::SGG_RGBA) noexcept;
        
        [[nodiscard]] lunasvg::Bitmap& getBitmap() noexcept;
        
        [[nodiscard]] AssetRef<ITexture2D> getTexture() noexcept;
        
        [[nodiscard]] uivec2_32 getSize() const noexcept;

    protected:
        void doLoad(const InterpolatedPath& path) noexcept final { }
        // todo: implement
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) final;

        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept SG_CRTP_OVERRIDE;

    private:
        lunasvg::Bitmap m_bitmap;
        
        AssetRef<ITexture2D> m_texture;
        
        uivec2_32 m_size { 0, 0 };
    };
}

#endif // SUNGEARENGINE_SVGIMAGESPECIALIZATION_H
