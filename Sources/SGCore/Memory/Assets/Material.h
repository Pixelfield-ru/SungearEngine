#pragma once

#ifndef SUNGEARENGINE_MATERIAL_H
#define SUNGEARENGINE_MATERIAL_H

#define SGMAT_BASE_COLOR "material_baseColor"
#define SGMAT_SPECULAR_COLOR "material_specularColor"
#define SGMAT_ROUGHNESS "material_roughness"
#define SGMAT_NORMAL_MAP "material_normalMap"
#define SGMAT_PARALLAX_MAP "material_parallaxMap"
#define SGMAT_OCCLUSION "material_occlusion"
#define SGMAT_EMISSIVE "material_emissive"

#include <map>
#include <string>

#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/IAsset.h"

namespace Core::Memory::Assets
{
    class Material : public std::enable_shared_from_this<Material>, public IAsset
    {
    private:
        std::map<std::string, std::shared_ptr<Graphics::API::ITexture2D>> m_textures;
        std::shared_ptr<Graphics::API::IShader> m_shader;

    public:
        Material() noexcept;

        #pragma region Materials

        void createAsPBR() noexcept;

        #pragma endregion

        std::shared_ptr<Material> bind() noexcept;

        // TODO: impl
        std::shared_ptr<IAsset> load(const std::string_view& path) final;

        /**
        * Adds texture2D. Method is copying texture.
        * @param name - Name of texture
        * @param texture2DAsset - Texture asset
        * @return this
        */
        std::shared_ptr<Material> addTexture2D(const std::string& name,
                          const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset) noexcept;

        /**
        * Adds texture2D. Method is copying texture. This method is looking for texture asset by path.
        * @param name - Name of texture
        * @param texture2DAsset - Texture asset
        * @return this
        */
        std::shared_ptr<Material> findAndAddTexture2D(const std::string& name, const std::string& path) noexcept;

        /**
         * Sets texture2D by name. Method is copying texture.
         * @param name - Name of texture
         * @param texture2DAsset - Texture asset
         * @return this
         */
        std::shared_ptr<Material> setTexture2D(const std::string_view& name,
                                               const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset) noexcept;
        /**
         * Sets texture2D by name. Method is copying texture. This method is looking for texture asset by path.
         * @param name - Name of texture
         * @param path - Path to texture asset
         * @return
         */
        std::shared_ptr<Material> findAndSetTexture2D(const std::string_view& name,
                                               const std::string& path) noexcept;

        std::shared_ptr<Graphics::API::ITexture2D> getTexture2D(const std::string& name) noexcept;
    };
}

#endif //SUNGEARENGINE_MATERIAL_H
