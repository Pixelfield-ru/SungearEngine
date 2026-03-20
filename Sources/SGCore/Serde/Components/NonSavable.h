//
// Created by stuka on 10.09.2024.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct SGCORE_EXPORT NonSavable : ECS::Component<NonSavable, const NonSavable>
    {
    private:
        bool m_dummy { };
    };
}
