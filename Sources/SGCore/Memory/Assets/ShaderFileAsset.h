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
        sg_implement_asset_type_id(ShaderFileAsset, 9)

    protected:
        void doLoad(const std::filesystem::path& path) override;
    };
}

#endif //SUNGEARENGINE_SHADERFILEASSET_H
