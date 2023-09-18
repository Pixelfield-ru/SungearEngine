#ifndef SUNGEARENGINE_IPRIMITIVECOMPONENT_H
#define SUNGEARENGINE_IPRIMITIVECOMPONENT_H

#include "SGCore/ECS/IComponent.h"
#include "SGCore/Memory/Assets/ShaderAsset.h"
#include "SGCore/Graphics/GraphicsFilesResourcesManager.h"

#include <glm/vec3.hpp>

namespace Core::ECS
{
    class IPrimitiveComponent : public IComponent
    {
    public:
        std::shared_ptr<Memory::Assets::ShaderAsset> m_shaderAsset =
                Memory::AssetManager::loadAsset<Memory::Assets::ShaderAsset>(
                        Graphics::getShaderPath(Graphics::StandardShaderType::SG_PRIMITIVES_SHADER)
                );

        glm::vec3 offset;

        float m_width = 3.0;

    protected:
        void init() noexcept final { }
    };
}

#endif //SUNGEARENGINE_IPRIMITIVECOMPONENT_H
