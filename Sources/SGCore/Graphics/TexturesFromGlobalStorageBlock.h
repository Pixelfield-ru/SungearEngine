//
// Created by stuka on 24.12.2023.
//

#ifndef SUNGEARENGINE_TEXTURESFROMGLOBALSTORAGEBLOCK_H
#define SUNGEARENGINE_TEXTURESFROMGLOBALSTORAGEBLOCK_H

#include <string>
#include <list>

#include "SGCore/Main/CoreGlobals.h"
#include "TexturesBlock.h"

namespace SGCore
{
    struct TexturesFromGlobalStorageBlock : TexturesBlock
    {
        void addTexture(const Ref<ITexture2D>& texture2D) noexcept final;
        void removeTexture(const Ref<ITexture2D>& texture2D) noexcept final;
        void clearTextures() noexcept final;

        std::list<std::string> m_requiredTexturesNames;
    };
}

#endif //SUNGEARENGINE_TEXTURESFROMGLOBALSTORAGEBLOCK_H
