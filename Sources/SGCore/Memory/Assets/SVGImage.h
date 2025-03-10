//
// Created by ilya on 21.05.24.
//

#ifndef SUNGEARENGINE_SVGIMAGE_H
#define SUNGEARENGINE_SVGIMAGE_H

#include <SGCore/pch.h>

#include "IAsset.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SVGImageSpecialization.h"

namespace SGCore
{
    struct SVGImage : public IAsset, public IAssetsRefsResolver<SVGImage>
    {
        sg_implement_type_id(SVGImage, 6)

        sg_assets_refs_resolver_as_friend

        [[nodiscard]] AssetRef<SVGImageSpecialization> getSpecialization(const std::uint32_t& width, const std::uint32_t& height) noexcept;
        void removeSpecialization(const std::uint32_t& width, const std::uint32_t& height) noexcept;
    
    protected:
        void doLoad(const InterpolatedPath& path) override;

        // TODO: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;

        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept SG_CRTP_OVERRIDE;

        std::vector<AssetRef<SVGImageSpecialization>> m_specializations;
        
    private:
        std::unique_ptr<lunasvg::Document> m_document;
    };
}

#endif //SUNGEARENGINE_SVGIMAGE_H
