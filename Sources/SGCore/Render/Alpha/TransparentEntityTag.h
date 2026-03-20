//
// Created by stuka on 31.12.2024.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct TransparentEntityTag : public ECS::Component<TransparentEntityTag, const TransparentEntityTag>
    {
    private:
        bool m_dummy = false;
    };
}
