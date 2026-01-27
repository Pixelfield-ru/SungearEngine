//
// Created by stuka on 26.01.2026.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct EnableBatchingPass : ECS::Component<EnableBatchingPass, const EnableBatchingPass>
    {
    private:
        bool m_dummy = false;
    };
}
