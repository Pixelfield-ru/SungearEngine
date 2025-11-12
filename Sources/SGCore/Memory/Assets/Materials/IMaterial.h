#ifndef SUNGEARENGINE_IMATERIAL_H
#define SUNGEARENGINE_IMATERIAL_H

#include "SGCore/Graphics/API/RenderState.h"
#include "SGCore/Memory/IAssetsRefsResolver.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/AssetRef.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Render/Alpha/MaterialTransparencyType.h"

namespace SGCore
{
    class IShader;
    class ITexture2D;



    // TODO: make remove texture
    // TODO: make function addBlockDeclaration
    class IMaterial : public IAsset, public IAssetsRefsResolver<IMaterial>
    {
    public:
        sg_serde_as_friend()

        sg_implement_type_id(IMaterial, 11)

        sg_assets_refs_resolver_as_friend

        IMaterial() noexcept;
        IMaterial(const IMaterial&) noexcept = default;
        IMaterial(IMaterial&&) noexcept = default;

        std::string m_name;

        // first - tag, second - shader
        std::unordered_map<std::string, AssetRef<IShader>> m_shaders;

        MeshRenderState m_meshRenderState;

        MaterialTransparencyType m_transparencyType = MaterialTransparencyType::MAT_OPAQUE;

        /**
        * Adds texture2D. Method is copying texture. This method is looking for texture asset by path.
        * @param type Material type of texture
        * @param path Path to texture.
        * @return this
        */
        AssetRef<ITexture2D> findAndAddTexture2D(const SGTextureSlot& textureType,
                                                 const InterpolatedPath& path,
                                                 AssetManager& toAssetManager = *AssetManager::getInstance());

        /**
         * Adds texture2D.
         * @param textureType Material type of texture
         * @param tex Texture to add.
         */
        void addTexture2D(const SGTextureSlot& textureType,
                          const AssetRef<ITexture2D>& tex);

        /**
         * Replaces old texture in textures array by new texture.
         * @param textureType Type of texture to replace.
         * @param textureIndex  Index of texture in vector of texture with type \ptextureType . Array of textures can contain multiple texture with same type.
         * @param newTex New texture that will replace old texture.
         */
        void replaceTexture(SGTextureSlot textureType, int textureIndex, const AssetRef<ITexture2D>& newTex);

        AssetRef<ITexture2D> getTexture(const SGTextureSlot& textureType, int textureIndex) noexcept;

        void copyTexturesRefs(IMaterial* to) const noexcept;

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
        // some textures can be atlases
        std::array<std::vector<AssetRef<ITexture2D>>, texture_types_count> m_textures;
        // std::unordered_map<SGTextureSlot, std::vector<AssetRef<ITexture2D>>> m_textures;

        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept;

        glm::vec4 m_diffuseColor        = glm::vec4(1.0f);
        glm::vec4 m_specularColor       = glm::vec4(1.0f);
        glm::vec4 m_ambientColor        = glm::vec4(0.0f);
        glm::vec4 m_emissionColor       = glm::vec4(1.0f);
        glm::vec4 m_transparentColor    = glm::vec4(1.0f);
        float m_shininess               = 32.0f;
        float m_metallicFactor          = 0.5f;
        float m_roughnessFactor         = 0.2f;

        void doLoad(const InterpolatedPath& path) override;
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;
        // todo: impl
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;
        
        // first - shader name
        // std::unordered_map<std::string, std::shared_ptr<Graphics::IShader>> m_shaders;
        // std::shared_ptr<Graphics::IShader> m_currentShader;
    };
}

#endif //SUNGEARENGINE_IMATERIAL_H
