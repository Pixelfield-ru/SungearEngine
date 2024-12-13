#include <SGCore/Logger/Logger.h>
#include "GL46Renderer.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/TextFileAsset.h"

bool SGCore::GL46Renderer::confirmSupport() noexcept
{
    std::string glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    if(!glVersion.starts_with("4.6"))
    {
        LOG_I(SGCORE_TAG,
              "OpenGL 4.6 is not supported!\n{0}",
              SG_CURRENT_LOCATION_STR);

        return false;
    }

    return true;
}

SGCore::GL46Shader* SGCore::GL46Renderer::createShader() const
{
    auto* shader = new GL46Shader;
    shader->m_version = "460";

    //GPUObjectsStorage::addShader(shader->weak_from_this());

    return shader;
}

SGCore::GL46Texture2D* SGCore::GL46Renderer::createTexture2D() const
{
    return new GL46Texture2D;
}

const std::shared_ptr<SGCore::GL46Renderer>& SGCore::GL46Renderer::getInstance() noexcept
{
    static std::shared_ptr<GL46Renderer> s_instancePointer(new GL46Renderer);
    s_instancePointer->m_apiType = SG_API_TYPE_GL46;

    return s_instancePointer;
}
