//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_BOXGIZMOSUPDATER_H
#define SUNGEARENGINE_BOXGIZMOSUPDATER_H

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    class BoxGizmosRenderer : public ISystem
    {
        sg_implement_type_id(BoxGizmosRenderer, 21)

        void fixedUpdate(const double& dt, const double& fixedDt) final;
    };
}

#endif //SUNGEARENGINE_BOXGIZMOSUPDATER_H
