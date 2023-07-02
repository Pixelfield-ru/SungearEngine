#include "PBRMaterial.h"
#include "SGCore/Main/Core.h"

Core::Memory::Assets::PBRMaterial::PBRMaterial()
{
    m_shader = std::shared_ptr<Core::Graphics::API::IShader>(Core::Main::Core::getRenderer().createShader());
    m_shader->compile(Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>("../SGResources/shaders/pbr/default_shader.glsl"));
    m_shader->addShaderDefines({
                                       //Core::Graphics::API::ShaderDefine("FLIP_TEXTURES_Y", "")
                               });

    /*m_maxEmissive = 1;
    m_maxAmbientOcclusion = 1;
    m_maxAmbient = 1;
    m_maxDiffuseRoughness = 1;
    m_maxDiffuse = 1;
    m_maxDisplacement = 1;
    m_maxHeight = 1;
    m_maxNormals = 1;
    m_maxBaseColor = 1;
    m_maxClearcoat = 1;
    m_maxEmissionColor = 1;
    m_maxLightmap = 1;
    m_maxMetalness = 1;
    m_maxNormalCamera = 1;
    m_maxOpacity = 1;
    m_maxReflection = 1;
    m_maxSheen = 1;
    m_maxShininess= 1;
    m_maxSpecular = 1;
    m_maxTransmission = 1;*/

    get<0>(m_textures[SGMaterialTextureType::SGTP_EMISSIVE]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_AMBIENT_OCCLUSION]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_AMBIENT]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_DIFFUSE_ROUGHNESS]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_DIFFUSE]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_DISPLACEMENT]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_HEIGHT]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_NORMALS]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_BASE_COLOR]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_CLEARCOAT]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_EMISSION_COLOR]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_LIGHTMAP]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_METALNESS]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_NORMAL_CAMERA]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_OPACITY]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_REFLECTION]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_SHEEN]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_SHININESS]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_SPECULAR]) = 1;
    get<0>(m_textures[SGMaterialTextureType::SGTP_TRANSMISSION]) = 1;

    get<1>(m_textures[SGMaterialTextureType::SGTP_EMISSIVE]) = 0;
    get<1>(m_textures[SGMaterialTextureType::SGTP_AMBIENT_OCCLUSION]) = 1;
    get<1>(m_textures[SGMaterialTextureType::SGTP_AMBIENT]) = 2;
    get<1>(m_textures[SGMaterialTextureType::SGTP_DIFFUSE_ROUGHNESS]) = 3;
    get<1>(m_textures[SGMaterialTextureType::SGTP_DIFFUSE]) = 4;
    get<1>(m_textures[SGMaterialTextureType::SGTP_DISPLACEMENT]) = 5;
    get<1>(m_textures[SGMaterialTextureType::SGTP_HEIGHT]) = 6;
    get<1>(m_textures[SGMaterialTextureType::SGTP_NORMALS]) = 7;
    get<1>(m_textures[SGMaterialTextureType::SGTP_BASE_COLOR]) = 8;
    get<1>(m_textures[SGMaterialTextureType::SGTP_CLEARCOAT]) = 9;
    get<1>(m_textures[SGMaterialTextureType::SGTP_EMISSION_COLOR]) = 10;
    get<1>(m_textures[SGMaterialTextureType::SGTP_LIGHTMAP]) = 11;
    get<1>(m_textures[SGMaterialTextureType::SGTP_METALNESS]) = 12;
    get<1>(m_textures[SGMaterialTextureType::SGTP_NORMAL_CAMERA]) = 13;
    get<1>(m_textures[SGMaterialTextureType::SGTP_OPACITY]) = 14;
    get<1>(m_textures[SGMaterialTextureType::SGTP_REFLECTION]) = 15;
    get<1>(m_textures[SGMaterialTextureType::SGTP_SHEEN]) = 16;
    get<1>(m_textures[SGMaterialTextureType::SGTP_SHININESS]) = 17;
    get<1>(m_textures[SGMaterialTextureType::SGTP_SPECULAR]) = 18;
    get<1>(m_textures[SGMaterialTextureType::SGTP_TRANSMISSION]) = 19;
}

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::PBRMaterial::load(const std::string& path)
{
    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::PBRMaterial::addTexture2D(const SGMaterialTextureType& type,
                                                                           const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset)
{
    return shared_from_this();
}

// TODO: MOVE ALL THIS METHODS TO THE BASE CLASS
std::shared_ptr<Core::Memory::Assets::IMaterial>
Core::Memory::Assets::PBRMaterial::findAndAddTexture2D(const SGMaterialTextureType& type, const std::string& path)
{
    auto& texturesWithType = m_textures[type];

    auto& unpackedMaximumTextures = get<0>(texturesWithType);
    auto& unpackedTextureUnitOffset = get<1>(texturesWithType);
    auto& unpackedTexturesWithType = get<2>(texturesWithType);

    // texturesWithType.second IS MAP
    // texturesWithType.first IS MAXIMUM OF TEXTURES OF THIS TYPE

    // if this texture not exists
    if(unpackedTexturesWithType.find(path) == unpackedTexturesWithType.end())
    {
        auto foundTex = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(path);

        // current max found unit
        int maxUnit = unpackedTextureUnitOffset;
        if(!unpackedTexturesWithType.empty())
        {
            // finding current maximum texture unit
            for(auto& texPair : unpackedTexturesWithType)
            {
                if(texPair.second.first > maxUnit)
                {
                    maxUnit = texPair.second.first;
                }
            }

            maxUnit += 1;
        }

        // if there is free space for another texture of this type
        if(maxUnit < maxUnit + unpackedMaximumTextures)
        {
            // this is texture unit to bind for this texture
            unpackedTexturesWithType[path].first = maxUnit;
            unpackedTexturesWithType[path].second = foundTex;

            std::string finalName = sgMaterialTextureTypeToString(type)  + std::to_string(maxUnit) + "_DEFINED";
            SGC_SUCCESS("Final define for PBR texture: " + finalName);
            m_shader->addShaderDefines({Graphics::API::ShaderDefine(sgMaterialTextureTypeToString(type)  + std::to_string(maxUnit) + "_DEFINED", "")});
        }
    }


    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IMaterial> Core::Memory::Assets::PBRMaterial::setTexture2D
(const SGMaterialTextureType& type, const std::string_view& name, const std::shared_ptr<Memory::Assets::Texture2DAsset>& texture2DAsset)
{
    return shared_from_this();
}

std::shared_ptr<Core::Memory::Assets::IMaterial>
Core::Memory::Assets::PBRMaterial::findAndSetTexture2D(const SGMaterialTextureType& type, const std::string& path)
{
    return shared_from_this();
}
