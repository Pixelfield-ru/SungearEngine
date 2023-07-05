#pragma once

#ifndef SUNGEARENGINE_IMATERIAL_H
#define SUNGEARENGINE_IMATERIAL_H

#include <map>
#include <string>

#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/IAsset.h"

namespace Core::Memory::Assets
{
    class IMaterial : public std::enable_shared_from_this<IMaterial>, public IAsset
    {
    protected:
        // EXAMPLE:
        // first: material texture type
        // second : std::pair<std::uint8_t, std::map<std::string, std::pair<std::uint8_t, std::shared_ptr<Texture2DAsset>>>>
        //      first: maximum textures of this type
        //      second: offset of first texture block for this texture type
        //      third: std::map<std::string, std::pair<std::uint8_t, std::shared_ptr<Texture2DAsset>>>
        //          first: std::string - path to texture
        //          second: std::tuple<std::uint8_t, std::shared_ptr<Texture2DAsset>> - Texture2DAsset
        //              first: std::uint8_t - texture unit to bind for this texture (watch down - second)
        //              second: std::shared_ptr<Texture2DAsset> - texture 2d asset
        std::map<SGMaterialTextureType, std::tuple<std::uint8_t, std::uint8_t, std::map<std::string, std::pair<std::uint8_t, std::shared_ptr<Texture2DAsset>>>>> m_textures;

        // Textures that could not be added to the material. This collection is used to migrate textures to another material
        std::map<SGMaterialTextureType, std::list<std::shared_ptr<Texture2DAsset>>> m_notUsedTextures;

    public:
        std::string m_name;

        std::shared_ptr<Graphics::IShader> m_shader;

        std::shared_ptr<IMaterial> bind();

        // TODO: impl
        std::shared_ptr<IAsset> load(const std::string& path) override;

        /**
        * Adds texture2D. Method is not copying texture.
        * @param type - Material type of texture
        * @param texture2DAsset - Texture asset
        * @return this
        */
        std::shared_ptr<IMaterial> addTexture2D(const SGMaterialTextureType& type,
                                                const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset);

        /**
        * Adds texture2D. Method is copying texture. This method is looking for texture asset by path.
        * @param type - Material type of texture
        * @param texture2DAsset - Texture asset
        * @return this
        */
        std::shared_ptr<IMaterial> findAndAddTexture2D(const SGMaterialTextureType& type, const std::string& path);

        /**
         * Sets texture2D by name. Method is copying texture.
         * @param type - Material type of texture
         * @param oldTextureAssetPath - The path to the texture to be replaced
         * @param texture2DAsset - Texture asset that will replace the old texture
         *
         * @return this (shared_ptr)
         */
        std::shared_ptr<IMaterial> setTexture2D(const SGMaterialTextureType& type, const std::string_view& oldTextureAssetPath,
                                                const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset);
        /**
         * Sets texture2D by name. Method is copying texture. This method is looking for texture asset by path.
         * @param type - Material type of texture
         * @param oldTextureAssetPath - The path to the texture to be replaced
         * @param newTextureAssetPath - The path to the new texture
         *
         * @return this (shared_ptr)
         */
        std::shared_ptr<IMaterial> findAndSetTexture2D(const SGMaterialTextureType& type, const std::string& oldTextureAssetPath, const std::string& newTextureAssetPath);

        /**
         * Copies all texture assets (textures data is not copied) to another material.\n
         * Also tries to do the same with unused textures of this material.
         * @param other - Copied material
         * @return Modified this material
         */
        IMaterial& operator=(const IMaterial& other) noexcept;
    };
}

#endif //SUNGEARENGINE_IMATERIAL_H
