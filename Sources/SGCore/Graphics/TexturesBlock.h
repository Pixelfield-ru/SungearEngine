//
// Created by stuka on 24.12.2023.
//

#ifndef SUNGEARENGINE_TEXTURESBLOCK_H
#define SUNGEARENGINE_TEXTURESBLOCK_H

#include <string>
#include <list>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"

namespace SGCore
{
    class ITexture2D;
    class ISubPassShader;
    class IMaterial;

    struct TexturesBlock
    {
        friend class GLShadersPreprocessor;

        std::string m_uniformName;

        bool m_isSingleTextureBlock = false;

        virtual void addTexture(const Ref<ITexture2D>& texture2D) noexcept { };
        virtual void addTexture(const Ref<ITexture2D>& texture2D, SGTextureType textureType) noexcept { };
        virtual void removeTexture(const Ref<ITexture2D>& texture2D) noexcept = 0;
        virtual void clearTextures() noexcept = 0;

        virtual void collectTexturesFromMaterial(const Ref<IMaterial>& material) noexcept { };

        [[nodiscard]] const auto& getTextures() const noexcept
        {
            return m_textures;
        }

        bool operator==(const TexturesBlock& other) const noexcept;
        bool operator!=(const TexturesBlock& other) const noexcept;

        void setParentShader(const Ref<ISubPassShader>& shader) noexcept;

        std::list<Weak<ITexture2D>>::const_iterator eraseTexture(const std::list<Weak<ITexture2D>>::const_iterator& iter) noexcept;

    protected:
        Weak<ISubPassShader> m_parentShader;

        std::list<Weak<ITexture2D>> m_textures;
    };
}

#endif //SUNGEARENGINE_TEXTURESBLOCK_H
