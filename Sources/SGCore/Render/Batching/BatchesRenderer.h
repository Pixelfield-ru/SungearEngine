//
// Created by ilya on 01.03.24.
//

#ifndef SUNGEARENGINE_BATCHESRENDERER_H
#define SUNGEARENGINE_BATCHESRENDERER_H

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct BatchesRenderer : public ISystem
    {
        void update(const double& dt, const double& fixedDt) noexcept;
    };
}

#endif //SUNGEARENGINE_BATCHESRENDERER_H
