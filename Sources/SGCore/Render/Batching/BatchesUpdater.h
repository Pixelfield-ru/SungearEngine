//
// Created by stuka on 18.07.2025.
//

#ifndef SUNGEARENGINE_BATCHESUPDATER_H
#define SUNGEARENGINE_BATCHESUPDATER_H

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct BatchesUpdater : ISystem
    {
        sg_implement_type_id(BatchesUpdater, 35)

        void update(const double& dt, const double& fixedDt) noexcept final;
    };
}

#endif // SUNGEARENGINE_BATCHESUPDATER_H
