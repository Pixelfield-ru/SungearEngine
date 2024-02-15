//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_BOXGIZMOSUPDATER_H
#define SUNGEARENGINE_BOXGIZMOSUPDATER_H

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    class BoxGizmosUpdater : public ISystem
    {
        void fixedUpdate(const Ref<Scene>& scene) final;
    };
}

#endif //SUNGEARENGINE_BOXGIZMOSUPDATER_H