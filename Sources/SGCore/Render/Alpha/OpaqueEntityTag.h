//
// Created by stuka on 31.12.2024.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct SGCORE_EXPORT OpaqueEntityTag : public ECS::Component<OpaqueEntityTag, const OpaqueEntityTag>
    {
    private:
        bool m_dummy = false;
    };
}
