//
// Created by stuka on 01.08.2023.
//

#ifndef SUNGEARENGINE_DIRECTIONALLIGHTSSYSTEM_H
#define SUNGEARENGINE_DIRECTIONALLIGHTSSYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "PBRFRPDirectionalLight.h"

namespace SGCore
{
    struct PBRFRPDirectionalLightsCollector : public ISystem
    {
        PBRFRPDirectionalLightsCollector();
        SG_COPY_CTOR(PBRFRPDirectionalLightsCollector);
        SG_MOVE_CTOR(PBRFRPDirectionalLightsCollector);
    };
}

#endif //SUNGEARENGINE_DIRECTIONALLIGHTSSYSTEM_H
