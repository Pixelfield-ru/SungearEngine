//
// Created by stuka on 27.04.2025.
//

#ifndef SUNGEARENGINE_DECAL_H
#define SUNGEARENGINE_DECAL_H

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct Decal : ECS::Component<Decal, const Decal>
    {
    private:
        bool m_dummy = false;
    };
}

#endif // SUNGEARENGINE_DECAL_H
