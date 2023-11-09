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

#define SGMAT_STANDARD_SHADER_NAME      "standardShader"
#define SGMAT_SHADOW_GEN_SHADER_NAME    "shadowGenShader"

namespace Core::Graphics
{
    class IShader;
    class MarkedShader;
    class MarkedTexturesBlock;
}

namespace Core::Memory::Assets
{
    struct MaterialTexture
    {
        SGTextureType m_type
        = SGTextureType::SGTP_DIFFUSE;

        std::shared_ptr<Graphics::ITexture2D> m_texture;
    };

    // TODO: make remove texture
    // TODO: make function addBlockDeclaration
    class IMaterial : public std::enable_shared_from_this<IMaterial>, public IAsset
    {
    public:
        std::string m_name;

        std::vector<MaterialTexture> m_textures;

        // TODO: REMOVE
        std::shared_ptr<Graphics::IShader> m_customShader;

        glm::vec4 m_diffuseColor        = glm::vec4(1.0f);
        glm::vec4 m_specularColor       = glm::vec4(1.0f);
        glm::vec4 m_ambientColor        = glm::vec4(0.0f);
        glm::vec4 m_emissionColor       = glm::vec4(1.0f);
        glm::vec4 m_transparentColor    = glm::vec4(1.0f);
        float m_shininess               = 32.0f;
        float m_metallicFactor          = 1.0f;
        float m_roughnessFactor         = 1.0f;

        // FRAMEBUFFER ATTACHMENTS SHOULD BE CUSTOM-BOUND BY THE USER BASED ON MarkedShader!!!!!!!!!!!!!!!!!!
        std::shared_ptr<IMaterial> bind(const std::shared_ptr<Graphics::MarkedShader>& markedShader);

        std::shared_ptr<IMaterial> bind(const std::shared_ptr<Graphics::IShader>& shader,
                                        const std::unordered_map<SGTextureType,
                                                Graphics::MarkedTexturesBlock>& markedTexturesBlocks);

        std::shared_ptr<IMaterial> bind(const std::unordered_map<SGTextureType,
                                                Graphics::MarkedTexturesBlock>& markedTexturesBlocks);

        // TODO: impl
        std::shared_ptr<IAsset> load(const std::string& path) override;

        /**
        * Adds texture2D. Method is copying texture. This method is looking for texture asset by path.
        * @param type - Material type of texture
        * @param texture2DAsset - Texture asset
        * @return this
        */
        std::shared_ptr<Texture2DAsset> findAndAddTexture2D(const SGTextureType& textureType,
                                                            const std::string& path);

        void copyTextures(const std::shared_ptr<IMaterial>& to) const noexcept;

        /**
         * Copies all texture assets (textures data is not copied) to another material.\n
         * Also tries to do the same with unused textures of this material.
         * @param other - Copied material
         * @return Modified this material
         */
        IMaterial& operator=(const IMaterial& other) noexcept;

    protected:

        // first - shader name
        // std::unordered_map<std::string, std::shared_ptr<Graphics::IShader>> m_shaders;
        // std::shared_ptr<Graphics::IShader> m_currentShader;
    };
}

#endif //SUNGEARENGINE_IMATERIAL_H
