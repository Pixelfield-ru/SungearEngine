#pragma once

#ifndef SUNGEARENGINE_CAMERARENDERINGSYSTEM_H
#define SUNGEARENGINE_CAMERARENDERINGSYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "SGCore/ECS/Rendering/Pipelines/IRenderPipeline.h"

namespace SGCore
{
    class IMeshData;
    class IMaterial;

    struct PBRForwardRenderPipeline : public IRenderPipeline
    {
        PBRForwardRenderPipeline();
        SG_COPY_CTOR(PBRForwardRenderPipeline);
        SG_MOVE_CTOR(PBRForwardRenderPipeline);

        void update(const Ref<Scene>& scene) noexcept override;

        void useScene(const Ref<Scene>& scene) override;
    };
}

#endif //SUNGEARENGINE_CAMERARENDERINGSYSTEM_H
