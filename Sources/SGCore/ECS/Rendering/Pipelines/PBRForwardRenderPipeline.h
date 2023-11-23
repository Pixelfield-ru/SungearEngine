#pragma once

#ifndef SUNGEARENGINE_CAMERARENDERINGSYSTEM_H
#define SUNGEARENGINE_CAMERARENDERINGSYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "IRenderPipeline.h"

namespace SGCore
{
    class IMeshData;
    class IMaterial;

    struct PBRForwardRenderPipeline : public IRenderPipeline
    {
        void update(const Ref<Scene>& scene) final;

    private:
        void updateUniforms(const Ref<IShader>& shader,
                            const Ref<IMaterial>& material,
                            const Ref<Transform>& transformComponent) const noexcept;

        SG_CUSTOM_CTOR_SINGLETON(PBRForwardRenderPipeline)
        SG_COPY_SINGLETON(PBRForwardRenderPipeline)
        SG_MOVE_SINGLETON(PBRForwardRenderPipeline)
    };
}

#endif //SUNGEARENGINE_CAMERARENDERINGSYSTEM_H
