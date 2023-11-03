#include "GL46Renderer.h"

#include <thread>

#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Rendering/CameraComponent.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCasterComponent.h"

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

Core::Graphics::GL46Shader* Core::Graphics::GL46Renderer::createShader(const std::string& path)
{
    auto* shader = createShader();
    shader->compile(
            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::FileAsset>(path)
    );

    return shader;
}

Core::Graphics::GL46Texture2D* Core::Graphics::GL46Renderer::createTexture2D()
{
    return new GL46Texture2D;
}

const std::shared_ptr<Core::Graphics::GL46Renderer>& Core::Graphics::GL46Renderer::getInstance() noexcept
{
    static std::shared_ptr<GL46Renderer> s_instancePointer(new GL46Renderer);
    s_instancePointer->m_apiType = SG_API_TYPE_GL46;

    return s_instancePointer;
}
