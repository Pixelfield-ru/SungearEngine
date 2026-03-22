//
// Created by Ilya on 18.09.2023.
//

#pragma once

#include "IAsset.h"

#include "SGCore/Graphics/API/IShader.h"

namespace SGCore
{
    // todo: del
    struct SGCORE_EXPORT ShaderFileAsset : public IAsset
    {
        sg_implement_type_id(SGCore::ShaderFileAsset)

    protected:
        void doLoad(const InterpolatedPath& path) override;

        // TODO: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;
    };
}
