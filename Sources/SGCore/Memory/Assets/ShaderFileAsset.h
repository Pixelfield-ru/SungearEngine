//
// Created by Ilya on 18.09.2023.
//

#ifndef SUNGEARENGINE_SHADERFILEASSET_H
#define SUNGEARENGINE_SHADERFILEASSET_H

#include "IAsset.h"

#include "SGCore/Graphics/API/ISubPassShader.h"

namespace SGCore
{
    class ShaderFileAsset : public IAsset, public std::enable_shared_from_this<ShaderFileAsset>
    {
    public:
        [[nodiscard]] std::shared_ptr<IAsset> load(const std::string& path) override;
    };
}

#endif //SUNGEARENGINE_SHADERFILEASSET_H
