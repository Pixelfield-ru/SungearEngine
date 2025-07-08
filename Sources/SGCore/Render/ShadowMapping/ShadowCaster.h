//
// Created by stuka on 08.07.2025.
//

#ifndef SUNGEARENGINE_SHADOWCASTER_H
#define SUNGEARENGINE_SHADOWCASTER_H

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    /// Tag to indicate object that can cast shadows.
    struct ShadowCaster : ECS::Component<ShadowCaster, const ShadowCaster>
    {
    private:
        bool m_dummy = false;
    };
}

#endif // SUNGEARENGINE_SHADOWCASTER_H
