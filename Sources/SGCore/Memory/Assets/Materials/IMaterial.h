#ifndef SUNGEARENGINE_IMATERIAL_H
#define SUNGEARENGINE_IMATERIAL_H

#include <SGCore/pch.h>

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/IAsset.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"

namespace SGCore
{
    class IShader;
    class ITexture2D;

    // TODO: make remove texture
    // TODO: make function addBlockDeclaration
    class IMaterial : public std::enable_shared_from_this<IMaterial>, public IAsset
    {
    public:
        static inline size_t asset_type_id = StaticTypeID<IMaterial>::setID(11);

        std::string m_name;

        /**
        * Adds texture2D. Method is copying texture. This method is looking for texture asset by path.
        * @param type - Material type of texture
        * @param path - Path to texture.
        * @return this
        */
        Ref<ITexture2D> findAndAddTexture2D(const SGTextureType& textureType,
                                            const std::string& path,
                                            AssetManager& toAssetManager = *AssetManager::getInstance());

        /**
         * Adds texture2D.
         * @param textureType - Material type of texture
         * @param tex - Texture to add.
         */
        void addTexture2D(const SGTextureType& textureType,
                          const Ref<ITexture2D>& tex);

        void copyTextures(const Ref<IMaterial>& to) const noexcept;

        void setDiffuseColor(const glm::vec4& col) noexcept;

        auto getDiffuseColor() const noexcept
        { return m_diffuseColor; }

        void setSpecularColor(const glm::vec4& col) noexcept;

        auto getSpecularColor() const noexcept
        { return m_specularColor; }

        void setAmbientColor(const glm::vec4& col) noexcept;

        auto getAmbientColor() const noexcept
        { return m_ambientColor; }

        void setEmissionColor(const glm::vec4& col) noexcept;

        auto getEmissionColor() const noexcept
        { return m_emissionColor; }

        void setTransparentColor(const glm::vec4& col) noexcept;

        auto getTransparentColor() const noexcept
        { return m_transparentColor; }

        void setShininess(const float& shininess) noexcept;

        float getShininess() const noexcept { return m_shininess; }

        void setMetallicFactor(const float& metallicFactor) noexcept;

        float getMetallicFactor() const noexcept { return m_metallicFactor; }

        void setRoughnessFactor(const float& roughnessFactor) noexcept;

        float getRoughnessFactor() const noexcept { return m_roughnessFactor; }

        /**
         * Copies all texture assets (textures data is not copied) to another material.\n
         * Also tries to do the same with unused textures of this material.
         * @param other - Copied material
         * @return Modified this material
         */
        IMaterial& operator=(const IMaterial& other) noexcept;

        const auto& getTextures() const noexcept
        {
            return m_textures;
        }

    protected:
        std::unordered_map<SGTextureType, std::vector<Ref<ITexture2D>>> m_textures;

        Ref<IShader> m_shader;

        glm::vec4 m_diffuseColor        = glm::vec4(1.0f);
        glm::vec4 m_specularColor       = glm::vec4(1.0f);
        glm::vec4 m_ambientColor        = glm::vec4(0.0f);
        glm::vec4 m_emissionColor       = glm::vec4(1.0f);
        glm::vec4 m_transparentColor    = glm::vec4(1.0f);
        float m_shininess               = 32.0f;
        float m_metallicFactor          = 1.0f;
        float m_roughnessFactor         = 1.0f;
        
        // TODO: impl
        void doLoad(const std::filesystem::path& path) override;
        
        // first - shader name
        // std::unordered_map<std::string, std::shared_ptr<Graphics::IShader>> m_shaders;
        // std::shared_ptr<Graphics::IShader> m_currentShader;
    };
}

#endif //SUNGEARENGINE_IMATERIAL_H
