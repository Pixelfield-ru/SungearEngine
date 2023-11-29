//
// Created by stuka on 03.08.2023.
//

#ifndef SUNGEARENGINE_SKYBOXRENDERINGSYSTEM_H
#define SUNGEARENGINE_SKYBOXRENDERINGSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace SGCore
{
    struct SkyboxesCollector : public ISystem
    {
        SkyboxesCollector();
        SG_COPY_CTOR(SkyboxesCollector);
        SG_MOVE_CTOR(SkyboxesCollector);
    };
}

#endif //SUNGEARENGINE_SKYBOXRENDERINGSYSTEM_H
