//
// Created by stuka on 24.12.2023.
//

#ifndef SUNGEARENGINE_TEXTURESFROMMATERIALBLOCK_H
#define SUNGEARENGINE_TEXTURESFROMMATERIALBLOCK_H

#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "TexturesBlock.h"

namespace SGCore
{
    class IMaterial;

    struct TexturesFromMaterialBlock : TexturesBlock
    {
        void addTexture(const Ref<ITexture2D>& texture2D, SGTextureType textureType) noexcept final;
        void removeTexture(const Ref<ITexture2D>& texture2D) noexcept final;
        void clearTextures() noexcept final;

        void collectTexturesFromMaterial(const Ref<IMaterial>& material) noexcept final;

        SGTextureType m_typeToCollect = SGTextureType::SGTT_NONE;
    };
}

#endif //SUNGEARENGINE_TEXTURESFROMMATERIALBLOCK_H
