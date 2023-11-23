//
// Created by stuka on 01.08.2023.
//

#ifndef SUNGEARENGINE_DIRECTIONALLIGHTSSYSTEM_H
#define SUNGEARENGINE_DIRECTIONALLIGHTSSYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "DirectionalLight.h"

namespace SGCore
{
    struct DirectionalLightsCollector : public ISystem
    {
        SG_CUSTOM_CTOR_SINGLETON(DirectionalLightsCollector)
        SG_COPY_SINGLETON(DirectionalLightsCollector)
        SG_MOVE_SINGLETON(DirectionalLightsCollector)
    };
}

#endif //SUNGEARENGINE_DIRECTIONALLIGHTSSYSTEM_H
