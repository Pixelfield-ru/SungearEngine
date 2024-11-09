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
        sg_implement_type_id(SphereGizmosUpdater, 24)

        void fixedUpdate(const double& dt, const double& fixedDt) final;
    };
}

#endif //SUNGEARENGINE_SPHEREGIZMOSUPDATER_H
