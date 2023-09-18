//
// Created by stuka on 03.08.2023.
//

#ifndef SUNGEARENGINE_SKYBOXCOMPONENT_H
#define SUNGEARENGINE_SKYBOXCOMPONENT_H

#include "SGCore/ECS/IComponent.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/GraphicsFilesResourcesManager.h"
#include "SGCore/Memory/Assets/ShaderAsset.h"

namespace Core::ECS
{
    class SkyboxComponent : public IComponent
    {
    private:
        void init() noexcept final { }

    public:
        std::shared_ptr<Memory::Assets::ShaderAsset> m_shaderAsset =
                Memory::AssetManager::loadAsset<Memory::Assets::ShaderAsset>(
                        Graphics::getShaderPath(Graphics::StandardShaderType::SG_SKYBOX_SHADER)
                );
    };
}

#endif //SUNGEARENGINE_SKYBOXCOMPONENT_H
