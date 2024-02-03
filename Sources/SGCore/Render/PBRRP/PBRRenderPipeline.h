#pragma once

#ifndef SUNGEARENGINE_CAMERARENDERINGSYSTEM_H
#define SUNGEARENGINE_CAMERARENDERINGSYSTEM_H

#include "SGCore/Render/IRenderPipeline.h"

namespace SGCore
{
    class IMeshData;
    class IMaterial;

    struct PBRRenderPipeline : public IRenderPipeline
    {
        PBRRenderPipeline();
        SG_COPY_CTOR(PBRRenderPipeline);
        SG_MOVE_CTOR(PBRRenderPipeline);
    };
}

#endif //SUNGEARENGINE_CAMERARENDERINGSYSTEM_H
