//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_LINEGIZMOSUPDATER_H
#define SUNGEARENGINE_LINEGIZMOSUPDATER_H

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct LineGizmosRenderer : public ISystem
    {
        void fixedUpdate(const double& dt, const double& fixedDt) final;
    };
}

#endif //SUNGEARENGINE_LINEGIZMOSUPDATER_H
