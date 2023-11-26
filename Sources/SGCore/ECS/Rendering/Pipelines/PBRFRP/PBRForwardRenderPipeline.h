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
        SG_CUSTOM_CTOR_SINGLETON(PBRForwardRenderPipeline)
        SG_COPY_SINGLETON(PBRForwardRenderPipeline)
        SG_MOVE_SINGLETON(PBRForwardRenderPipeline)
    };
}

#endif //SUNGEARENGINE_CAMERARENDERINGSYSTEM_H
