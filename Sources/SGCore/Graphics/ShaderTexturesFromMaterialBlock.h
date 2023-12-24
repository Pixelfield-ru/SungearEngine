//
// Created by stuka on 24.12.2023.
//

#ifndef SUNGEARENGINE_SHADERTEXTURESFROMMATERIALBLOCK_H
#define SUNGEARENGINE_SHADERTEXTURESFROMMATERIALBLOCK_H

#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "ShaderTexturesBlock.h"

namespace SGCore
{
    class IMaterial;

    struct ShaderTexturesFromMaterialBlock : ShaderTexturesBlock
    {
        void addTexture(const Ref<ITexture2D>& texture2D) noexcept final;
        void removeTexture(const Ref<ITexture2D>& texture2D) noexcept final;
        void clearTextures() noexcept final;

        void collectTexturesFromMaterial(const Ref<IMaterial>& material) noexcept final;

    private:
        SGTextureType m_typeToCollect = SGTextureType::SGTT_NONE;
    };
}

#endif //SUNGEARENGINE_SHADERTEXTURESFROMMATERIALBLOCK_H
