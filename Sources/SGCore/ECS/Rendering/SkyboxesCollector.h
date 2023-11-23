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
        SG_CUSTOM_CTOR_SINGLETON(SkyboxesCollector)
        SG_COPY_SINGLETON(SkyboxesCollector)
        SG_MOVE_SINGLETON(SkyboxesCollector)
    };
}

#endif //SUNGEARENGINE_SKYBOXRENDERINGSYSTEM_H
