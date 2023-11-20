#pragma once

#ifndef SUNGEARENGINE_CAMERARENDERINGSYSTEM_H
#define SUNGEARENGINE_CAMERARENDERINGSYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "IRenderPipeline.h"

namespace SGCore
{
    class IMeshData;
    class IMaterial;

    class PBRForwardRenderPipeline : public IRenderPipeline
    {
        SG_DECLARE_COPY_MOVE_SINGLETON(PBRForwardRenderPipeline)

    protected:
        PBRForwardRenderPipeline();

    private:
        void updateUniforms(const Ref<IShader>& shader,
                            const Ref<IMaterial>& material,
                            const Ref<Transform>& transformComponent) const noexcept;

    public:
        void update(const Ref<Scene>& scene) final;

        void cacheEntity(const Ref<Entity>& entity) final;
    };
}

#endif //SUNGEARENGINE_CAMERARENDERINGSYSTEM_H
