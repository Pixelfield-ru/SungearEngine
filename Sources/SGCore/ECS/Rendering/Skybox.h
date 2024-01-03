//
// Created by stuka on 03.08.2023.
//

#ifndef SUNGEARENGINE_SKYBOXCOMPONENT_H
#define SUNGEARENGINE_SKYBOXCOMPONENT_H

#include "SGCore/ECS/IComponent.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/ISubPassShader.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/Defines.h"
#include "SGCore/Memory/Assets/ShaderFileAsset.h"
#include "SGCore/Utils/ShadersPaths.h"
#include "Mesh.h"

namespace SGCore
{
    class Skybox : public Mesh
    {
        void registerRenderPipelineIfNotRegistered(const Ref<IRenderPipeline>& pipeline) noexcept override;
    };
}

#endif //SUNGEARENGINE_SKYBOXCOMPONENT_H
