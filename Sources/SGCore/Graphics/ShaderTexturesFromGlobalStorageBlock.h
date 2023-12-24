//
// Created by stuka on 24.12.2023.
//

#ifndef SUNGEARENGINE_SHADERTEXTURESFROMGLOBALSTORAGEBLOCK_H
#define SUNGEARENGINE_SHADERTEXTURESFROMGLOBALSTORAGEBLOCK_H

#include <string>
#include <list>

#include "SGCore/Main/CoreGlobals.h"
#include "ShaderTexturesBlock.h"

namespace SGCore
{
    struct ShaderTexturesFromGlobalStorageBlock : ShaderTexturesBlock
    {
        void addTexture(const Ref<ITexture2D>& texture2D) noexcept final;
        void removeTexture(const Ref<ITexture2D>& texture2D) noexcept final;
        void clearTextures() noexcept final;

    private:
        std::list<std::string> m_requiredTexturesNames;
    };
}

#endif //SUNGEARENGINE_SHADERTEXTURESFROMGLOBALSTORAGEBLOCK_H
