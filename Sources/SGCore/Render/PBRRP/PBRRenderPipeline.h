#pragma once

#include "SGCore/Render/IRenderPipeline.h"

namespace SGCore
{
    class IMeshData;
    class IMaterial;

    struct SGCORE_EXPORT PBRRenderPipeline : public IRenderPipeline
    {
        // android: do not remove and do not =default
        ~PBRRenderPipeline() override;

        PBRRenderPipeline();
        SG_COPY_CTOR(PBRRenderPipeline);
        SG_MOVE_CTOR(PBRRenderPipeline);
    };
}
