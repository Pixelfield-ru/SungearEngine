//
// Created by Ilya on 06.11.2023.
//

#ifndef SUNGEARENGINE_COMPLEXPRIMITIVECOMPONENT_H
#define SUNGEARENGINE_COMPLEXPRIMITIVECOMPONENT_H

#include "IGizmo.h"

namespace SGCore
{
    struct IComplexGizmo : public IGizmo
    {
        IComplexGizmo(const Ref<IRenderPipeline>& pipeline) noexcept;
    };
}

#endif // SUNGEARENGINE_COMPLEXPRIMITIVECOMPONENT_H
