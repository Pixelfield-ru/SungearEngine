//
// Created by stuka on 21.07.2025.
//

#ifndef INSTANCINGUPDATER_H
#define INSTANCINGUPDATER_H
#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct InstancingUpdater : ISystem
    {
        sg_implement_type_id(InstancingUpdater, 36)

        void update(const double& dt, const double& fixedDt) noexcept final;
    };
}

#endif //INSTANCINGUPDATER_H
