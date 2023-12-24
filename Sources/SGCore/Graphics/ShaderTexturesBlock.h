//
// Created by stuka on 24.12.2023.
//

#ifndef SUNGEARENGINE_SHADERTEXTURESBLOCK_H
#define SUNGEARENGINE_SHADERTEXTURESBLOCK_H

#include <string>
#include <list>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class ITexture2D;
    class IShader;
    class IMaterial;

    struct ShaderTexturesBlock
    {
        friend class GLShadersPreprocessor;

        std::string m_uniformName;

        bool m_isSingleTextureBlock = false;

        virtual void addTexture(const Ref<ITexture2D>& texture2D) noexcept = 0;
        virtual void removeTexture(const Ref<ITexture2D>& texture2D) noexcept = 0;
        virtual void clearTextures() noexcept = 0;

        virtual void collectTexturesFromMaterial(const Ref<IMaterial>& material) noexcept { };

        [[nodiscard]] const auto& getTextures() const noexcept
        {
            return m_textures;
        }

        bool operator==(const ShaderTexturesBlock& other) const noexcept;
        bool operator!=(const ShaderTexturesBlock& other) const noexcept;

        void setParentShader(const Ref<IShader>& shader) noexcept;

    protected:
        Weak<IShader> m_parentShader;

        std::list<Weak<ITexture2D>> m_textures;
    };
}

#endif //SUNGEARENGINE_SHADERTEXTURESBLOCK_H
