#include "GL46Renderer.h"

#include <thread>

#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/Rendering/Camera.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCaster.h"

bool SGCore::GL46Renderer::confirmSupport() noexcept
{
    std::string glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    if(!glVersion.starts_with("4.6"))
    {
        SGCF_ERROR("OpengGL 4.6 is not supported!", SG_LOG_CURRENT_SESSION_FILE);

        return false;
    }

    return true;
}

SGCore::GL46Shader* SGCore::GL46Renderer::createShader()
{
    auto* shader = new GL46Shader;
    shader->m_version = "460";

    return shader;
}

SGCore::GL46Shader* SGCore::GL46Renderer::createShader(const std::string& path)
{
    auto* shader = createShader();
    shader->compile(
            AssetManager::loadAsset<FileAsset>(path)
    );

    return shader;
}

SGCore::GL46Texture2D* SGCore::GL46Renderer::createTexture2D()
{
    return new GL46Texture2D;
}

const std::shared_ptr<SGCore::GL46Renderer>& SGCore::GL46Renderer::getInstance() noexcept
{
    static std::shared_ptr<GL46Renderer> s_instancePointer(new GL46Renderer);
    s_instancePointer->m_apiType = SG_API_TYPE_GL46;

    return s_instancePointer;
}
