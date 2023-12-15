#pragma once

#ifndef SUNGEARENGINE_IMATERIAL_H
#define SUNGEARENGINE_IMATERIAL_H

#include <map>
#include <string>
#include <tsl/robin_map.h>

#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/IAsset.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"

namespace SGCore
{
    class IShader;

    class MarkedTexturesBlock;

    struct MaterialTexture
    {
        SGTextureType m_type = SGTextureType::SGTP_DIFFUSE;

        Ref<ITexture2D> m_texture;
    };

    // TODO: make remove texture
    class IMaterial : public std::enable_shared_from_this<IMaterial>, public IAsset
    {
    public:
        std::string m_name;

        std::vector<MaterialTexture> m_textures;
        std::vector<Weak<IFrameBuffer>> m_frameBuffers;

        glm::vec4 m_diffuseColor        = glm::vec4(1.0f);
        glm::vec4 m_specularColor       = glm::vec4(1.0f);
        glm::vec4 m_ambientColor        = glm::vec4(0.0f);
        glm::vec4 m_emissionColor       = glm::vec4(1.0f);
        glm::vec4 m_transparentColor    = glm::vec4(1.0f);
        float m_shininess               = 32.0f;
        float m_metallicFactor          = 1.0f;
        float m_roughnessFactor         = 1.0f;

        Ref<IMaterial> bind(const Ref<IShader>& shader);

        // TODO: impl
        Ref<IAsset> load(const std::string& path) override;

        /**
        * Adds texture2D. This method is looking for texture asset by path.
        * @param type - Material type of texture
        * @param texture2DAsset - Texture asset
        * @return this
        */
        Ref<Texture2DAsset> findAndAddTexture2D(const SGTextureType& textureType,
                                                const std::string& path);

        void copyTextures(const Ref<IMaterial>& to) const noexcept;

        /**
         * Copies all texture assets (textures data is not copied) to another material.\n
         * Also tries to do the same with unused textures of this material.
         * @param other - Copied material
         * @return Modified this material
         */
        IMaterial& operator=(const IMaterial& other) noexcept;

    protected:

        std::unordered_map<SGTextureType, std::uint16_t> m_texturesOfTypeCount;
        // first - shader name
        // std::unordered_map<std::string, std::shared_ptr<Graphics::IShader>> m_shaders;
        // std::shared_ptr<Graphics::IShader> m_currentShader;
    };
}

#endif //SUNGEARENGINE_IMATERIAL_H
