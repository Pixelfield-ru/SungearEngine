//
// Created by stuka on 03.08.2023.
//

#ifndef SUNGEARENGINE_SKYBOXCOMPONENT_H
#define SUNGEARENGINE_SKYBOXCOMPONENT_H

#include "SGCore/ECS/IComponent.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/Defines.h"
#include "SGCore/Memory/Assets/ShaderAsset.h"
#include "SGCore/Utils/ShadersPaths.h"

namespace SGCore
{
    class Skybox : public IComponent
    {
    private:
        void init() noexcept final { }
    };
}

#endif //SUNGEARENGINE_SKYBOXCOMPONENT_H
