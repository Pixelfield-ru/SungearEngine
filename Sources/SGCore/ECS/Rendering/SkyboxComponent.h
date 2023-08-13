//
// Created by stuka on 03.08.2023.
//

#ifndef SUNGEARENGINE_SKYBOXCOMPONENT_H
#define SUNGEARENGINE_SKYBOXCOMPONENT_H

#include "SGCore/ECS/IComponent.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Main/CoreMain.h"

namespace Core::ECS
{
    class SkyboxComponent : public IComponent
    {
    private:
        void init() noexcept final { }

    public:
        // loading standard skybox
        /*std::shared_ptr<Memory::Assets::Texture2DAsset> m_texture =
                Memory::AssetManager::loadAsset<Memory::Assets::Texture2DAsset>(
                        "../SGResources/textures/skyboxes/standard_skybox0.png",
                        SGTextureType::SGG_CUBE_MAP_TEXTURE
                        );*/
        // maybe static

        std::shared_ptr<Graphics::IShader> m_shader = std::shared_ptr<Graphics::IShader>(
                Main::CoreMain::getRenderer().createSkyboxShader()
        );
    };
}

#endif //SUNGEARENGINE_SKYBOXCOMPONENT_H
