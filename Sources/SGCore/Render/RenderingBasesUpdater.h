//
// Created by stuka on 03.02.2024.
//

#ifndef SUNGEARENGINE_RENDERINGBASESUPDATER_H
#define SUNGEARENGINE_RENDERINGBASESUPDATER_H

#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    struct RenderingBasesUpdater : public ISystem
    {
        sg_implement_type_id(RenderingBasesUpdater, 17)

        void fixedUpdate(const double& dt, const double& fixedDt) final;
    };
}

#endif //SUNGEARENGINE_RENDERINGBASESUPDATER_H
