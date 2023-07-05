#include "PBRMaterial.h"
#include "SGCore/Main/Core.h"

// TODO: make check for current used GAPI
Core::Memory::Assets::PBRMaterial::PBRMaterial()
{
    m_shader = std::shared_ptr<Core::Graphics::IShader>(Core::Main::Core::getRenderer().createShader());
    m_shader->compile(Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>("../SGResources/shaders/pbr/default_shader.glsl"));
    m_shader->addShaderDefines({
                                       //Core::Graphics::API::ShaderDefine("FLIP_TEXTURES_Y", "")
                               });

    // setting maximum textures of different types
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

    // setting offsets
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
