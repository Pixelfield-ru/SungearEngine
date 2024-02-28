//
// Created by ilya on 28.02.24.
//

#ifndef OCEANSEDGE_INSTANCINGRENDERER_H
#define OCEANSEDGE_INSTANCINGRENDERER_H

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct InstancingRenderer : public ISystem
    {
        void update(const double& dt) override;
    };
}

#endif //OCEANSEDGE_INSTANCINGRENDERER_H
