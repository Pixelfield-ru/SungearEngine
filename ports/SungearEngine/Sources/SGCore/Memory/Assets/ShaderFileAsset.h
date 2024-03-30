//
// Created by Ilya on 18.09.2023.
//

#ifndef SUNGEARENGINE_SHADERFILEASSET_H
#define SUNGEARENGINE_SHADERFILEASSET_H

#include "IAsset.h"

#include "SGCore/Graphics/API/ISubPassShader.h"

namespace SGCore
{
    class ShaderFileAsset : public IAsset
    {
    public:
        [[nodiscard]] void load(const std::string& path) override;
    };
}

#endif //SUNGEARENGINE_SHADERFILEASSET_H
