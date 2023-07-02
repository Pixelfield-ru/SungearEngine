#pragma once

#ifndef SUNGEARENGINE_PBRMATERIAL_H
#define SUNGEARENGINE_PBRMATERIAL_H

#include "IMaterial.h"

namespace Core::Memory::Assets
{
    class PBRMaterial : public IMaterial
    {
    private:

    public:
        PBRMaterial();

        //std::shared_ptr<IMaterial> bind() final;
        std::shared_ptr<IAsset> load(const std::string& path) final;

        std::shared_ptr<IMaterial> addTexture2D(const SGMaterialTextureType& type,
                                                const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset) final;

        std::shared_ptr<IMaterial> findAndAddTexture2D(const SGMaterialTextureType& type, const std::string& path) final;

        std::shared_ptr<IMaterial> setTexture2D(const SGMaterialTextureType& type, const std::string_view& name,
                                                const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset) final;

        std::shared_ptr<IMaterial> findAndSetTexture2D(const SGMaterialTextureType& type,
                                                       const std::string& path) final;
    };
}

#endif //SUNGEARENGINE_PBRMATERIAL_H
