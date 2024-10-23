//
// Created by Ilya on 18.09.2023.
//

#ifndef SUNGEARENGINE_SHADERFILEASSET_H
#define SUNGEARENGINE_SHADERFILEASSET_H

#include "IAsset.h"

#include "SGCore/Graphics/API/ISubPassShader.h"

namespace SGCore
{
    struct ShaderFileAsset : public IAsset
    {
        static inline size_t asset_type_id = StaticTypeID<ShaderFileAsset>::setID(9);

    protected:
        void doLoad(const std::filesystem::path& path) override;

        // todo:
        void serializeToPackage(AssetsPackage::AssetSection& currentAssetSection, bool isDataSerializing) override;
    };
}

#endif //SUNGEARENGINE_SHADERFILEASSET_H
