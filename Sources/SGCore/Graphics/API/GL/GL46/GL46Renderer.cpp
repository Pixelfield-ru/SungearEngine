#include "GL46Renderer.h"

#include <thread>

#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Rendering/CameraComponent.h"
#include "SGCore/ECS/Rendering/ShadowsCasterComponent.h"

bool Core::Graphics::GL46Renderer::confirmSupport() noexcept
{
    std::string glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    if(!glVersion.starts_with("4.6"))
    {
        SGCF_ERROR("OpengGL 4.6 is not supported!", SG_LOG_CURRENT_SESSION_FILE);

        return false;
    }

    return true;
}

Core::Graphics::GL46Shader* Core::Graphics::GL46Renderer::createPBRShader()
{
    auto* shader = new GL46Shader;
    shader->m_version = "460";
    shader->compile(
            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>(SG_GLSL46_PBR_SHADER_PATH)
    );

    return shader;
}

Core::Graphics::GL46Shader* Core::Graphics::GL46Renderer::createOnlyGeometryShader()
{
    auto* shader = new GL46Shader;
    shader->m_version = "460";
    shader->compile(
            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>(SG_GLSL4_ONLY_GEOM_SHADER_PATH)
    );

    return shader;
}

Core::Graphics::GL46Texture2D *Core::Graphics::GL46Renderer::createTexture2D()
{
    return new GL46Texture2D;
}

Core::Memory::Assets::IMaterial *Core::Graphics::GL46Renderer::createMaterial()
{
    auto* mat = new Memory::Assets::IMaterial;

    mat->m_shader = std::shared_ptr<Core::Graphics::IShader>(createPBRShader());

    // setting maximum textures of different types
    mat->getTextures()[SGMaterialTextureType::SGTP_EMISSIVE].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_AMBIENT_OCCLUSION].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_AMBIENT].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_DIFFUSE_ROUGHNESS].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_DIFFUSE].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_DISPLACEMENT].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_HEIGHT].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_NORMALS].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_BASE_COLOR].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_CLEARCOAT].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_EMISSION_COLOR].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_LIGHTMAP].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_METALNESS].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_NORMAL_CAMERA].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_OPACITY].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_REFLECTION].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_SHEEN].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_SHININESS].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_SPECULAR].m_maximumTextures = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_TRANSMISSION].m_maximumTextures = 1;

    // setting offsets
    mat->getTextures()[SGMaterialTextureType::SGTP_EMISSIVE].m_texturesUnitOffset = 0;
    mat->getTextures()[SGMaterialTextureType::SGTP_AMBIENT_OCCLUSION].m_texturesUnitOffset = 1;
    mat->getTextures()[SGMaterialTextureType::SGTP_AMBIENT].m_texturesUnitOffset = 2;
    mat->getTextures()[SGMaterialTextureType::SGTP_DIFFUSE_ROUGHNESS].m_texturesUnitOffset = 3;
    mat->getTextures()[SGMaterialTextureType::SGTP_DIFFUSE].m_texturesUnitOffset = 4;
    mat->getTextures()[SGMaterialTextureType::SGTP_DISPLACEMENT].m_texturesUnitOffset = 5;
    mat->getTextures()[SGMaterialTextureType::SGTP_HEIGHT].m_texturesUnitOffset = 6;
    mat->getTextures()[SGMaterialTextureType::SGTP_NORMALS].m_texturesUnitOffset = 7;
    mat->getTextures()[SGMaterialTextureType::SGTP_BASE_COLOR].m_texturesUnitOffset = 8;
    mat->getTextures()[SGMaterialTextureType::SGTP_CLEARCOAT].m_texturesUnitOffset = 9;
    mat->getTextures()[SGMaterialTextureType::SGTP_EMISSION_COLOR].m_texturesUnitOffset = 10;
    mat->getTextures()[SGMaterialTextureType::SGTP_LIGHTMAP].m_texturesUnitOffset = 11;
    mat->getTextures()[SGMaterialTextureType::SGTP_METALNESS].m_texturesUnitOffset = 12;
    mat->getTextures()[SGMaterialTextureType::SGTP_NORMAL_CAMERA].m_texturesUnitOffset = 13;
    mat->getTextures()[SGMaterialTextureType::SGTP_OPACITY].m_texturesUnitOffset = 14;
    mat->getTextures()[SGMaterialTextureType::SGTP_REFLECTION].m_texturesUnitOffset = 15;
    mat->getTextures()[SGMaterialTextureType::SGTP_SHEEN].m_texturesUnitOffset = 16;
    mat->getTextures()[SGMaterialTextureType::SGTP_SHININESS].m_texturesUnitOffset = 17;
    mat->getTextures()[SGMaterialTextureType::SGTP_SPECULAR].m_texturesUnitOffset = 18;
    mat->getTextures()[SGMaterialTextureType::SGTP_TRANSMISSION].m_texturesUnitOffset = 19;

    return mat;
}

const std::shared_ptr<Core::Graphics::GL46Renderer>& Core::Graphics::GL46Renderer::getInstance() noexcept
{
    static std::shared_ptr<GL46Renderer> s_instancePointer(new GL46Renderer);
    s_instancePointer->m_apiType = APIType::OPENGL;

    return s_instancePointer;
}
