//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_SPHEREGIZMOSUPDATER_H
#define SUNGEARENGINE_SPHEREGIZMOSUPDATER_H

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct SphereGizmosUpdater : public ISystem
    {
        void fixedUpdate() final;
    };
}

#endif //SUNGEARENGINE_SPHEREGIZMOSUPDATER_H
