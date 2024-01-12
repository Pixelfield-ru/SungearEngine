#ifndef SUNGEARENGINE_LINECOMPONENT_H
#define SUNGEARENGINE_LINECOMPONENT_H

#include "IGizmo.h"

namespace SGCore
{
    struct LineGizmo : public IGizmo
    {
        explicit LineGizmo(const Ref<IRenderPipeline>& pipeline) noexcept;
    };
}

#endif //SUNGEARENGINE_LINECOMPONENT_H
