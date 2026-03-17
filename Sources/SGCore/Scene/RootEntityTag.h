//
// Created by stuka on 17.03.2026.
//

#pragma once

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct RootEntityTag : ECS::Component<RootEntityTag, const RootEntityTag>
    {

    };
}