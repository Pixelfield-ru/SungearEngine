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

Core::Graphics::GL46Shader* Core::Graphics::GL46Renderer::createShader()
{
    auto* shader = new GL46Shader;
    shader->m_version = "460";

    return shader;
}

Core::Graphics::GL46Shader* Core::Graphics::GL46Renderer::createPBRShader()
{
    auto* shader = createShader();
    shader->compile(
            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>(SG_GLSL46_PBR_SHADER_PATH)
    );

    return shader;
}

Core::Graphics::GL46Shader* Core::Graphics::GL46Renderer::createOnlyGeometryShader()
{
    auto* shader = createShader();
    shader->compile(
            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>(SG_GLSL4_SHADOWS_GENERATOR_SHADER_PATH)
    );

    return shader;
}

Core::Graphics::GL46Texture2D* Core::Graphics::GL46Renderer::createTexture2D()
{
    return new GL46Texture2D;
}

Core::Memory::Assets::IMaterial *Core::Graphics::GL46Renderer::createMaterial()
{
    auto* mat = new Memory::Assets::IMaterial;

    mat->setShader(std::shared_ptr<Core::Graphics::IShader>(createPBRShader()));

    // adding block decls
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_EMISSIVE,
                             1, 0);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_AMBIENT_OCCLUSION,
                             1, 1);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_AMBIENT,
                             1, 2);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_DIFFUSE_ROUGHNESS,
                             1, 3);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_DIFFUSE,
                             1, 4);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_DISPLACEMENT,
                             1, 5);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_HEIGHT,
                             1, 6);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_NORMALS,
                             1, 7);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_BASE_COLOR,
                             1, 8);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_CLEARCOAT,
                             1, 9);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_EMISSION_COLOR,
                             1, 10);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_LIGHTMAP,
                             1, 11);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_METALNESS,
                             1, 12);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_NORMAL_CAMERA,
                             1, 13);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_OPACITY,
                             1, 14);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_REFLECTION,
                             1, 15);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_SHEEN,
                             1, 16);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_SHININESS,
                             1, 17);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_SPECULAR,
                             1, 18);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_TRANSMISSION,
                             1, 19);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_SHADOW_MAP,
                             5, 20);
    mat->addBlockDeclaration(SGMaterialTextureType::SGTP_SKYBOX,
                             1, 25);

    return mat;
}

const std::shared_ptr<Core::Graphics::GL46Renderer>& Core::Graphics::GL46Renderer::getInstance() noexcept
{
    static std::shared_ptr<GL46Renderer> s_instancePointer(new GL46Renderer);
    s_instancePointer->m_apiType = APIType::OPENGL;

    return s_instancePointer;
}
